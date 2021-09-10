/*
  ==============================================================================

    OscComponent.cpp
    Created: 20 Aug 2021 6:50:06pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorId, juce::String octaveSelectorId)
{
    juce::StringArray choices{ "Sine", "Saw", "Square", "Plus" };
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(oscWaveSelector);

    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, waveSelectorId, oscWaveSelector);

    if (octaveSelectorId != "") {

        juce::StringArray choices{ "-2", "-1", "0", "+1", "+2" }; //shift ottava
        octaveSelector.addItemList(choices, 1);
        addAndMakeVisible(octaveSelector);

        octaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, octaveSelectorId, octaveSelector);
    }
}

OscComponent::~OscComponent()
{
}

void OscComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void OscComponent::resized()
{
    oscWaveSelector.setBounds(0, 0, 90, 20);
    
    octaveSelector.setBounds(0, 30, 90, 20); //se non funziona metti if di controllo
}