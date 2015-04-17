#ifndef MONOSYNTH_H_INCLUDED
#define MONOSYNTH_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <modules/juce_audio_basics/buffers/juce_AudioSampleBuffer.h>
#include <modules/juce_audio_basics/midi/juce_MidiBuffer.h>

//==============================================================================
/**
*/
class MonoSynth
{
public:
    //==============================================================================
    MonoSynth();
    ~MonoSynth();

    //==============================================================================
    void setSampleRate(const double sampleRate);
    void processBlock(AudioSampleBuffer&, MidiBuffer&);

protected:
    //==============================================================================
    /** This is used to control access to the rendering callback and the note trigger methods. */
    CriticalSection lock;

private:
    //==============================================================================
    void render(AudioSampleBuffer&, int, int);

    //==============================================================================
    double mSampleRate;
    double mCurrentAngle;
    double mAngleDelta;
    bool mIsPlaying;
};

#endif  // MONOSYNTH_H_INCLUDED
