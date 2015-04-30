#include "Envelope.h"
// #include <iostream>
// #include <fstream>
// using namespace std;

//==============================================================================
Envelope::Envelope()
{
    mGain = 0;
    mClock = 0;
    mBlocksSeen = 0;
    mCurrentState = idle;
}

//==============================================================================
Envelope::~Envelope()
{
}

//==============================================================================
void Envelope::setSampleRate(const double sampleRate)
{
    if (mSampleRate != sampleRate)
    {
        // const ScopedLock sl(lock);
        mSampleRate = sampleRate;
    }
}

//==============================================================================
void Envelope::setEnvelopeState(EnvelopeStates state)
{
  mClock = 0;
  mBlocksSeen = 0;
  mOffsetGain = mGain;
  mCurrentState = state;
}

//==============================================================================
void Envelope::on()
{
  setEnvelopeState(attack);
}

//==============================================================================
void Envelope::off()
{
  setEnvelopeState(release);
}

//==============================================================================
void Envelope::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
  // ofstream gainFile("/home/andrewrynhard/Desktop/gain.txt", ofstream::out | ofstream::app);
  //  if (gainFile.is_open())
  //  {
  //    gainFile << mGain;
  //    gainFile << ",\n";
  //    gainFile.close();
  //  }
  //  else cout << "Unable to open file";

  for(int channel = buffer.getNumChannels(); --channel >= 0;)
  {
    float* channelData = buffer.getWritePointer(channel);

    for(int i = currentIndex; i < numSamples; i++)
    {
      if (mCurrentState != sustain) {
        mClock = (i + numSamples * mBlocksSeen) / mSampleRate;
      }

      switch (mCurrentState) {
        case idle:
          if (mGain != 0) {
            mGain = 0;
          }
          channelData[i] *= mGain;

          break;
        case attack:
          mGain = 1 - exp(-mClock/(mAttackTau));
          channelData[i] *= mGain;

          if (mClock >= 5 * mAttackTau)
          {
            setEnvelopeState(decay);
          }

          break;
        case decay:
          mGain = mOffsetGain * exp(-mClock/(mDecayTau));
          channelData[i] *= mGain;

          if (mClock >= 5 * mDecayTau || mGain < mSustainGain)
          {
            setEnvelopeState(sustain);
          }

          break;
        case sustain:
          channelData[i] *= mGain;

          break;
        case release:
          mGain = mOffsetGain * exp(-mClock/(mReleaseTau));
          channelData[i] *= mGain;

          if (mClock >= 5 * mReleaseTau || mGain < 0)
          {
            setEnvelopeState(idle);
          }

          break;
      }

      if (i == numSamples - 1) {
        mBlocksSeen += 1;
      }
    }
  }
}
