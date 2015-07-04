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
    void setSampleRate(const int);

    virtual void processBlock(AudioSampleBuffer&, int, int) = 0;

protected:
    //==============================================================================
    /** This is used to control access to the rendering callback and the note trigger methods. */
    CriticalSection lock;

    int mSampleRate;
    int mNote;
    int mTimer;
};

class SineOscillator : public Oscillator
{
public:
    //==============================================================================
    void processBlock(AudioSampleBuffer&, int, int) override;
};

class SawOscillator : public Oscillator
{
public:
    //==============================================================================
    void processBlock(AudioSampleBuffer&, int, int) override;
};

class SquareOscillator : public Oscillator
{
public:
    //==============================================================================
    void processBlock(AudioSampleBuffer&, int, int) override;
};

class TriangleOscillator : public Oscillator
{
public:
    //==============================================================================
    void processBlock(AudioSampleBuffer&, int, int) override;
};

#endif  // OSCILLATOR_H_INCLUDED
