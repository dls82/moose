#include "Envelope.h"

//==============================================================================
Envelope::Envelope()
{
    mGain = 0;
}

//==============================================================================
Envelope::~Envelope()
{
}

//==============================================================================
void Envelope::on()
{
   mGain = 1;
}

//==============================================================================
void Envelope::off()
{
   mGain = 0;
}

//==============================================================================
void Envelope::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    for (int i = buffer.getNumChannels(); --i >= 0;)
        buffer.applyGain(i, currentIndex, numSamples, mGain);
}
