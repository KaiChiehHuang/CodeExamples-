# Kai-Chieh(Jay) Huang // Adaptive Signal Processing Final Exam
# Date: 3,18,2015
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sig
import random as rand
import recursiveFilter as recFilter
import plotting as plting

##-----------------------------------------------------------------------##
# This script is used to model the plants using FIR and IIR adaptive     ##
# filters. Use the functions recursiveFilter.py and plotting.py to do    ##
# work of training the filter and plotting the results. When this script ##
# is runned, 3 plots should be generated and a value of percent MSE      ##
# should be calculated for each filter.                                  ##
##-----------------------------------------------------------------------##

# Since the values for mu and mean, variance of the white gaussian noise
# are the same for the two questions we will just declear the variables
# once in the beginning

mu = 0.01
mean = 0
variance = 0.1
noise = np.array([rand.normalvariate(mean,np.sqrt(variance)) for i in range(0,12000)])

##--Question 1--##--------------------------------------------------------##

# Number of feedforward weights for FIR testing
numWFIRForward = 20 # feedforward
numWFIRBack = 0     # feedback
# Number of feedforward and feedback weights for IIR testing
numWIIRForward = 10 # feedforward
numWIIRBack    = 10 # feedback
# Plant feedforward coefficients
plantA = [1,1.3,-0.6]
# Plant feedback coefficients
plantB = [1,-0.9,0.4]
# Generate the desired output from plant
desiredOutput = sig.lfilter(plantA,plantB,noise)
# Calculate the output power
plantPower = (desiredOutput*desiredOutput).sum()/desiredOutput.size
# Run the recursiveFilter function to find the convergence filter coefficients & error
FIR_a, FIR_b, FIR_e = recFilter.recursiveFilter(numWFIRForward,numWFIRBack,noise,desiredOutput,mu)
# Plot the result of the convergence FIR filter compare to the plant
percentMSE = plting.plotting(FIR_a,FIR_b,plantA,plantB,FIR_e,plantPower,'FIR1')
print "percentMSE_FIR = ", percentMSE
# Run the recursiveFilter function to find the convergence filter coefficients & error
IIR_a, IIR_b, IIR_e = recFilter.recursiveFilter(numWIIRForward,numWIIRBack,noise,desiredOutput,mu)
# Plot the result of the convergence IIR filter compare to the plant
percentMSE = plting.plotting(IIR_a,IIR_b,plantA,plantB,IIR_e,plantPower,'IIR1')
print "percentMSE_IIR = ", percentMSE

#----------------------------------------------------------------------------------#

##--Question 2--##----------------------------------------------------------------##

# Number of feedforward weights for FIR testing
numWFIR_1 = 100
numWFIR_2 = 200
numWFIRBack = 0
# Number of feedforward and feedback weights for IIR testing
numWIIRForward = 10 # feedforward
numWIIRBack    = 10 # feedback
# Plant feedforward coefficients
plantA = [1,0.8]
# Plant feedback coefficients
plantB = [1,-0.18,0.23,0.47,-0.41,-0.18]
# Generate the desired output from plant
desiredOutput = sig.lfilter(plantA,plantB,noise)
plantPower = (desiredOutput*desiredOutput).sum()/desiredOutput.size
# Run the recursiveFilter function to find the convergence filter coefficients & error
FIR_a, FIR_b, FIR_e = recFilter.recursiveFilter(numWFIR_1,numWFIRBack,noise,desiredOutput,mu)
# Plot the result of the convergence FIR filter compare to the plant
percentMSE = plting.plotting(FIR_a,FIR_b,plantA,plantB,FIR_e,plantPower,'FIR2')
print "percentMSE_FIR = ", percentMSE
# Run the recursiveFilter function to find the convergence filter coefficients & error
IIR_a, IIR_b, IIR_e = recFilter.recursiveFilter(numWIIRForward,numWIIRBack,noise,desiredOutput,mu)
# Plot the result of the convergence IIR filter compare to the plant
percentMSE = plting.plotting(IIR_a,IIR_b,plantA,plantB,IIR_e,plantPower,'IIR2')
print "percentMSE_IIR = ", percentMSE

#-------------------------------------------------------------------------------------#
