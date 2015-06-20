#ifndef OSCILLATOR_H_INCLUDED
#define OSCILLATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class Oscillator
{
public:
    //==============================================================================
    Oscillator();
    ~Oscillator();

    //==============================================================================
    void note(int);
    void processBlock(AudioSampleBuffer&, int, int);
    void setSampleRate(int);

private:
    //==============================================================================
    int mSampleRate;
    int mCurrentNote;
    double mCurrentAngle;
};

#endif  // OSCILLATOR_H_INCLUDED
