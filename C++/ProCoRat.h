//------------------------------------------------------------------------------
// VST Effect Plug-in
//
// Filename     : ProCoRat.h
// Created by   : music424 staff
// Company      : CCRMA - Stanford University
// Description  :
//
// Date         : 5/4/15
//------------------------------------------------------------------------------

#ifndef __ProCoRat__
#define __ProCoRat__

#include "/Users/yui410122/Desktop/DAFX/Hw5/ProCoRat/Lab5_MAC_VST/vst_sdk/public.sdk/source/vst2.x/audioeffectx.h"
#include <math.h>

#ifndef dB
// if below -100dB, set to -100dB to prevent taking log of zero
#define dB(x)               20.0 * ((x) > 0.00001 ? log10(x) : log10(0.00001))
#endif

#ifndef dB2mag
#define dB2mag(x)           pow( 10.0, (x) / 20.0 )
#endif

#define kMaxLen			32

#define pi 3.14159265358979
#define eps 2.220446049250313e-16


#define kNumBiquads 5

struct ProCoRatDistortion{

    // ProCoRat circuit parameters
    double IC1, IC2, RD, ID, V1, V2, V3;
    double T;
    double C1 = 4.7*pow(10, -6);
    double C2 = 3.3*pow(10, -9);
    double R1 = 1000;
    double RC1;
    double RC2;
    double Is =  pow(10, -12);
    double Vt =  0.026;
    
    //-----------------------------------------------------------------------------
    ProCoRatDistortion(){
        this->IC1=0.0;
        this->IC2=0.0;
        this->RD=0.0;
        this->ID=0.0;
        this->V1=0.0;
        this->V2=0.0;
        this->V3=0.0;
    }
    
    void setParameters(double kUSRatio, double fs){
        this->T = 1./(kUSRatio*fs);
        this->RC1 = T/C1;
        this->RC2 = T/C2;
    }
    //-----------------------------------------------------------------------------
    double calculateRD(double VD){
        
        return Vt/(2*Is*cosh(VD/Vt));
    }
    
    //-----------------------------------------------------------------------------
    double calculateID(double VD){
    
        return 2*Is*(VD/Vt*cosh(VD/Vt)-sinh(VD/Vt));
    }
    
    double process (double input, double R2) {
        
        double Vin = input;
        double k1;
        double k2;
        double k3;
        double k4;
        double RD_R2RC2;
        double RD_R1RC1;
        double output;
        
        // Solve for the convergence V2 by iterations
        for (int i = 0; i<10; i++)
        {
            // Calculate superposition of Vin contribution to V2
            RD_R2RC2 = RD*(R2+RC2)/(RD+R2+RC2);  // the resistance of RD//(R2+RC2)
            k1 = RD_R2RC2/(R1+RC1+RD_R2RC2);
            // Calculate superposition of IC1 contribution to V2
            k2 = -RC1*RD_R2RC2/(RC1+R1+RD_R2RC2);
            // Calculate superposition of IC2 contribution to V2
            RD_R1RC1 = RD*(R1+RC1)/(RD+R1+RC1);  // the resistance of RD//(R1+RC1)
            k3 = RC2*RD_R1RC1/(RC2+R2+RD_R1RC1);
            // Calculate superposition of ID contribution to V2
            k4 = RD_R1RC1*(R2+RC2)/(RD_R1RC1+R2+RC2);
            // Calculate V2 = k1*Vin+k2*IC1+k3*IC2+k4*ID
            V2 = k1*Vin+k2*IC1+k3*IC2+k4*ID;
            // Calculate new RD and ID
            RD = calculateRD(V2);
            ID = calculateID(V2);
        }
        //---------------------------------------------------------------------------------//
        // Calculate V1 and Vout = V3 for calculating new linear components and the output //
        //---------------------------------------------------------------------------------//
        // Calculate superposition of Vin contribution to V3
        RD_R2RC2 = RD*(R2+RC2)/(RD+R2+RC2);  // the resistance of RD//(R2+RC2)
        k1 = RD_R2RC2/(R1+RC1+RD_R2RC2)*RC2/(R2+RC2);
        // Calculate superposition of IC1 contribution to V3
        k2 = -RC1*RD_R2RC2/(RC1+R1+RD_R2RC2)*RC2/(R2+RC2);
        // Calculate superposition of IC2 contribution to V3
        RD_R1RC1 = RD*(R1+RC1)/(RD+R1+RC1);  // the resistance of RD//(R1+RC1)
        k3 = RC2*(R2+RD_R1RC1)/(RC2+R2+RD_R1RC1);
        // Calculate superposition of ID contribution to V3
        k4 = RD_R1RC1*RC2/(RD_R1RC1+R2+RC2);
        // Calculate V3 = k1*Vin+k2*IC1+k3*IC2+k4*ID
        V3 = k1*Vin+k2*IC1+k3*IC2+k4*ID;
        // Calculate superposition of Vin contribution to V1
        k1 = (RC1+RD_R2RC2)/(R1+RC1+RD_R2RC2);
        // Calculate superposition of IC1 contribution to V1
        k2 = RC1*R1/(RC1+R1+RD_R2RC2);
        // Calculate superposition of IC2 contribution to V1
        k3 = RC2*RD_R1RC1/(RC2+R2+RD_R1RC1)*R1/(R1+RC1);
        // Calculate superposition of ID contribution to V1
        k4 = RD_R1RC1*(R2+RC2)/(RD_R1RC1+R2+RC2)*R1/(R1+RC1);
        // Calculate V1 = k1*Vin+k2*IC1+k3*IC2+k4*ID
        V1 = k1*Vin+k2*IC1+k3*IC2+k4*ID;
        // Calculate new linear components for IC1, IC2, RC1, RC2
        IC1 = (V1-V2)*C1/T;
        IC2 = V3*C2/T;
        output = V3;
        return output;
    }

};


//------------------------------------------------------------------------------
// signal processing functions
struct Biquad {
    //  biquad filter section
    double	b0, b1, b2, a1, a2, z1, z2;
    
    Biquad() {
        this->b0=1.0;
        this->b1=0.0;
        this->b2=0.0;
        this->a1=0.0;
        this->a2=0.0;
        reset();
    }
    void setCoefs(double* coefs) {
        // set filter coefficients [b0 b1 b2 a1 a2]
        this->b0=*(coefs);
        this->b1=*(coefs+1);
        this->b2=*(coefs+2);
        this->a1=*(coefs+3);
        this->a2=*(coefs+4);
    }
    void reset() {
        // reset filter state
        z1=0;
        z2=0;
    }
    void process (double input, double& output) {
        // process input sample, direct form II transposed
        output = z1 + input*b0;
        z1 = z2 + input*b1 - output*a1;
        z2 = input*b2 - output*a2;
    }
};


//------------------------------------------------------------------------------
class ProCoRat : public AudioEffectX
{
public:
    ProCoRat (audioMasterCallback audioMaster);
    ~ProCoRat ();
    
    // Processing
    virtual void processReplacing (float** inputs, float** outputs,
                                   VstInt32 sampleFrames);
    
    // Program
    virtual void setProgramName (char* name);
    virtual void getProgramName (char* name);
    
    // Parameters
    virtual void setParameter (VstInt32 index, float value);
    virtual float getParameter (VstInt32 index);
    virtual void getParameterLabel (VstInt32 index, char* label);
    virtual void getParameterDisplay (VstInt32 index, char* text);
    virtual void getParameterName (VstInt32 index, char* text);
    
    void   bilinearTranform(double acoefs[], double scoefs[]);
    void   designShelf(double* scoefs, double Rp);
    double proCoRatDistortion(double input, float R2);
    double calculateRD(double VD);
    double calculateID(double VD);
    
    virtual bool getEffectName (char* name);
    virtual bool getVendorString (char* text);
    virtual bool getProductString (char* text);
    virtual VstInt32 getVendorVersion ();
    
    
protected:
    char	programName[kVstMaxProgNameLen + 1];
    
    // configuration
    enum {
        kNumProgs	= 1,
        kNumInputs	= 2,
        kNumOutputs	= 2
    };
    
    // user interface parameters
    enum {
        kParamDrive,
        kParamLevel,
        kParamTone,
        kNumParams
    };
    
    float DriveKnob, DriveValue;	// sets pedal distortion, ohms
    float LevelKnob, LevelValue;	// output gain, dB   
    float ToneKnob, ToneValue;	    // filter pot, adjusts output tone, ohms
    
    
    // signal processing parameters and state
    double fs;	// sampling rate, Hz
    
    double driveR, level, toneR;	// input, output gains, amplitude
    
    double DriveCoefs[5];	// shelf filter coefficients
    Biquad ShelfFilter;	    // self filter, also controls the drive, via the changing resistor 
    
    enum{kUSRatio = 4};	// upsampling factor, sampling rate ratio
    enum{kAAOrder = kNumBiquads};	// antialiasing/antiimaging filter order, biquads
    Biquad AIFilter[kAAOrder];	// antiimaging filter
    Biquad AAFilter[kAAOrder];	// antialiasing filter

    Biquad dcBlockers[2];       //dc blocking filters
    ProCoRatDistortion distortion;
    
};


// antialiasing/antiimaging filter coefficients
// Chebyshev, 10th order filter
const static double AACoefs[kNumBiquads][5] = {
    {0.0028,    0.0028,         0,       -0.3921,         0,},
    {1.0000,    0.3784,    1.0000,       -0.8773,    0.2335,},
    {1.0000,   -0.8263,    1.0000,       -1.0941,    0.4230,},
    {1.0000,   -1.2553,    1.0000,       -1.3352,    0.6451,},
    {1.0000,   -1.3987,    1.0000,       -1.5595,    0.8747,},
};

// input drive/shelf resistor, resistor value in ohms
const static float DriveLimits[2] = {0.0, 100000.0};
const static float DriveTaper = 1.0;

// output level limits, dB; taper, exponent
const static float LevelLimits[2] = {-48.0, 24.0};
const static float LevelTaper = 1.0;


// output filter control, resistor value in ohms
const static float ToneLimits[2] = {0.0, 100000.0};
const static float ToneTaper = 1.0;


//------------------------------------------------------------------------------
// "static" class to faciliate the knob handling
class SmartKnob {
public:
    // convert knob on [0,1] to value in [limits[0],limits[1]] according to taper
    static float knob2value(float knob, const float *limits, float taper)
    {
        float value;
        if (taper > 0.0) {  // algebraic taper
            value = limits[0] + (limits[1] - limits[0]) * pow(knob, taper);
        } else {            // exponential taper
            value = limits[0] * exp(log(limits[1]/limits[0]) * knob);
        }
        return value;
    };
    
    // convert value in [limits[0],limits[1]] to knob on [0,1] according to taper
    static float value2knob(float value, const float *limits, float taper)
    {
        float knob;
        if (taper > 0.0) {  // algebraic taper
            knob = pow((value - limits[0])/(limits[1] - limits[0]), 1.0/taper);
        } else {            // exponential taper
            knob = log(value/limits[0])/log(limits[1]/limits[0]);
        }
        return knob;
    };
    
};


#endif	// __ProCoRat_HPP


