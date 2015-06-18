#ifndef LOWPASS_H_INCLUDED
#define LOWPASS_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include <vector>

class LowPass
{
public:
    //==============================================================================
    LowPass(const int, const double, const double, const double);
    ~LowPass();

    //==============================================================================
    void processBlock(AudioSampleBuffer&, int, int);

private:
    //==============================================================================
    double a0,a1,a2,b0,b1,b2;
    std::vector<double> x1,x2,y1,y2;
    double mFreqCutoff, mQ;
    double mSampleRate;
};

#endif  // LOWPASS_H_INCLUDED
