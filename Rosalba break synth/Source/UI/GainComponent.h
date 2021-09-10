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
        // GAIN
        gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "GAIN", gainSlider);

        gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
        gainSlider.setColour(gainSlider.textBoxTextColourId, juce::Colours::black);
        addAndMakeVisible(gainSlider);

        // OSC PRESENCE
        presenceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "PRESENCE", presenceSlider);
        
        presenceSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        presenceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);
        addAndMakeVisible(presenceSlider);

        // QUANTIZATION BIT NUMBER
        bitNumberAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "BITNUMBER", bitNumberSlider);

        bitNumberSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        bitNumberSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
        bitNumberSlider.setColour(bitNumberSlider.textBoxTextColourId,juce::Colours::black);
        addAndMakeVisible(bitNumberSlider);

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

        const auto sliderWidth = (bounds.getWidth()-10) / 2;
        const auto sliderHeight = bounds.getHeight() - 100;
        const auto sliderStartX = (bounds.getWidth() + 20) / 2 - (sliderWidth + 5);
        const auto sliderStartY = 0;

        gainSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
        bitNumberSlider.setBounds(sliderStartX + sliderWidth + 10, sliderStartY, sliderWidth, sliderHeight);
        presenceSlider.setBounds(sliderStartX, sliderHeight + sliderStartY + 45, bounds.getWidth() , 30);

    }

private:
    juce::Slider gainSlider;
    juce::Slider presenceSlider;
    juce::Slider bitNumberSlider;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitNumberAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainComponent)
};
