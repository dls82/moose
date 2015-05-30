#include "../Catch/catch.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "MonoSynth.h"

TEST_CASE( "basic envelope check", "[monosynth]" ) {
    const int sampleRate = 44100;
    const int startSample = 1000;
    const int endSample = 50000;
    MidiBuffer midi;
    midi.addEvent(MidiMessage::noteOn(1,80,1.0f), startSample);
    midi.addEvent(MidiMessage::noteOff(1,80), endSample);
    AudioSampleBuffer audio(1,endSample+startSample);
    audio.clear();
    MonoSynth synth;
    synth.setSampleRate(sampleRate);
    synth.processBlock(audio, midi);

    // on/off as expected
    REQUIRE(audio.getSample(0,startSample) == 0.0f);
    REQUIRE(audio.getSample(0,startSample+1) != 0.0f);
    REQUIRE(audio.getSample(0,endSample-1) != 0.0f);
    REQUIRE(audio.getSample(0,endSample) == 0.0f);

    // example WAV writing
    File file("/home/derek/Desktop/test.wav");
    FileOutputStream* os = file.createOutputStream();
    WavAudioFormat format;
    AudioFormatWriter* writer = format.createWriterFor(os, sampleRate, 1, 16, NULL, 0);
    writer->writeFromAudioSampleBuffer(audio,0,audio.getNumSamples());
    delete writer;
}
