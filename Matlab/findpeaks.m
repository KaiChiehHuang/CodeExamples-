% Music 421 Lab5: Question 2
function [peaks,freqs]=findpeaks(Xwdb,maxPeaks,fs,win,N)
% peaks = a vector containing the peak magnitude estimates (linear) using
%         parabolic interpolation in order from largest to smallest peak.
% freqs = a vector containing the frequency estimates (Hz) corresponding
%         to the peaks defined above
% Xwdb  = DFT magnitude (in dB scale) vector of a windowed signal.
%         NOTE that it may contain
%         only low-frequency (length < N/2+1), positive-frequency
%         (length = N/2+1), or all (length = N) bins of the FFT.
% maxPeaks = the number of peaks we are looking for
% fs = sampling frequency in Hz 
% win = window used to obtain Xwdb (assumed zero phase)
% N = NFFT, the number of points used in the FFT creating Xwdb
%-------------------------------------------------------------------------%
%-- Find all peaks (magnitudes and indices) by comparing each point of ---%
%-- magnitude spectrum with its two neighbors ---%
allPeaks = [];
for i=2:length(Xwdb)-1
    if Xwdb(i)>Xwdb(i-1) & Xwdb(i)>Xwdb(i+1)
        allPeaks = [allPeaks Xwdb(i)];
    end
end
%-- Order from largest to smallest magnitude, keep only maxPeaks of them --%
peaks = sort(allPeaks,'descend');
esti_peaks = zeros(1,maxPeaks);
esti_freqs = zeros(1,maxPeaks);
%-- Do parabolic interpolation in dB magnitude to find more accurate peak --%
%-- and frequency estimates --%
for i=1:maxPeaks
    idx=find(Xwdb==peaks(i));
    %parabolic interpolation
    a=Xwdb(idx-1);
    b=Xwdb(idx);
    c=Xwdb(idx+1);
    p = 1/2 * (a-c)/(a-2*b+c);
    esti_peaks(i) = (b - 1/4*(a-c)*p);
    esti_freqs(i) = (idx-1+p)*fs/N; % change from index to freq bin
end
%-- Return linear amplitude and frequency in Hz --%
% NOTE that we must use knowledge of the window to normalize amplitude here
% if we have a TD cosine of amplitude 0.6, this output should be 0.6
peaks = esti_peaks;
freqs = esti_freqs;