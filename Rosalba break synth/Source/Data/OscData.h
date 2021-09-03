/*
  ==============================================================================

    OscData.h
    Created: 20 Aug 2021 5:40:13pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define DEFAULTOCTAVE 2
#define PI juce::MathConstants<float>::pi

class OscData : public juce::dsp::Oscillator<float> {
    
public:
    OscData();
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int choice);
    void setWaveFrequency(const int midiNoteNumber);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

    void setOctave(const int choice);

private:
    float shiftOctave;
};