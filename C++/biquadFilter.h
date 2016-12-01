//
//  biquadFilter.h
//  Synth Wah
//
//  Created by jay on 13/7/27.
//  Copyright (c) 2013年 jay. All rights reserved.
//

#ifndef __Synth_Wah__biquadFilter__
#define __Synth_Wah__biquadFilter__ 1

#include <iostream>

 /* defined(__Synth_Wah__biquadFilter__) */

class FiltBiQuad{
    
public:
    
    void  setHP(float dBgain, float f0, float Q);
    void  setLP(float dBgain, float f0, float Q);
    void  setPK(float dBgain, float f0, float Q);

    void  process(float* in1,float* out1,int sampleFrames);
    float processBySample(float in1);

private:
   
    float a[3],b[3];
    const double pi = 3.1415926;
    float input;
    float z1 = 0;
    float z2 = 0;
    float temp = 0;
    
};

#endif