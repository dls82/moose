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
    void setEnvelopeState(EnvelopeStates state);

private:
    EnvelopeStates mCurrentState;
    double mSampleRate;
    double mClock;
    int mBlocksSeen;
    double mGain;
    double mOffsetGain;
    double mAttackTau;
    double mDecayTau;
    double mReleaseTau;
    double mSustainGain;
};

#endif  // ENVELOPE_H_INCLUDED
