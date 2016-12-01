%% This script simulates the circuits of ProCoRat distortion pedal %% 
%%-----------------------------------------------------------------%%
% Simulates the frequency response of the simplified ProCoRat       %
% circuit by finding the s plane transfer function of the circuit   %
% and use backward difference mapping to find the discrete time     %
% transfer function and plot it's frequency response.               %
%%-----------------------------------------------------------------%%

% Solve the analog circuits transfer funciton and plot the frequency response.
R1 = 1000;
R2 = 10000;
C1 = 4.7*10^-6;
C2 = 3.3*10^-9;
R3 = 10000;

b1 = R3/C2;
a2 = R1*R2+R1*R3+R2*R3;
a1 = (R1+R3)/C2+(R2+R3)/C1;
a0 = 1/(C1*C2);

% Digitize the analog circuits above with backward difference:
% s --> (1-z^-1)/T and compare the frequency response.
T = 1/44100.;
bz0 =  b1*T;
bz1 =  -b1*T;
bz2 = 0;
az0 = a2 + a1*T + a0*T^2;
az1 = -(2*a2+a1*T);
az2 = a2;

Bz = [bz0,bz1,bz2];
Az = [az0,az1,az2];

figure(1);
[h,t] = impz(Bz,Az);
stem(h(1:200));
title('Impulse response of analytic transfer function');
xlabel('Time(samples)');
ylabel('Amplitude');

% Nodal Analysis Time Simulation
A = [1/R1+C1/T -C1/T 0;-C1/T C1/T+1/R3+1/R2 -1/R2;0 -1/R2 1/R2+C2/T];
b = [-1/R1; 0; 0];
impulse = zeros(1000);

for i=1:1000
   v = -inv(A)*b; 
   impulse(i) = [0 0 1]*v;
   b = [-C1/T*(v(1)-v(2)); C1/T*(v(1)-v(2)); -v(3)*C2/T]; 
end

figure(2);
stem(impulse(1:200));
title('Impulse response of nodal analysis time simulation');
xlabel('Time(samples)');
ylabel('Amplitude');

% State-Space Time Simulation
R2_R3 = R2*R3/(R2+R3);
R1_R3 = R1*R3/(R1+R3);
A = [1+T/C1*(1/(R1+R2_R3)) T/C1*(1/(R2+R1_R3))*R3/(R1+R3); T/C2*(1/(R1+R2_R3))*R3/(R2+R3) 1+T/C2*(1/(R2+R1_R3))];
invA = inv(A);    
BT = [T/C1*(1/(R1+R2_R3)); T/C2*(1/(R1+R2_R3))*R3/(R2+R3)];
u = 1;
v = [0;0];
impulse = zeros(1000);
for i=1:1000
   v = invA*v + invA*BT*u;
   u = 0;
   impulse(i) = [0 1]*v;
end
figure(3);
stem(impulse(1:200));
title('Impulse response of state-space time simulation');
xlabel('Time(samples)');
ylabel('Amplitude');

