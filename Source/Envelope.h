#ifndef ENVELOPE_H_INCLUDED
#define ENVELOPE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <modules/juce_audio_basics/buffers/juce_AudioSampleBuffer.h>

//==============================================================================
/**
*/
class Envelope
{
public:
    //==============================================================================
    Envelope();
    ~Envelope();

    //==============================================================================
    void on();
    void off();
    void processBlock(AudioSampleBuffer&, int, int);

private:
    double mGain;
};

#endif  // ENVELOPE_H_INCLUDED
