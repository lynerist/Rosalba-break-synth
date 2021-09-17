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
    LookAndFeel::setDefaultLookAndFeel(&LandF);
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
    g.setFont(0.4);

    oscWaveSelector.setJustificationType(juce::Justification::centred);

    oscWaveSelector.setColour(oscWaveSelector.backgroundColourId, juce::Colours::black);
    oscWaveSelector.setColour(oscWaveSelector.outlineColourId, juce::Colour(0xF0B88546));
    oscWaveSelector.setColour(oscWaveSelector.arrowColourId, juce::Colour(0xF0B88546));

    octaveSelector.setJustificationType(juce::Justification::centred);

    octaveSelector.setColour(octaveSelector.backgroundColourId, juce::Colours::black);
    octaveSelector.setColour(octaveSelector.outlineColourId, juce::Colour(0xF0B88546));
    octaveSelector.setColour(octaveSelector.arrowColourId, juce::Colour(0xF0B88546));

    oscWaveSelector.setLookAndFeel(&LandF);
    octaveSelector.setLookAndFeel(&LandF);
}

void OscComponent::resized()
{
    oscWaveSelector.setBounds(0 U, 0 U, 4 U, 2 U);
    octaveSelector.setBounds(5 U, 0 U, 4 U, 2 U); 
}
