//------------------------------------------------------------------------------
// VST Effect Plug-in
//
// Filename     : ProCoRat.cpp
// Created by   : music424 staff
// Company      : CCRMA - Stanford University
// Description  :
//
// Date         : 5/4/15
//------------------------------------------------------------------------------

#include "ProCoRat.h"
#include <stdlib.h>

//------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new ProCoRat (audioMaster);
}

//------------------------------------------------------------------------------
ProCoRat::ProCoRat (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, kNumProgs, kNumParams)	// 1 program, 1 parameter only
{
    setNumInputs (kNumInputs);		// stereo in
    setNumOutputs (kNumOutputs);		// stereo out
    setUniqueID ('Dstn');	// identify
    canProcessReplacing ();	// supports replacing output
    
    
    vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
    
    // user interface parameter initialization
    DriveValue = (float) 1000.0;	// drive resistance in ohms
    DriveKnob = SmartKnob::value2knob(DriveValue, DriveLimits, DriveTaper);
    driveR = DriveValue;
    
    LevelValue = (float) 0.0;	// input gain, dB
    LevelKnob = SmartKnob::value2knob(LevelValue, LevelLimits, LevelTaper);
    level = dB2mag( LevelValue );
    
    ToneValue = (float) 1000.0;	// tone resistance in ohms
    ToneKnob = SmartKnob::value2knob(ToneValue, ToneLimits, ToneTaper);
    toneR = ToneValue;
    
    // signal processing parameter and state initialization
    fs = getSampleRate();	// sampling rate, Hz
    
    //initialize the shelf filter coefficients
    designShelf(DriveCoefs, DriveValue);
    ShelfFilter.setCoefs(DriveCoefs);
    
    //initialize the distortion coefficients
    distortion.setParameters(kUSRatio, fs);
    
    //set the coefficients od our anti imaging and aliasing filters
    for (int j = 0; j < kAAOrder; j++) {
        AIFilter[j].setCoefs((double *) AACoefs[j]);
        AAFilter[j].setCoefs((double *) AACoefs[j]);
    }
    
    //set the coefficients of our dc blocking filters
    double dcBlockCoeffs[5] = {0.9996, 0, -0.9996, 0.0007, -0.9993};
    dcBlockers[0].setCoefs(dcBlockCoeffs);
    dcBlockers[1].setCoefs(dcBlockCoeffs);
}

//------------------------------------------------------------------------------
ProCoRat::~ProCoRat ()
{
    // nothing to do here
}

//------------------------------------------------------------------------------
void ProCoRat::setProgramName (char* name)
{
    vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------------
void ProCoRat::getProgramName (char* name)
{
    vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//------------------------------------------------------------------------------
void ProCoRat::setParameter(VstInt32 index, float value)
{
    
    // assign control and signal processing parameter values
    switch (index)
    {
        case kParamDrive:
            // input gain, dB
            DriveKnob = value;
            // Compute Rp
            if(DriveKnob<=0.5)
               driveR =  0.2*DriveLimits[1]*DriveKnob;
            else
               driveR =  0.1*DriveLimits[1]+1.8*DriveLimits[1]*(DriveKnob-0.5);
            DriveValue = driveR;
            // design new shelf filter with the drive resistance
            designShelf(DriveCoefs, driveR);
            ShelfFilter.setCoefs(DriveCoefs);
            break;
            
        case kParamLevel:
            // output gain, dB
            LevelKnob = value;
            LevelValue = SmartKnob::knob2value(LevelKnob, LevelLimits, LevelTaper);
            
            // input gain, amplitude
            level = dB2mag( LevelValue );
            break;
            
        case kParamTone:
            // tone resistance
            ToneKnob = value;
            ToneValue = SmartKnob::knob2value(ToneKnob, ToneLimits, ToneTaper);
            break;
            
        default :
            break;
    }
    
}


//------------------------------------------------------------------------------
float ProCoRat::getParameter(VstInt32 index)
{
    // return knob position
    switch (index)
    {
        case kParamDrive:
            // drive resistance, ohms
            return DriveKnob;
            break;
            
        case kParamLevel:
            // output gain, dB
            return LevelKnob;
            break;
            
        case kParamTone:
            // tone resistance, ohms
            return ToneKnob;
            break;
            
        default:
            return 0.0;
    }
}

//------------------------------------------------------------------------------
void ProCoRat::getParameterName(VstInt32 index, char *label)
{
    // return knob name
    switch (index)
    {
        case kParamDrive:
            // drive resistance, ohms
            vst_strncpy(label, " Drive ", kVstMaxParamStrLen);
            break;
            
        case kParamLevel:
            // output level, dB
            vst_strncpy(label, " Output Gain ", kVstMaxParamStrLen);
            break;
            
        case kParamTone:
            // tone resistance, ohms
            vst_strncpy(label, " Tone ", kVstMaxParamStrLen);
            break;
            
        default :
            *label = '\0';
            break;
    };
}

//------------------------------------------------------------------------------
void ProCoRat::getParameterDisplay(VstInt32 index, char *text)
{
    // return parameter value as text
    switch (index)
    {
        case kParamDrive:
            // drive resistance, ohms
            float2string(DriveValue, text, kVstMaxParamStrLen);
            break;
            
        case kParamLevel:
            // output level, dB
            float2string(LevelValue, text, kVstMaxParamStrLen);
            break;
            
        case kParamTone:
            // tone resistance, ohms
            float2string(ToneValue, text, kVstMaxParamStrLen);
            break;
                
        default :
            *text = '\0';
            break;
    };
}

//------------------------------------------------------------------------------
void ProCoRat::getParameterLabel(VstInt32 index, char *label)
{
    switch (index)
    {
        case kParamDrive:
            // drive resistance, ohms
            vst_strncpy(label, " ohms ", kVstMaxParamStrLen);
            break;
            
        case kParamLevel:
            // output gain, dB
            vst_strncpy(label, " dB ", kVstMaxParamStrLen);
            break;
            
        case kParamTone:
            // tone resistance, ohms
            vst_strncpy(label, " ohms ", kVstMaxParamStrLen);
            break;
        
        default :
            *label = '\0';
            break;
    };
}

//------------------------------------------------------------------------------
bool ProCoRat::getEffectName (char* name)
{
    vst_strncpy (name, "ProCoRatABC", kVstMaxEffectNameLen);
    return true;
}

//------------------------------------------------------------------------------
bool ProCoRat::getProductString (char* text)
{
    vst_strncpy (text, "ProCoRatABC", kVstMaxProductStrLen);
    return true;
}

//------------------------------------------------------------------------------
bool ProCoRat::getVendorString (char* text)
{
    vst_strncpy (text, "Stanford/CCRMA MUS424", kVstMaxVendorStrLen);
    return true;
}

//------------------------------------------------------------------------------
VstInt32 ProCoRat::getVendorVersion ()
{
    return 1000;
}

//------------------------------------------------------------------------------
void ProCoRat::processReplacing(float **inputs, float **outputs,
                                  VstInt32 sampleFrames)
// overwrite output
{
    
    float*	in0 = inputs[0];
    float*  in1 = inputs[1];
    float*	out0 = outputs[0];
    float*  out1 = outputs[1];
    
    double isignal, fsignal, osignal, usignal, dsignal;
    
    int i, j, k;
    
    for (i = 0; i < sampleFrames; i++)
    {
        // assign input
        double inp0 = (*in0);
        double inp1 = (*in1);
        

        //add noise to signal to prevent denormal
        float noise = ((float)(rand()-16384)) * 6.103515623e-015;
        isignal = inp0 + inp1 + noise;
        fsignal = isignal;
        
        // upsample, apply ProCoRat, downsample
        for (k = 0; k < kUSRatio; k++)
        {
            // upsample (insert zeros) and apply upsampling gain
            usignal = (k == kUSRatio - 1) ? fsignal : 0.0;
            
            // apply antiimaging filter
            for (j = 0; j < kAAOrder; j++) {
                AIFilter[j].process(usignal,usignal);
            }
            
            // APPLY THE SHELF FILTER WHICH CONTROLS THE AMOUNT OF GAIN
            // GOING TO THE NONLINEARITY. REMEMBER TO PERFORM YOUR BLT
            // AT THE UPSAMPLED RATE (4 TIMES THE ORIGINAL IN THIS CASE)
            ShelfFilter.process(usignal,usignal);

            // APPLY PROCORAT NONLINEARITY
            dsignal = distortion.process(usignal,ToneValue);
            
            //remove dc portion of signal
            dcBlockers[0].process(dsignal, dsignal);
            dcBlockers[1].process(dsignal, dsignal);
            
            // apply antialiasing filter
            for (j = 0; j < kAAOrder; j++) {
                AAFilter[j].process(dsignal,dsignal);
            }
            
        }
    
        // apply gain, assign output
        osignal = level*dsignal;
        *out0++ = osignal;
        *out1++ = osignal;
        
        // update input pointers
        in0++;in1++;
    }
}

//------------------------------------------------------------------------------
void ProCoRat::bilinearTranform(double acoefs[], double dcoefs[])
{
    double b0, b1, b2, a0, a1, a2;		//storage for continuous-time filter coefs
    double bz0, bz1, bz2, az0, az1, az2;	// coefs for discrete-time filter.
    
    // For easier looking code...unpack
    b0 = acoefs[0]; b1 = acoefs[1]; b2 = acoefs[2];
    a0 = acoefs[3]; a1 = acoefs[4]; a2 = acoefs[5];

    // Perform bilinear transform in the up sampling rate
    double T = 1./(kUSRatio*fs);
    double alpha = 2/T;
    double commen = a2+a1*alpha+a0*alpha*alpha;
    //set up the coefficients for passing unchanged signal
    bz0 = (b2+b1*alpha+b0*alpha*alpha)/commen;
    bz1 = 2*(b2-b0*alpha*alpha)/commen;
    bz2 = (b0*alpha*alpha-b1*alpha+b2)/commen;
    az1 = 2*(a2-a0*alpha*alpha)/commen;
    az2 = (a0*alpha*alpha-a1*alpha+a2)/commen;
    
    // return coefficients to the output
    dcoefs[0] = bz0; dcoefs[1] = bz1; dcoefs[2] = bz2;
    dcoefs[3] = az1; dcoefs[4] = az2;
}


//------------------------------------------------------------------------------
void ProCoRat::designShelf(double* scoefs, double Rp)
// design the input shelf filter based on the variable resistance
{
    double b0, b1, b2, a0, a1, a2;		//storage for continuous-time filter coefs
    double acoefs[6];
    
    //Design the analog coefficients of the shelf filter. These will be based
    //on the resistances, capacitances, and the value of the variable resistor
    //Rp from problem 2.
    double R11 = 560;
    double R12 = 47;
    double C11 = 4.7*pow(10,-6);
    double C12 = 2.2*pow(10,-6);
    double gain = 0.09;
    
    b0 = (R11*R12+R11*Rp+R12*Rp)*C11*C12;
    b1 = (R11+Rp)*C11+(R12+Rp)*C12;
    b2 = 1;
    a0 = R12*R12*C11*C12;
    a1 = R11*C11+R12*C12;
    a2 = 1;
    
    acoefs[0] = gain*b0; acoefs[1] = gain*b1; acoefs[2] = gain*b2;
    acoefs[3] = a0; acoefs[4] = a1; acoefs[5] = a2;
    
    // inputs the 6 analog coeffs, output the 5 digital coeffs
    bilinearTranform(acoefs, scoefs);
    
}

