/*
  ==============================================================================

    SynthVoice.h
    Created: 16 Aug 2021 3:59:58pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "Data/OscData.h"

const auto DEFAULT_GAIN = 0.5f;
const auto DEFAULT_PRESENCE = 0.5f;
const auto MIN_FREQ = 0;
const auto MAX_FREQ = 3000;

const auto DEFAULT_ATTACK = 0.1f;
const auto DEFAULT_DECAY = 0.1f;
const auto DEFAULT_SUSTAIN = 1.0f;
const auto DEFAULT_RELEASE = 0.4f;
const auto MIN_ADSR = 0.1f;
const auto MAX_ADSR = 3.0f;

const auto INTERVAL_VALUE = 0.1f;

// 1 unit = 25 pixel
const int u = 25;
#define U *u


class SynthVoice : public juce::SynthesiserVoice {

public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void update(const float attack, const float decay, const float sustain, const float release, const float gain, const float presence, const int newBitNumber, const int highfreq, const int lowfreq);
    OscData& getOscillator(int id){ return id-1?osc2:osc1; };

private:
    AdsrData adsr;
    juce::AudioBuffer<float> synthBuffer;

    OscData osc1;
    OscData osc2;
    
    juce::dsp::StateVariableTPTFilter<float> highpassFilter;
    juce::dsp::StateVariableTPTFilter<float> lowpassFilter;

    juce::dsp::Gain<float> gain;

    int bitNumber;

    bool isPrepared { false };

};