%% ===================================================================== %%
% This script implements the performance evaluation of Blind Audio Source %
% Separation in a perceptual objective concept, where the quality of the  %
% separation is based on comparing the critical band smoothed spectrum of %
% the estimated source and true source.                                   %
%% ===================================================================== %%
% Version: 1.0
% Created by: Kai-Chieh Huang, 2015, Dec, 4th

%% ===================================================================== %%
% The evaluation is carried out by first performing critcal band smooth-  %
% ing on the estimated source and all true sources. Then the critical b-  %
% and smoothed spectrum of the estimated source is decomposed into thre-  %
% e parts: target source, interference error, and artifact error by doing %
% projections. The decomposition is defined as follows:                   %
%                                                                         %
% (cbss stands for critical band smoothed spectrum)                       %
% ------------------------------------------------------------------------%
% cbss_estimatedSource = cbss_targetSource + e_interf + e_artif           %
% ------------------------------------------------------------------------%
% Where:                                                                  %
% ------------------------------------------------------------------------%
% cbss_targetSource = cbss_estimatedSource project on cbss_trueSource     %
% e_interf = sum of cbss_estimatedSource project on all other             %
%            cbss_trueSources                                             %
% e_artif  = cbss_estimatedSource - gain*cbss_trueSource - e_interf       %
% ------------------------------------------------------------------------%
% Several term is defined and calculated using the decomposition result:  %
%                                                                         %
% SDR: Source to Distortion Ratio                                         %
% SDR = 10*log(  |cbss_targetSource|^2 / |e_interf + e_artif|^2 )         %
% SIR: Source to Interferences Ratio                                      %
% SIR = 10*log(  |cbss_targetSource|^2 / |e_interf|^2 )                   %
% SAR: Sources to Artifacts Ratio                                         %
% SAR = 10*log(  |cbss_targetSource + e_interf|^2 / |e_artif|^2 )         %
%-------------------------------------------------------------------------%
% By computing these three ratios, we can understand how well the Blind   %
% audio source separation algorithm is doing in terms of suppressing the  %
% interferences, artifacts, or just general accuracy compare to the targe-%
% ed true source.                                                         %
% ------------------------------------------------------------------------%
% Reference:https://hal.inria.fr/inria-00544230/PDF/vincent_TASLP06bis.pdf%
% Toolbox: http://bass-db.gforge.inria.fr/bss_eval/                       %
%% ===================================================================== %%

%% Function Usage ====================================================== %%
%  Input: 
%  1. estimatedSources -> matrix which each row representing one of the estimated source from ICA 
%  2. trueSources -> matrix which rows correspond to the original sources
%  3. fs -> the sampling rate of all the audio sources
%  4. algorithmName -> the algorithm name currently using to generate the estimated sources 
%-----------------------------------------------------------------------------------------%
%  Output: 
%  evaluationResult -> A matrix which each row contains the SDR, SIR, SAR %
%  values for the estimated source at that index. Where the SDR, SIR, SAR % 
%  are defined as previously and the range is as follows:                 %
%
%  All the values are -inf(low performance) to inf(high performance)      %
%  1. SDR: Source to Distortion Ratio (in dB)    ->  -inf to inf          %
%  2. SIR: Source to Interferences Ratio (in dB) ->  -inf to inf          %
%  3. SAR: Sources to Artifacts Ratio (in dB)    ->  -inf to inf          %
%-----------------------------------------------------------------------------------------%
function [evaluationResult] = bssEvaluation(estimatedSources,trueSources,fs,algorithmName) 

%% Pairing estimated sources to true sources
numSources = size(estimatedSources,1);
pairingIndices = ones(numSources,1);
for i = 1:numSources 
    innerProductMax = 0;
    for j = 1:numSources
        innerProduct = estimatedSources(i,:) * trueSources(j,:)';
        if  (innerProduct > innerProductMax)
            innerProductMax = innerProduct;
            pairingIndices(i) = j;
        end
    end
end

%% Compute the spectrum of the estimated source and all the true sources
% Global variables (Here we assume all the sources has been processed to the same length)
hanningWindow = hanning(length(estimatedSources(1,:)))';
NFFT = 2^nextpow2(length(estimatedSources(1,:)));
f = fs/2 * (0:2/NFFT:1);
estimatedSourceSpectrums = zeros(numSources,NFFT/2+1);
trueSourcesSpectrums = zeros(numSources,NFFT/2+1);
for i = 1:numSources
    % Caculate the spectrum of estimated source
    fprintf('Computing estimated source spectrum %i ...\n',i);
    windowed_estimatedSource = estimatedSources(i,:) .* hanningWindow;
    estimatedSourceSpectrum = fft(windowed_estimatedSource,NFFT)/length(windowed_estimatedSource);
    estimatedSourceSpectrums(i,:) = estimatedSourceSpectrum(1:NFFT/2+1);
    % Calculate the spectrum of all true sources
    fprintf('Computing true source spectrum %i ...\n',i);
    windowed_trueSource = trueSources(i,:) .* hanningWindow;
    trueSourceSpectrum = fft(windowed_trueSource,NFFT)/length(windowed_trueSource);
    trueSourcesSpectrums(i,:) = trueSourceSpectrum(1:NFFT/2+1);
end

%% Perform critical band smoothing on the estimated source and all the true sources
beta = 0.5; % Smoothe the spectrum of the sources with 0.5 critical band
cbs_estimatedSourceSpectrums = zeros(numSources,NFFT/2+1);
cbs_trueSourcesSpectrums = zeros(numSources,NFFT/2+1);
for i = 1:numSources
    % Perform critical band smoothing on the estimated source
    fprintf('Performing critical band smoothing on estimated source spectrum %i ...\n',i);
    cbs_estimatedSourceSpectrums(i,:) = cbsmooth(estimatedSourceSpectrums(i,:),beta,fs);
    % Perform critical band smoothing on all the true sources
    fprintf('Performing critical band smoothing on true source spectrum %i ...\n',i);
    cbs_trueSourcesSpectrums(i,:) = cbsmooth(trueSourcesSpectrums(i,:),beta,fs);
end

%% Compute performance measure: SDR, SIR, SAR
evaluationResult = zeros(numSources,3);
for i = 1:numSources
    % Compute the decomposition
    fprintf('Decomposing the estimated source spectrum %i ...\n',i);
    [s_target,e_interf,e_artif] = bss_decomp_gain(abs(estimatedSourceSpectrums(i,:)),pairingIndices(i),abs(cbs_trueSourcesSpectrums));
    % Compute SDR, SIR, SAR
    fprintf('Evaluating the source separation quality %i ...\n',i);
    [SDR,SIR,SAR] = bss_crit(s_target,e_interf,e_artif);
    evaluationResult(i,:) = [SDR SIR SAR];
end

%% Show Evaluation result on all estimated sources
%** Should address any "inf" in the evaluation result in order to plot **%
figure(1);
h = bar(evaluationResult,'hist');
set(h(1),'FaceColor',[255,20,147]/255,'EdgeColor','k');
set(h(2),'FaceColor',[0,191,255]/255,'EdgeColor','k');
set(h(3),'FaceColor',[112,128,144]/255,'EdgeColor','k');
grid on;
l = cell(1,3);
l{1} = 'SDR'; l{2} = 'SIR'; l{3} = 'SAR';
leg = legend(h,l);
set(leg,'Location','BestOutside');
str = sprintf('Performance Evaluation on Blind Source Separation Algorithm: %s',algorithmName);
title(str);
xlabel('Index of Estimated Sources');
ylabel('Performance (dB)');
end