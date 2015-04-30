#ifndef ENVELOPE_H_INCLUDED
#define ENVELOPE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <modules/juce_audio_basics/buffers/juce_AudioSampleBuffer.h>

//==============================================================================
/**
*/
class Envelope
{
  using EnvelopeStates = enum  {
    attack,
    decay,
    sustain,
    release,
    idle
  };

public:
    //==============================================================================
    Envelope();
    ~Envelope();

    //==============================================================================
    void on();
    void off();
    void processBlock(AudioSampleBuffer&, int, int);
    void setSampleRate(const double sampleRate);
    void setState(EnvelopeStates state);

private:
    double mSampleRate;
    double mGain;
    double mOffsetGain;
    EnvelopeStates mCurrentState;
    int mBlocksSeen;
    double mClock;
    double mAttackTau = 3.0;
    double mDecayTau = 2.0;
    double mReleaseTau = 1.0;
    double mSustainGain = 0.25;
};

#endif  // ENVELOPE_H_INCLUDED
