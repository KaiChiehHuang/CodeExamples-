# Kai-Chieh(Jay) Huang // Adaptive Signal Processing Final Exam
# Date: 3,18,2015
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal as sig

##-------------------------------------------------------------------------##
# This function implements an adaptive recursive filter with L feedforward ##
# weights, K feedback weights, an input signal of X, a desired response    ##
# signal of D and a learning step of mu. It returns the feedforward weight ##
# vector a, the feedback weight vector b, and the error signal e.          ##
##-------------------------------------------------------------------------##
# Usage:  a, b, e  = recursiveFilter( L, K, X, D, mu )                     ##
# L is the number of feedforward weights                                   ##
# K is the number of feedback weights.                                     ##
# X is the sequence of inputs.                                             ##
# D is the sequence of plant outputs, which are also the desired responses ##
##-------------------------------------------------------------------------##

def recursiveFilter(L,K,X,D,mu):

    numOfIterations = 10000
    feedforwardWeighs_A = np.zeros(L)
    feedbackWeights_B =   np.zeros(K)
    squaredErrors = np.zeros(numOfIterations)
    # For storing the previous output value to use in recursive filter
    output_y = np.zeros(numOfIterations*1.2)
    # Iterate through the learning process
    for num in range(L,numOfIterations+L):
        # Find the current input vetor
        curr_X = X[num-L+1:num+1]
        # Flip the input vector to correspond to the correct weight value
        curr_X = curr_X[::-1]
        # Set the recursive contribution to the output_y as 0
        recursiveSum = 0
        # Find the current recursive input as past output_y
        curr_Y = output_y[num-K:num]
        # Flip the recursive input vector to correspond to the correct weight value
        curr_Y = curr_Y[::-1]
        # Calculate the recursive contribution if there's feedback weights
        if (K > 0):
            recursiveSum = (feedbackWeights_B*curr_Y).sum()
        # Calculate the current output_y
        output_y[num] = (feedforwardWeighs_A*curr_X).sum() + recursiveSum
        # Calculate the error
        error = D[num] - output_y[num]
        squaredErrors[num-L] = error**2
        # Updates the weights
        feedforwardWeighs_A = feedforwardWeighs_A + 2*mu*error*curr_X
        # Update the recursive weights if there's feedback weights
        if (K > 0):
            feedbackWeights_B = feedbackWeights_B + 2*mu*error*curr_Y

    return feedforwardWeighs_A, feedbackWeights_B, squaredErrors