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
void Oscillator::setSampleRate(const int sampleRate)
{
    jassert(sampleRate > 0);
    if (mSampleRate != sampleRate)
    {
        const ScopedLock sl(lock);
        mSampleRate = sampleRate;
    }
}

//==============================================================================
void SineOscillator::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    jassert(mSampleRate > 0);
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
void SawOscillator::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    jassert(mSampleRate > 0);
    const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(mNote);
    const double cyclesPerSample = cyclesPerSecond / mSampleRate;

    while(--numSamples >= 0)
    {
        // phase offest so sawtooth begins at zero
        const double time = 0.25 + mTimer * cyclesPerSecond / mSampleRate;
        const double fracTime = time - (long)time;
        const float currentSample = (float) (1-(2.0 * fracTime));

        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample(i, currentIndex, currentSample);
        ++mTimer;
        ++currentIndex;
    }
}

//==============================================================================
void SquareOscillator::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    jassert(mSampleRate > 0);
    const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(mNote);
    const double cyclesPerSample = cyclesPerSecond / mSampleRate;

    while(--numSamples >= 0)
    {
        const double time = mTimer * cyclesPerSecond / mSampleRate;
        const double fracTime = time - (long)time;
        const float currentSample = (fracTime < 0.5) ? 1.0f : -1.0f;

        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample(i, currentIndex, currentSample);
        ++mTimer;
        ++currentIndex;
    }
}

//==============================================================================
void TriangleOscillator::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    jassert(mSampleRate > 0);
    const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz(mNote);
    const double cyclesPerSample = cyclesPerSecond / mSampleRate;

    while(--numSamples >= 0)
    {
        const double time = mTimer * cyclesPerSecond / mSampleRate;
        const double fracTime = time - (long)time;
        const float currentSample
            = (fracTime < 0.5) ?
                (float) (-1.0+4.0*fracTime) :
                (float) (3.0-4.0*fracTime);

        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample(i, currentIndex, currentSample);
        ++mTimer;
        ++currentIndex;
    }
}
