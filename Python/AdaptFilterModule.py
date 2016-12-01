## Adaptive Filter Module ##
#Author: Kai-Chieh(Jay) Huang // Date: 2015/02/23
# This Module implements a basic Adaptive filter
# Can choose between LMS or Random Search algorithm
# to perform adaptation 
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sig
import random as rand

class AdaptFilter:
    
    def __init__(self,numWeights,mu):
        self.errors = []
        self.output_y = []
        self.numW = numWeights
        self.mu = mu

    def getSquaredErrors(self):
        return self.errors

    def getWeights(self):
        return self.weights

    def runLMS(self,input,desired_y,numOfIterations):
        self.output_y = np.zeros(numOfIterations)
        self.errors = np.zeros(numOfIterations)
        numOfWeights = self.numW
        curr_W = np.zeros(numOfWeights)
        self.weights = np.zeros((numOfWeights,numOfIterations))
        for num in range(numOfWeights,numOfIterations+numOfWeights):
            curr_X = input[num-numOfWeights+1:num+1]
            curr_X = curr_X[::-1]
            curr_y = (curr_X*curr_W).sum()
            for w in range(0,numOfWeights):
                self.weights[w][num-numOfWeights] = curr_W[w]
            self.output_y[num-numOfWeights] = curr_y
            error = desired_y[num]-curr_y
            self.errors[num-numOfWeights] = error*error
            curr_W = curr_W + 2*self.mu*error*curr_X
        self.finalW = curr_W
        return self.output_y

    def runRandSearch(self,input,desired_y,numOfIterations,var,numObserv):
        self.output_y = np.zeros(numOfIterations)
        self.errors = np.zeros(numOfIterations)
        numOfWeights = self.numW
        curr_W = np.zeros(numOfWeights)
        for num in range(numOfWeights,numOfIterations+numOfWeights):
            curr_X = input[num-numOfWeights+1:num+1]
            curr_X = curr_X[::-1]
            curr_y = (curr_X*curr_W).sum()
            self.output_y[num-numOfWeights] = curr_y
            error = desired_y[num]-curr_y
            self.errors[num-numOfWeights] = error*error
            randVector = np.array([rand.normalvariate(0,np.sqrt(var)) for i in range(numOfWeights)])
            rand_W = curr_W + randVector
            estiCurrWMSE = 0
            estiRandWMSE = 0
        
            test = []
            c = 0
            for N in range(0,numObserv):
                obser_X = input[num-numOfWeights+1+N:num+1+N]
                obser_X = obser_X[::-1]
                obserW_y = np.dot(obser_X, curr_W)
                estiCurrWError = (desired_y[num+N]-obserW_y)
                estiCurrWMSE += estiCurrWError**2
                obserRandW_Y = np.dot(obser_X, rand_W)
                estiRandWError = (desired_y[num+N]-obserRandW_Y)
                estiRandWMSE += estiRandWError**2
        
        
            estiCurrWMSE = estiCurrWMSE/numObserv
            estiRandWMSE = estiRandWMSE/numObserv
            
            curr_W = curr_W + (self.mu/var)*(estiCurrWMSE-estiRandWMSE)*randVector
        self.finalW = curr_W
        return self.output_y

    def plotLearningCurve(self,show=False,fileName="LearningCurve.png"):
        plt.plot(self.errors)
        plt.title("Learning curve")
        plt.ylabel("Squared Error")
        plt.xlabel("Interation number")
        plt.savefig(fileName)
        if (show==True):
            plt.show()

    def plotFreqResponse(self,show=False,fileName="FrequencyResponse.png"):
        w, h = sig.freqz(self.finalW,[1])
        plt.subplot(211)
        plt.plot(w,20*np.log(abs(h)))
        plt.ylabel("Magnitude(dB)")
        plt.title("Frequency Response")
        plt.subplot(212)
        plt.plot(w,np.unwrap(np.angle(h)))
        plt.ylabel("Phase(rad)")
        plt.xlabel("Frequency(rad/s)")
        plt.savefig(fileName)
        if (show==True):
            plt.show()
