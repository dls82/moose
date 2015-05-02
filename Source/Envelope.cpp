#include "Envelope.h"
#include <iostream>
#include <fstream>
using namespace std;

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
inline double approxExp(double x) {
  x = 1.0 + x / 1024;
  x *= x; x *= x; x *= x; x *= x;
  x *= x; x *= x; x *= x; x *= x;
  x *= x; x *= x;
  return x;
}

inline double fast_exp(double y) {
  double d;
  *((int*)(&d) + 0) = 0;
  *((int*)(&d) + 1) = (int)(1512775 * y + 1072632447);
  return d;
}

void Envelope::setAttackParam(double param)
{
  mParameters.attackParam = param;
}
void Envelope::setDecayParam(double param)
{
  mParameters.decayParam = param;
}
void Envelope::setSustainParam(double param)
{
  mParameters.sustainParam = param;
}
void Envelope::setReleaseParam(double param)
{
  mParameters.releaseParam = param;
}

bool recordData = true;
//==============================================================================
void Envelope::processBlock(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
  for(int channel = buffer.getNumChannels(); --channel >= 0;)
  {
    float* channelData = buffer.getWritePointer(channel);

    for(int i = currentIndex; i < numSamples; i++)
    {
      mClock = (i + numSamples * mBlocksSeen) / mSampleRate;
      if (i == numSamples - 1) {
        mBlocksSeen += 1;
      }

      if (mGain != 0 && recordData) {
        ofstream gainFile("/home/andrewrynhard/Desktop/data.txt", ofstream::out | ofstream::app);
        if (gainFile.is_open())
        {
            gainFile << mGain ;
            gainFile << "\t";
            gainFile << channelData[i];
            gainFile << "\n";
            gainFile.close();
        }
        else
        {
          cout << "Unable to open file";
        }
      }

      switch (mCurrentState) {
        case idle:
          if (mGain != 0) {
            mGain = 0;
          }

          channelData[i] *= mGain;

          break;
        case attack:
          mGain = 1 - exp(-mClock/mParameters.attackParam);

          if (mClock >= 5 * mParameters.attackParam)
          {
            setEnvelopeState(decay);
          }
          else
          {
            channelData[i] *= mGain;
          }

          break;
        case decay:
          mGain = (mOffsetGain - mParameters.sustainParam) * exp(-mClock/mParameters.decayParam) + mParameters.sustainParam;

          if (mClock >= 5 * mParameters.decayParam || mGain < mParameters.sustainParam)
          {
            setEnvelopeState(sustain);
          }
          else
          {
            channelData[i] *= mGain;
          }

          break;
        case sustain:
          channelData[i] *= mGain;

          break;
        case release:
          mGain = mOffsetGain * exp(-mClock/mParameters.releaseParam);

          if (mGain < 0)
          {
            setEnvelopeState(idle);
          }
          else
          {
            channelData[i] *= mGain;
          }

          break;
      }
    }
  }
}
