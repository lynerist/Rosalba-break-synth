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

class SynthVoice : public juce::SynthesiserVoice {

public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved(int newPitchWheelValue) override;
    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release);

private:
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::AudioBuffer<float> synthBuffer;

    //juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); }};                       //sine
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x/juce::MathConstants<float>::pi; }};    //saw tooth     
    juce::dsp::Oscillator<float> osc{ [](float x) { return x<0.0f?-1.0f:1.0f; }};                 //square
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f?(x / juce::MathConstants<float>::pi)+std::sin(x): juce::MathConstants<float>::pi/x-std::cos(x); } };
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f?(x / juce::MathConstants<float>::pi)+std::sin(x): juce::MathConstants<float>::pi/std::sin(x); } };
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f ? (x / juce::MathConstants<float>::pi) + std::sin(x) : juce::MathConstants<float>::pi / (std::sin(x + sin(x))); } };
    
    juce::dsp::Gain<float> gain;

    bool isPrepared { false };

};