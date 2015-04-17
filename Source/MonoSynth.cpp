#include "MonoSynth.h"

#include "../JuceLibraryCode/JuceHeader.h"
#include <modules/juce_audio_basics/midi/juce_MidiMessage.h>

//==============================================================================
MonoSynth::MonoSynth()
{
    mSampleRate = 0;
    mCurrentAngle = 0;
    mAngleDelta = 0;
    mIsPlaying = false;
}

//==============================================================================
MonoSynth::~MonoSynth()
{
}

//==============================================================================
void MonoSynth::setSampleRate(const double sampleRate)
{
    if (mSampleRate != sampleRate)
    {
        const ScopedLock sl (lock);
        mSampleRate = sampleRate;
    }
}

//==============================================================================
void MonoSynth::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // must set the sample rate before using this!
    jassert (mSampleRate != 0);

    const ScopedLock sl (lock);

    int numSamples = buffer.getNumSamples();
    int startSample = 0;
    MidiBuffer::Iterator midiIterator (midiMessages);
    midiIterator.setNextSamplePosition (0);
    MidiMessage m (0xf4, 0.0);

    while (numSamples > 0)
    {
        int midiEventPos;
        const bool useEvent
            = midiIterator.getNextEvent (m, midiEventPos)
                && midiEventPos < startSample + numSamples;
        const int numThisTime = useEvent ? midiEventPos - startSample : numSamples;

        // render up to the next event (or end of buffer) using current state
        if (numThisTime > 0)
            render(buffer, startSample, numThisTime);

        // update state based on next event (if it exists)
        if (useEvent)
        {
            if (m.isNoteOn())
            {
                const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (m.getNoteNumber());
                const double cyclesPerSample = cyclesPerSecond / mSampleRate;
                mAngleDelta = cyclesPerSample * 2.0 * double_Pi;
                mCurrentAngle = 0.0;
                mIsPlaying = true;
            }
            else if (m.isNoteOff())
            {
                mAngleDelta = 0.0;
                mIsPlaying = false;
            }
        }

        startSample += numThisTime;
        numSamples -= numThisTime;
    }
}

//==============================================================================
void MonoSynth::render(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    if(!mIsPlaying)
        return;

    // idiom for looping over buffer
    while (--numSamples >= 0)
    {
        const float currentSample = (float) (sin(mCurrentAngle));
        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample (i, currentIndex, currentSample);
        mCurrentAngle += mAngleDelta;
        ++currentIndex;
    }
}