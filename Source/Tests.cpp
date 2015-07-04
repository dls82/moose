#include "../Catch/catch.hpp"
#include "../JuceLibraryCode/JuceHeader.h"

#include "MonoSynth.h"
#include "LowPass.h"
#include "Oscillator.h"

#include <cmath>
#include <iostream>

static const double ERR_TOL = 0.000001;

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

// http://dsp.stackexchange.com/questions/20221/question-regarding-filter-implementation-audio-eq-cookbook
TEST_CASE( "impulse response", "[lowpass]" ) {
    // two channel buffer, each impulse shifted by one sample
    AudioSampleBuffer audio(2,50);
    audio.clear();
    audio.addSample(0, 0, 1);
    audio.addSample(1, 1, 1);

    LowPass lowpass(2,48000,5355,1);

    lowpass.processBlock(audio,0,10);
    lowpass.processBlock(audio,10,10);
    lowpass.processBlock(audio,20,10);
    lowpass.processBlock(audio,30,10);
    lowpass.processBlock(audio,40,10);
    REQUIRE(std::abs(audio.getSample(0,1)-0.281314433) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(0,10)+0.0022121917) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(0,24)-0.000159396572) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(1,2)-0.281314433) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(1,11)+0.0022121917) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(1,25)-0.000159396572) < ERR_TOL);
}

TEST_CASE( "sine oscillator", "[sine]" ) {
    AudioSampleBuffer audio(1,50);
    audio.clear();

    SineOscillator oscillator;
    oscillator.setSampleRate(48000);
    oscillator.note(80);
    oscillator.processBlock(audio,0,10);
    oscillator.processBlock(audio,10,10);
    oscillator.processBlock(audio,20,10);
    oscillator.processBlock(audio,30,10);
    oscillator.processBlock(audio,40,10);
    REQUIRE(std::abs(audio.getSample(0,0)) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(0,1)-0.108512424) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(0,2)-0.215743333) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(0,3)-0.320426375) < ERR_TOL);
    REQUIRE(std::abs(audio.getSample(0,4)-0.421325207) < ERR_TOL);
}
