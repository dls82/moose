/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <modules/juce_audio_basics/midi/juce_MidiBuffer.h>
#include <modules/juce_audio_basics/midi/juce_MidiMessage.h>


//==============================================================================
MooseAudioProcessor::MooseAudioProcessor()
{
    mSampleRate = 0;
    mCurrentAngle = 0;
    mAngleDelta = 0;
    mIsPlaying = false;
}

MooseAudioProcessor::~MooseAudioProcessor()
{
}

//==============================================================================
const String MooseAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int MooseAudioProcessor::getNumParameters()
{
    return 0;
}

float MooseAudioProcessor::getParameter (int index)
{
    return 0.0f;
}

void MooseAudioProcessor::setParameter (int index, float newValue)
{
}

const String MooseAudioProcessor::getParameterName (int index)
{
    return String();
}

const String MooseAudioProcessor::getParameterText (int index)
{
    return String();
}

const String MooseAudioProcessor::getInputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

const String MooseAudioProcessor::getOutputChannelName (int channelIndex) const
{
    return String (channelIndex + 1);
}

bool MooseAudioProcessor::isInputChannelStereoPair (int index) const
{
    return true;
}

bool MooseAudioProcessor::isOutputChannelStereoPair (int index) const
{
    return true;
}

bool MooseAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MooseAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MooseAudioProcessor::silenceInProducesSilenceOut() const
{
    return false;
}

double MooseAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MooseAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MooseAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MooseAudioProcessor::setCurrentProgram (int index)
{
}

const String MooseAudioProcessor::getProgramName (int index)
{
    return String();
}

void MooseAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MooseAudioProcessor::prepareToPlay (double sampleRate, int /**samplesPerBlock*/)
{
    if (mSampleRate != sampleRate)
    {
        const ScopedLock sl (lock);
        mSampleRate = sampleRate;
    }
}

void MooseAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void MooseAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // I've added this to avoid people getting screaming feedback
    // when they first compile the plugin, but obviously you don't need to
    // this code if your algorithm already fills all the output channels.
    for (int i = getNumInputChannels(); i < getNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // must set the sample rate before using this!
    jassert (mSampleRate != 0);
    
    const ScopedLock sl (lock);
    
    int numSamples = buffer.getNumSamples();
    int startSample = 0;
    MidiBuffer::Iterator midiIterator (midiMessages);
    midiIterator.setNextSamplePosition (0);
    MidiMessage m (0xf4, 0.0);
    
    while (numSamples > 0)
    {
        int midiEventPos;
        const bool useEvent
            = midiIterator.getNextEvent (m, midiEventPos)
                && midiEventPos < startSample + numSamples;
        const int numThisTime = useEvent ? midiEventPos - startSample : numSamples;

        // render up to the next event (or end of buffer) using current state
        if (numThisTime > 0)
            render(buffer, startSample, numThisTime);
        
        // update state based on next event (if it exists)
        if (useEvent)
        {
            if (m.isNoteOn())
            {
                const double cyclesPerSecond = MidiMessage::getMidiNoteInHertz (m.getNoteNumber());
                const double cyclesPerSample = cyclesPerSecond / getSampleRate();
                mAngleDelta = cyclesPerSample * 2.0 * double_Pi;
                mCurrentAngle = 0.0;
                mIsPlaying = true;
            }
            else if (m.isNoteOff())
            {
                mAngleDelta = 0.0;
                mIsPlaying = false;
            }
        }

        startSample += numThisTime;
        numSamples -= numThisTime;
    }
}

void MooseAudioProcessor::render(AudioSampleBuffer& buffer, int currentIndex, int numSamples)
{
    if(!mIsPlaying)
        return;
    
    // idiom for looping over buffer
    while (--numSamples >= 0)
    {
        const float currentSample = (float) (sin(mCurrentAngle));
        for (int i = buffer.getNumChannels(); --i >= 0;)
            buffer.addSample (i, currentIndex, currentSample);
        mCurrentAngle += mAngleDelta;
        ++currentIndex;
    }
}

//==============================================================================
bool MooseAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MooseAudioProcessor::createEditor()
{
    return new MooseAudioProcessorEditor (*this);
}

//==============================================================================
void MooseAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MooseAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MooseAudioProcessor();
}