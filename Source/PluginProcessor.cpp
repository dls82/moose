/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MooseAudioProcessor::MooseAudioProcessor()
{
}

MooseAudioProcessor::~MooseAudioProcessor()
{
}

//==============================================================================
const String MooseAudioProcessor::getName() const
{
    return ProjectInfo::projectName;
}

int MooseAudioProcessor::getNumParameters()
{
    return 0;
}

float MooseAudioProcessor::getParameter (int /*index*/)
{
    return 0.0f;
}

void MooseAudioProcessor::setParameter (int /*index*/, float /*newValue*/)
{
}

const String MooseAudioProcessor::getParameterName (int /*index*/)
{
    return String();
}

const String MooseAudioProcessor::getParameterText (int /*index*/)
{
    return String();
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

void MooseAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String MooseAudioProcessor::getProgramName (int /*index*/)
{
    return String();
}

void MooseAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

//==============================================================================
void MooseAudioProcessor::prepareToPlay (double sampleRate, int /**samplesPerBlock*/)
{
    // Why does JUCE use 'double' for sample rate?
    // Internally, we'll use an integer. Truncation should be fine.
    mMonoSynth.setSampleRate((int)sampleRate);
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
    for (int i = getTotalNumInputChannels(); i < getTotalNumInputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    mMonoSynth.processBlock(buffer, midiMessages);
}

//==============================================================================
bool MooseAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MooseAudioProcessor::createEditor()
{
    return new MooseAudioProcessorEditor (*this);
}

//==============================================================================
void MooseAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MooseAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MooseAudioProcessor();
}
