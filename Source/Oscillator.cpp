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
        //const double time = mTimer * cyclesPerSecond / mSampleRate;

        // so sawtooth begins at zero
        const double time = 0.25 + mTimer * cyclesPerSecond / mSampleRate;

        // sine wave
        //const float currentSample = (float) (sin(2.0 * double_Pi * time));

        // sawtooth
        //const double fracTime = time - (long)time;
        //const float currentSample = (float) (1-(2.0 * fracTime));

        // square
        // const double fracTime = time - (long)time;
        // float currentSample = 0.0f;
        // if(fracTime < 0.5) {
        //     currentSample = 1.0f;
        // } else {
        //     currentSample = -1.0f;
        // }

        // triangle
        const double fracTime = time - (long)time;
        float currentSample = 0.0f;
        if(fracTime < 0.5) {
            currentSample = (float) (-1.0+4.0*fracTime);
        } else {
            currentSample = (float) (3.0-4.0*fracTime);
        }

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
