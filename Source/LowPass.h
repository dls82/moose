#ifndef LOWPASS_H_INCLUDED
#define LOWPASS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class LowPass
{
public:
    //==============================================================================
    LowPass();
    ~LowPass();

    //==============================================================================
    void setSampleRate(const double);
    void setState(const double, const double);
    void processBlock(AudioSampleBuffer&, int, int);

private:
    //==============================================================================
    double a0,a1,a2,b0,b1,b2;
    double y1,y2;
    double mFreqCutoff, mQ;
    double mSampleRate;
};

#endif  // LOWPASS_H_INCLUDED
