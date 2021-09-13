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
    g.fillAll(juce::Colours::grey);

    g.drawText("A", Rectangle<int>(1 U, 6 U, 2 U, 1 U), juce::Justification::centred);
    g.drawText("D", Rectangle<int>((3+2/3) U, 6 U, 2 U, 1 U), juce::Justification::centred);
    g.drawText("S", Rectangle<int>((6+1/3) U, 6 U, 2 U, 1 U), juce::Justification::centred);
    g.drawText("R", Rectangle<int>(9 U, 6 U, 2 U, 1 U), juce::Justification::centred);

}

void AdsrComponent::resized()
{
    const auto bounds = getLocalBounds().reduced(10);
    const auto sliderWidth = 2 U;
    const auto padding = 1 U / 2;
    const auto sliderHeight = 6 U;
    const auto sliderStartX = 1 U;
    const auto sliderStartY = 0 U;

    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
}

void AdsrComponent::setSliderParams(juce::Slider& slider) {

    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);
}
