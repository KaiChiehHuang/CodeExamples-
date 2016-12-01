# -*- coding: utf-8 -*-
# Kai-Chieh(Jay) Huang // Adaptive Signal Processing Final Exam
# Date: 3,18,2015
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sig
import random as rand
from AdaptFilterModule import AdaptFilter

##--------------------------------------------------------------------------##
# This function plots the square error of training, the impulse response of ##
# the filter against the impulse response of the plant, and the output of   ##
# the filter and plant when driven by cos(2*pi/100*t). It returns the       ##
# value percentMSE which is 100 times the ratio of the power of the error   ##
# to the power of the signal after the filter has converged (i.e. the last  ##
# several hundred samples of error).                                        ##
##--------------------------------------------------------------------------##
##---------------------------------------------------------------------------------------##
# Usage:  percentMSE  = plotting(conWA,conWB,plantA,plantB,sErrors,plantPower,filterType)##
# conWA is the converged feedforward weights                                             ##
# conWB is the converged feedback weights                                                ##
# plantA is the feedforward weights of the plant                                         ##
# plantB is the feedback weights of the plant                                            ##
# sErrors is the sequence of squared errors over time calculated from recursiveFilter    ##
# plantPower is the the output power of the plant                                        ##
# filterType is the name of the filter type used in the saved file name                  ##
##---------------------------------------------------------------------------------------##

def plotting(conWA,conWB,plantA,plantB,sErrors,plantPower,filterType):

    # Plot the squared error as a function of time
    plt.plot(sErrors)
    plt.title('Squared Error as a function of time')
    plt.xlabel('Time(samples/iterations)')
    plt.ylabel('Squared Error')
    #plt.savefig(filterType+'_SEF.png')
    plt.show()
    # Plot the impulse response of the filter with the impulse response of the plant
    impulse = np.zeros(300)
    impulse[0] = 1.
    # When using FIR_b as the denominator coefficient it should be [1:FIR_b]
    filterImpulse = sig.lfilter(conWA,np.append([1],-1*conWB),impulse)
    plantImpulse = sig.lfilter(plantA,plantB,impulse)
    plt.plot(filterImpulse,'-b',linewidth = 1.5,label='Filter impulse response')
    plt.plot(plantImpulse,'-r',label='Plant impulse response')
    plt.legend(loc='upper right')
    plt.title('Plant and converged filter impulse response comparison')
    plt.xlabel('Time(samples/iterations)')
    plt.ylabel('Magnitude')
    #plt.savefig(filterType+'_impz.png')
    plt.show()
    # Calculate the percent Mean Square Error. Percent MSE = 100 × ratio of MSE to
    # the output power of the plant. Used the last 200 samples of the squared errors
    # to calculate the estimated average square error (MSE)
    MSE = sErrors[sErrors.size-200:].sum()/200
    percentMSE = (MSE/plantPower)*100
    # Apply the sinusoidal input cos(2πt/100) for t = [1 : 400] to the plant and to the
    # converged adaptive filter and plot the outputs together.
    t = np.arange(1.,401.,1.)
    input = np.cos(2*np.pi*t/100.)
    filterOutput = sig.lfilter(conWA,np.append([1],-1*conWB),input)
    plantOutput = sig.lfilter(plantA,plantB,input)
    plt.plot(filterOutput,'-b',linewidth = 1.5,label='Filter output')
    plt.plot(plantOutput,'-r',label='Plant output')
    plt.legend(loc='upper right')
    plt.title('Output comparison of the plant and converged filter (using cosine as input)')
    plt.xlabel('Time(samples/iterations)')
    plt.ylabel('Magnitude')
    #plt.savefig(filterType+'_cos.png')
    plt.show()
    return percentMSE
