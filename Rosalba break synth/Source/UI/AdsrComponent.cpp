/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 19 Aug 2021 6:24:10pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(juce::AudioProcessorValueTreeState& apvts)
{
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "RELEASE", releaseSlider);

    setSliderParams(attackSlider);
    setSliderParams(decaySlider);
    setSliderParams(sustainSlider);
    setSliderParams(releaseSlider);
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    //g.fillAll(juce::Colour(0xFFB4D989));
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colour(0xFFB4D989));
    g.setFont(0.7 U);
    g.drawText("A", Rectangle<int>(0.5 U , 6 U, 2 U, 1 U), juce::Justification::centred);
    g.drawText("D", Rectangle<int>(3.5 U, 6 U, 2 U, 1 U), juce::Justification::centred);
    g.drawText("S", Rectangle<int>(6.5 U, 6 U, 2 U, 1 U), juce::Justification::centred);
    g.drawText("R", Rectangle<int>(9.5 U, 6 U, 2 U, 1 U), juce::Justification::centred);

    LandF.setColour(Slider::thumbColourId, juce::Colour(0xFFB4D989));
    LandF.setColour(Slider::trackColourId, juce::Colour(0x80B4D989));
    LandF.setColour(Slider::textBoxOutlineColourId, juce::Colour(0xF0B88546));
    attackSlider.setLookAndFeel(&LandF);
    decaySlider.setLookAndFeel(&LandF);
    sustainSlider.setLookAndFeel(&LandF);
    releaseSlider.setLookAndFeel(&LandF);
}

void AdsrComponent::resized()
{
    const auto padding = 1 U ;
    const auto sliderStartY = 0 U;
    const auto sliderWidth = 2 U;
    const auto sliderHeight = 6 U;

    attackSlider.setBounds(1 U / 2, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
}

void AdsrComponent::setSliderParams(juce::Slider& slider) {

    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
