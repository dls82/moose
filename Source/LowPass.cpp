#include "LowPass.h"

#include <cmath>

// http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

//==============================================================================
LowPass::LowPass()
{
}

//==============================================================================
LowPass::~LowPass()
{
}

//==============================================================================
void LowPass::setSampleRate(const double sampleRate)
{
    mSampleRate = sampleRate;
    setState(mFreqCutoff, mQ);
}

//==============================================================================
void LowPass::setState(const double fc, const double q)
{
    mFreqCutoff = fc;
    mQ = q;

    const double w0 = 2*M_PI*mFreqCutoff/mSampleRate;
    const double cosw0 = cos(w0);
    const double sinw0 = sin(w0);
    const double alpha = sin(w0)/(2*mQ);

    a0 = 1 + alpha;
    a1 = -2*cosw0;
    a2 = 1 - alpha;
    b0 = (1-cosw0)/2;
    b1 = 1-cosw0;
    b2 = (1-cosw0)/2;
}

//==============================================================================
void LowPass::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    // what if... buffer length <= 2? or if setState() called while in processBlock()?

    // idiom for looping over buffer
    while (--numSamples >= 0)
    {
        for (int i = buffer.getNumChannels(); --i >= 0;)
        {
            // do stuff
            const double y0 = 0;
            audio.getSample(0,startSample)
            buffer.addSample(i, currentIndex, y0);
        }
        ++currentIndex;
    }
}

