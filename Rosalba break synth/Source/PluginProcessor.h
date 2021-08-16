/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthVoice.h"
#include "SynthSound.h"

//==============================================================================
/**
*/
class RosalbabreaksynthAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    RosalbabreaksynthAudioProcessor();
    ~RosalbabreaksynthAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::Synthesiser synth;
    //juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); }};                       //sine
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x/juce::MathConstants<float>::pi; }};    //saw tooth     
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x<0.0f?-1.0f:1.0f; }};                 //square
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f?(x / juce::MathConstants<float>::pi)+std::sin(x): juce::MathConstants<float>::pi/x-std::cos(x); } };
    //juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f?(x / juce::MathConstants<float>::pi)+std::sin(x): juce::MathConstants<float>::pi/std::sin(x); } };
    juce::dsp::Oscillator<float> osc{ [](float x) { return x < 0.0f?(x / juce::MathConstants<float>::pi)+std::sin(x): juce::MathConstants<float>::pi/(std::sin(x+sin(x))); } };

    juce::dsp::Gain<float> gain;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RosalbabreaksynthAudioProcessor)
};
