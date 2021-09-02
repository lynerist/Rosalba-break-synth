/*
  ==============================================================================

    OscData.h
    Created: 20 Aug 2021 5:40:13pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float> {
    OscData();

public:
    void prepareToPlay(juce::dsp::ProcessSpec& spec);
    void setWaveType(const int choice);
    void setWaveFrequency(const int midiNoteNumber);
    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block);

    void setOctave(const int choice);

private:
    float shiftOctave;
};