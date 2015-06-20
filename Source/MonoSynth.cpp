#include "MonoSynth.h"

//==============================================================================
MonoSynth::MonoSynth()
{
    mSampleRate = 0;
}

//==============================================================================
MonoSynth::~MonoSynth()
{
}

//==============================================================================
void MonoSynth::setSampleRate(const double sampleRate)
{
    // Why does JUCE use 'double' for sample rate?
    // Internally, we'll use an integer.
    if (mSampleRate != (int) sampleRate)
    {
        const ScopedLock sl(lock);
        mSampleRate = (int) sampleRate;
        mOscillator.setSampleRate(mSampleRate);
    }
}

//==============================================================================
void MonoSynth::processBlock(AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    jassert(mSampleRate > 0);

    const ScopedLock sl(lock);

    int numSamples = buffer.getNumSamples();
    int startSample = 0;
    MidiBuffer::Iterator midiIterator(midiMessages);
    midiIterator.setNextSamplePosition(0);

    while(numSamples > 0)
    {
        int midiEventPos;
        MidiMessage m(0xf4, 0.0);
        const bool useEvent
            = midiIterator.getNextEvent(m, midiEventPos)
                && midiEventPos < startSample + numSamples;
        const int numThisTime = useEvent ? midiEventPos - startSample : numSamples;

        // render up to the next event (or end of buffer) using current state
        if(numThisTime > 0) {
            mOscillator.processBlock(buffer, startSample, numThisTime);
            mEnvelope.processBlock(buffer, startSample, numThisTime);
        }

        // update state based on next event (if it exists)
        if(useEvent)
        {
            if(m.isNoteOn())
            {
                const int noteNumber = m.getNoteNumber();
                mOscillator.note(noteNumber);
                mNoteList.push_back(noteNumber);
                mEnvelope.on();
            }
            else if(m.isNoteOff())
            {
                mNoteList.remove(m.getNoteNumber());
                if(mNoteList.empty())
                    mEnvelope.off();
                else
                    mOscillator.note(mNoteList.back());
            }
        }

        startSample += numThisTime;
        numSamples -= numThisTime;
    }
}
