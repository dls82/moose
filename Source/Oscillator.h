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
    void setSampleRate(const int);

protected:
    //==============================================================================
    /** This is used to control access to the rendering callback and the note trigger methods. */
    CriticalSection lock;

private:
    //==============================================================================
    int mSampleRate;
    int mCurrentNote;
    double mCurrentAngle;
};

#endif  // OSCILLATOR_H_INCLUDED
