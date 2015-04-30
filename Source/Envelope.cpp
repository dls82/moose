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
void Envelope::on()
{
  setState(attack);
}

//==============================================================================
void Envelope::off()
{
  setState(release);
}

//==============================================================================
void Envelope::setState(EnvelopeStates state)
{
  mClock = 0;
  mBlocksSeen = 0;
  mOffsetGain = mGain;
  mCurrentState = state;
}

//==============================================================================
void Envelope::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
  // ofstream myfile("/home/andrewrynhard/Desktop/gain.txt", ofstream::out | ofstream::app);
  //  if (myfile.is_open())
  //  {
  //    myfile << mGain;
  //    myfile << ",\n";
  //    myfile.close();
  //  }
  //  else cout << "Unable to open file";

  for(int channel = buffer.getNumChannels(); --channel >= 0;)
  {
    float* channelData = buffer.getWritePointer(channel);

    for(int i = currentIndex; i < numSamples; i++)
    {
      if (mCurrentState != sustain) {
        mClock = (i + numSamples * mBlocksSeen) / 44100.0;
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
            // printf("ATTACK: Gain: %f\tState: %d\n", mGain, mCurrentState);
            setState(decay);
          }

          break;
        case decay:
          mGain = mOffsetGain * exp(-mClock/(mDecayTau));
          channelData[i] *= mGain;

          if (mClock >= 5 * mDecayTau || mGain < mSustainGain)
          {
            printf("DECAY: Gain: %f\tState: %d\n", mGain, mCurrentState);
            setState(sustain);
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
            // printf("RELEASE: Gain: %f\tState: %d\n", mGain, mCurrentState);
            setState(idle);
          }

          break;
      }

      if (i == numSamples - 1) {
        mBlocksSeen += 1;
      }
    }
  }
}
