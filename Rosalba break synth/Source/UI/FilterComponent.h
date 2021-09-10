/*
  ==============================================================================

    FilterComponent.h
    Created: 8 Sep 2021 11:25:18pm
    Author:  Leonardo Albani

  ==============================================================================
*/

#pragma once

#include "TwoValueSliderAttachment.h"
#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts)
        : sliderAttachment(apvts, "HIGHFREQ", "LOWFREQ", filterSlider)
    {

        filterSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
        filterSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);

        highpassFreq.setColour(highpassFreq.textColourId, juce::Colours::black);
        lowpassFreq.setColour(lowpassFreq.textColourId, juce::Colours::black);

        highpassFreq.getTextValue().referTo(filterSlider.getMinValueObject());
        lowpassFreq.getTextValue().referTo(filterSlider.getMaxValueObject());

        // VALORI INIZIALI????
        //highpassFreq.setText(filterSlider.getMinValueObject().getValue(), juce::NotificationType::sendNotification);
        //lowpassFreq.setText(filterSlider.getMaxValueObject().getValue(), juce::NotificationType::sendNotification);

        
        addAndMakeVisible(filterSlider);
        addAndMakeVisible(highpassFreq);
        addAndMakeVisible(lowpassFreq);

    }

    ~FilterComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::forestgreen);

    }

    void resized() override
    {
        const auto bounds = getLocalBounds().reduced(10);

        const auto sliderHeight = bounds.getHeight() - 30;
        const auto boxWidth = 100;
        const auto boxStartY = sliderHeight + 10;

        filterSlider.setBounds(0, 0, bounds.getWidth(), sliderHeight);

        highpassFreq.setBounds(0, boxStartY, boxWidth, 30);
        lowpassFreq.setBounds(bounds.getWidth() - boxWidth, boxStartY, boxWidth, 30);
    }

private:
    juce::Slider filterSlider;

    juce::Label highpassFreq;
    juce::Label lowpassFreq;

    TwoValueSliderAttachment sliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
