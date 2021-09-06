/*
  ==============================================================================

    gainComponent.h
    Created: 24 Aug 2021 6:14:26pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GainComponent  : public juce::Component
{
public:
    GainComponent(juce::AudioProcessorValueTreeState& apvts)
    {
        gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "GAIN", gainSlider);

        gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
        addAndMakeVisible(gainSlider);

        // OSC PRESENCE
        presenceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "PRESENCE", presenceSlider);
        
        presenceSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        addAndMakeVisible(presenceSlider);
    }

    ~GainComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::lightgoldenrodyellow);
    }

    void resized() override
    {
        const auto bounds = getLocalBounds().reduced(10);
        const auto padding = 10;
        const auto sliderWidth = bounds.getWidth() - padding;
        const auto sliderHeight = bounds.getHeight() - 100;
        const auto sliderStartX = bounds.getWidth() / 2 - padding;
        const auto sliderStartY = 0;

        gainSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);

        presenceSlider.setBounds(0, sliderHeight + sliderStartY + 45, sliderWidth, 30);

    }

private:
    juce::Slider gainSlider;

    juce::Slider presenceSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainComponent)
};
