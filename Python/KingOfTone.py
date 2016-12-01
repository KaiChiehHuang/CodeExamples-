# Kai-Chieh Huang / FinalProject / King Of Tone Distortion Pedal
# Date: May, 19th, 2015
import numpy as np
import matplotlib.pyplot as plt
import scipy.io.wavfile as wav
from   scipy import signal as sig
from audioPlayer import player

##---------------------------------------------------------##
# This program simulates the circuit of the handmade pedal  #
# King Of Tone by characterizing it's analog frequency      #
# response using bilinear transform and model the nonlinear #
# circuit relations by using nodal analysis and current     #
# voltage table.                                            #
##---------------------------------------------------------##

##-------------------------------------##
#   Globle values for Diode modeling    #
##-------------------------------------##

Is = 1./np.power(10, 12)
Vt = 26*0.001

#---Load input signal-----------------------------------------------------------------------#
fs= 44100.
T = 1./fs
player1 = player(fs)
#input = np.sin(2*np.pi*600*np.arange(0,2,T))
input = player1.readFile('fenderTele.wav');
input = input[20000:160000,1]
##-------------------------------------##
# STEP 1: Tone stage circuit modeling   #
##-------------------------------------##

#---Circuit Parameters-----------------------------------------------------------------------#
C3 = 0.01/np.power(10,6)
C4 = 100./np.power(10,12)
C5 = 0.01/np.power(10,6)
C6 = 0.01/np.power(10,6)
R4 = 1.*np.power(10,6)
R6 = 10.*np.power(10,3)
R7 = 33.*np.power(10,3)
R8 = 27.*np.power(10,3)
Rt = 100.*np.power(10,3)        # Adjustable resistance
t = 0.5                         # Initial knob value
#---Filter Parameters-----------------------------------------------------------------------#
Rs = R6 + t*Rt
a4 = Rs*C3*C4*C5*C6*R4*R7*R8
a3 = C3*C5*C6*R4*R7*R8 + Rs*C4*C5*C6*R7*R8 + Rs*C3*C4*C6*R4*R8 + Rs*C3*C4*C5*R4*R7
a2 = C5*C6*R7*R8 + C3*C6*R4*R8 + Rs*C4*C6*R8 + C3*C5*R4*R7 + Rs*C4*C5*R7 + Rs*C3*C4*R4
a1 = C6*R8 + C5*R7 + C3*R4 + Rs*C4
a0 = 1
b4 = a4
b3 = C3*C5*C6*R4*R7*R8 + Rs*C3*C4*C6*R4*R8 + Rs*C3*C4*C5*R4*R7 + Rs*C3*C5*C6*R4*R7 + Rs*C3*C5*C6*R4*R8
b2 = C3*C6*R4*R8 + C3*C5*R4*R7 + Rs*C3*C4*R4 + Rs*C3*C5*R4 + Rs*C3*C6*R4
b1 = C3*R4
b0 = 0
B = [b4,b3,b2,b1,b0]
A = [a4,a3,a2,a1,a0]
#---Frequency response of tone control analog circuit---------------------------------------#
w, h = sig.freqs(B,A)
plt.semilogx(w,abs(h))
plt.title('Magnitude response of first stage analog circuit')
plt.ylabel('Magnitude(linear scale)')
plt.xlabel('Frequency(Hz)')
plt.savefig('MagResAnaCir.png')
plt.show()
#---Frequency response of digitized tone control circuit------------------------------------#
targetFreq = 20540
alpha = targetFreq*2*np.pi/np.tan(targetFreq*np.pi*T)
B0 = alpha**4*b4 + alpha**3*b3 + alpha**2*b2 + alpha*b1
B1 = -4*alpha**4*b4 - 2*alpha**3*b3 + 2*alpha*b1
B2 = 6*alpha**4*b4 - 2*alpha**2*b2
B3 = -4*alpha**4*b4 + 2*alpha**3*b3 - 2*alpha*b1
B4 = alpha**4*b4 - alpha**3*b3 + alpha**2*b2 - alpha*b1
A0 = alpha**4*a4 + alpha**3*a3 + alpha**2*a2 + alpha*a1 + a0
A1 = -4*alpha**4*a4 - 2*alpha**3*a3 + 2*alpha*a1 + 4*a0
A2 = 6*alpha**4*a4 - 2*alpha**2*a2 + 6*a0
A3 = -4*alpha**4*a4 + 2*alpha**3*a3 - 2*alpha*a1 + 4*a0
A4 = alpha**4*a4 - alpha**3*a3 + alpha**2*a2 - alpha*a1 + a0
Bz = [B0,B1,B2,B3,B4]
Az = [A0,A1,A2,A3,A4]
wd, hd = sig.freqz(Bz,Az,1000)
plt.plot(w[0:2350],20.*np.log10(abs(h[0:2350])),'-b',label='Analog')
plt.plot(wd[0:760]/np.pi*fs/2.,20*np.log10(abs(hd[0:760])+0.00001),'-r',label='Digital')
plt.legend(loc='lower right')
plt.title('Magnitude response compare: Analog vs Digital')
plt.ylabel('Magnitude(dB)')
plt.xlabel('Frequency(Hz)')
plt.savefig('MagResCompare.png')
plt.show()
#---Filter input signal---------------------------------------------------------------------#
Vtemp = sig.lfilter(Bz,Az,input);
Vtemp = Vtemp
#-------------------------------------------------------------------------------------------#

##-------------------------------------##
# STEP 2: Nonlinear circuit modeling    #
##-------------------------------------##

#---Circuit Parameters----------------------------------------------------------------------#
R9  = 10.*np.power(10,3)
R10 = 220.*np.power(10,3)
R11 = 6.8*np.power(10,3)
C7  = 0.1/np.power(10,6)
#---Calculate the current going into the op-amp---------------------------------------------#
Vc = 0.
Ic = 0.
Iin = np.zeros(Vtemp.size) # the current going into the op-amp

for i in range(0,Vtemp.size):
    # the R-C path before op-amp
    Vc = (Vtemp[i]/R9 + Ic)/(T/C7+1./R9)
    Iin[i] = (Vtemp[i]-Vc)/R9
    Ic = C7*Vc/T

plt.plot(Iin)
#plt.savefig('Iin.png')
plt.show()
#---Nonelinear distortion-------------------------------------------------------------------#
Vd = 0.01
#Vout = np.zeros(Iin.size)
testLength = 50 #Iin.size
Vout = np.zeros(testLength)
Vds = np.zeros(testLength)
# calculate the Vout through op-amp nonlinear
for i in range(0,testLength):
    for j in range(0,100):
        fx = (R11+R10)*2.*Is*np.sinh(Vd/Vt) + 2.*Vd - R10*Iin[i]
        fxd = (R11+R10)*2.*Is*1./Vt*np.cosh(Vd/Vt) + 2.
        Vd = Vd - fx/fxd
    Vds[i] = Vd
    Vout[i] = (Iin[i] - 2.*Is*np.sinh(Vd/Vt))*R10
    print Vout[i]

player1.writeFile(Vout,'disFender')
plt.plot(Vout)
plt.plot(Vds,'-r')
plt.savefig('Vout.png')
plt.show()



