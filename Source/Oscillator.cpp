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
    mCurrentAngle = 0.0;
    mCurrentNote = noteNumber;
}

//==============================================================================
void Oscillator::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    jassert(mSampleRate > 0);
    const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(mCurrentNote);
    const double cyclesPerSample = cyclesPerSecond / mSampleRate;
    const double angleDelta = cyclesPerSample * 2.0 * double_Pi;

    while(--numSamples >= 0)
    {
        const float currentSample = (float) (sin(mCurrentAngle));
        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample(i, currentIndex, currentSample);
        mCurrentAngle += angleDelta;
        ++currentIndex;
    }
}

//==============================================================================
void Oscillator::setSampleRate(const int sampleRate)
{
    if (mSampleRate != sampleRate)
    {
        const ScopedLock sl(lock);
        mSampleRate = sampleRate;
    }
}
