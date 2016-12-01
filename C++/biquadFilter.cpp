//
//  biquadFilter.cpp
//  Synth Wah
//
//  Created by jay on 13/7/27.
//  Copyright (c) 2013年 jay. All rights reserved.
//

#include "biquadFilter.h"
#include "math.h"


void FiltBiQuad:: setLP(float dBgain, float f0, float Q)
{
    float A  = pow(10,dBgain/40);
    float w0 = 2*pi*f0/44100;
    float alpha = sin(w0)/(2*Q);
    
    a[0] =  1.0f;
    a[1] =  (-2*cos(w0))/(1 + alpha);
    a[2] =  (1 - alpha)/(1 + alpha);
    b[0] =  A*(1 - cos(w0))/2/(1 + alpha);
    b[1] =  A*(1 - cos(w0))/(1 + alpha);
    b[2] =  A*(1 - cos(w0))/2/(1 + alpha);
}
void  FiltBiQuad::setHP(float dBgain, float f0, float Q)
{
    float A  = pow(10,dBgain/40);
    float w0 = 2*pi*f0/44100;
    float alpha = sin(w0)/(2*Q);
    
    b[0] =  A*((1+cos(w0))/2.0)/(1+alpha);
    b[1] =	A*(-1*(1+cos(w0)))/(1+alpha);
    b[2] =	A*((1+cos(w0))/2.0)/(1+alpha);
    a[0] =	1.0;
    a[1] =	(-2*cos(w0))/(1+alpha);
    a[2] =	(1-alpha)/(1+alpha);
}

void FiltBiQuad::setPK(float dBgain, float f0, float Q)
{
    float A  = pow(10,dBgain/40);
    float w0 = 2*pi*f0/44100;
    float alpha = sin(w0)/(2*Q);
    
    b[0] =  2/(A*2) * (1 + alpha*A)/(1 + alpha/A);
    b[1] =  2/(A*2) * (-2*cos(w0))/(1 + alpha/A);
    b[2] =  2/(A*2) * (1 - alpha*A)/(1 + alpha/A);
    a[0] =  1.0f;
    a[1] =  (-2*cos(w0))/(1 + alpha/A);
    a[2] =  (1 - alpha/A)/(1 + alpha/A);
}


void FiltBiQuad:: process(float* in1,float* out1,int sampleFrames)
{
    // Process the filter using transposed direct form II
    for (int i=0; i<sampleFrames; i++)
    {
        input = in1[i];
        temp = z1+input * b[0];
        z1 = z2 + input * b[1] - temp * a[1];
        z2 = input * b[2] - temp * a[2];
        out1[i] = temp;
    }
}

float FiltBiQuad::processBySample(float in1)
{
    float out;
    // Process the filter using transposed direct form II
    input = in1;
    temp = z1+input * b[0];
    z1 = z2 + input * b[1] - temp * a[1];
    z2 = input * b[2] - temp * a[2];
    out = temp;
    
    return out;
}
