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

    struct Parameters
    {
      double attackParam;
      double decayParam;
      double sustainParam;
      double releaseParam;
    } mParameters;

    //==============================================================================
    void on();
    void off();
    void processBlock(AudioSampleBuffer&, int, int);
    void setSampleRate(const double sampleRate);
    void setEnvelopeState(EnvelopeStates state);
    void setAttackParam(double);
    void setDecayParam(double);
    void setSustainParam(double);
    void setReleaseParam(double);

private:
    EnvelopeStates mCurrentState;
    double mSampleRate;
    double mClock;
    int mBlocksSeen;
    double mGain;
    double mOffsetGain;
};

#endif  // ENVELOPE_H_INCLUDED
