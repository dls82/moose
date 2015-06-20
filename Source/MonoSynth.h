#ifndef MONOSYNTH_H_INCLUDED
#define MONOSYNTH_H_INCLUDED

#include <list>

#include "../JuceLibraryCode/JuceHeader.h"

#include "Envelope.h"
#include "Oscillator.h"

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
    void setSampleRate(const double);
    void processBlock(AudioSampleBuffer&, MidiBuffer&);

protected:
    //==============================================================================
    /** This is used to control access to the rendering callback and the note trigger methods. */
    CriticalSection lock;

private:
    //==============================================================================
    int mSampleRate;
    std::list<int> mNoteList;
    Envelope mEnvelope;
    Oscillator mOscillator;
};

#endif  // MONOSYNTH_H_INCLUDED
