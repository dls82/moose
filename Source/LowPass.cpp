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
    while(--numSamples >= 0)
    {
        for(int i = buffer.getNumChannels(); --i >= 0;)
        {
            const double x0 = buffer.getSample(i,currentIndex);
			const double y0 = (b0*x0+b1*x1+b2*x2-a1*y1-a2*y2)/a0;
            buffer.addSample(i, currentIndex, y0);
			y2=y1;
			y1=y0;
			x2=x1;
			x1=x0;
        }
        ++currentIndex;
    }
}

