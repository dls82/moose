#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator()
{
}

//==============================================================================
Oscillator::~Oscillator()
{
}

//==============================================================================
void Oscillator::note(int noteNumber)
{
    jassert(mSampleRate > 0);
    mTimer = 0;
    mNote = noteNumber;
}

//==============================================================================
void Oscillator::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    jassert(mSampleRate > 0);
    jassert(mNote > 0);
    const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(mNote);
    const double cyclesPerSample = cyclesPerSecond / mSampleRate;

    while(--numSamples >= 0)
    {
        const double time = mTimer * cyclesPerSecond / mSampleRate;
        const float currentSample = (float) (sin(2.0 * double_Pi * time));
        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample(i, currentIndex, currentSample);
        ++mTimer;
        ++currentIndex;
    }
}

//==============================================================================
void Oscillator::setSampleRate(const int sampleRate)
{
    jassert(sampleRate > 0);
    if (mSampleRate != sampleRate)
    {
        const ScopedLock sl(lock);
        mSampleRate = sampleRate;
    }
}
