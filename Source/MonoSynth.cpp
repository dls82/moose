#include "MonoSynth.h"

#include <modules/juce_audio_basics/midi/juce_MidiMessage.h>

//==============================================================================
MonoSynth::MonoSynth()
{
    mSampleRate = 0;
    mCurrentAngle = 0;
    mNoteCurrent = 0;
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
        const ScopedLock sl(lock);
        mSampleRate = sampleRate;
        mEnvelope.setSampleRate(sampleRate);
    }
}

void MonoSynth::setAttackParam(double param)
{
    mEnvelope.setAttackParam(param);
}
void MonoSynth::setDecayParam(double param)
{
    mEnvelope.setDecayParam(param);
}
void MonoSynth::setSustainParam(double param)
{
    mEnvelope.setSustainParam(param);
}
void MonoSynth::setReleaseParam(double param)
{
    mEnvelope.setReleaseParam(param);
}

//==============================================================================
void MonoSynth::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // must set the sample rate before using this!
    jassert(mSampleRate != 0);

    const ScopedLock sl(lock);

    int numSamples = buffer.getNumSamples();
    int startSample = 0;
    MidiBuffer::Iterator midiIterator(midiMessages);
    midiIterator.setNextSamplePosition (0);

    while (numSamples > 0)
    {
        int midiEventPos;
        MidiMessage m(0xf4, 0.0);
        const bool useEvent
            = midiIterator.getNextEvent(m, midiEventPos)
                && midiEventPos < startSample + numSamples;
        const int numThisTime = useEvent ? midiEventPos - startSample : numSamples;

        // render up to the next event (or end of buffer) using current state
        if (numThisTime > 0) {
            oscillate(buffer, startSample, numThisTime);
            mEnvelope.processBlock(buffer, startSample, numThisTime);
        }

        // update state based on next event (if it exists)
        if (useEvent)
        {
            if (m.isNoteOn())
            {
                mNoteCurrent = m.getNoteNumber();
                mNoteList.push_back(mNoteCurrent);
                mCurrentAngle = 0.0;
                mEnvelope.on();
            }
            else if (m.isNoteOff())
            {
                mNoteList.remove(m.getNoteNumber());
                if(mNoteList.empty())
                    mEnvelope.off();
                else
                    mNoteCurrent = mNoteList.back();
            }
        }

        startSample += numThisTime;
        numSamples -= numThisTime;
    }
}

//==============================================================================
void MonoSynth::oscillate(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(mNoteCurrent);
    const double cyclesPerSample = cyclesPerSecond / mSampleRate;
    const double angleDelta = cyclesPerSample * 2.0 * double_Pi;

    // idiom for looping over buffer
    while (--numSamples >= 0)
    {
        const float currentSample = (float) (sin(mCurrentAngle));
        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample(i, currentIndex, currentSample);
        mCurrentAngle += angleDelta;
        ++currentIndex;
    }
}
