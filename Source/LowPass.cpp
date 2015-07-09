#include "LowPass.h"

#include <cmath>

// http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt

//==============================================================================
LowPass::LowPass(const int numChannels, const int sampleRate,
    const double fc, const double q)
{
    x1.resize(numChannels);
    x2.resize(numChannels);
    y1.resize(numChannels);
    y2.resize(numChannels);

    mSampleRate = sampleRate;
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
LowPass::~LowPass()
{
}

//==============================================================================
void LowPass::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    while(--numSamples >= 0)
    {
        for (int i = buffer.getNumChannels(); --i >= 0;)
        {
            const double x0 = buffer.getSample(i,currentIndex);
            const double y0 = (b0*x0+b1*x1[i]+b2*x2[i]-a1*y1[i]-a2*y2[i])/a0;
            buffer.setSample(i, currentIndex, y0);
            y2[i]=y1[i];
            y1[i]=y0;
            x2[i]=x1[i];
            x1[i]=x0;
        }
        ++currentIndex;
    }
}
