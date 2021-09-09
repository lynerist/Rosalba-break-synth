/*
  ==============================================================================

    FilterComponent.h
    Created: 8 Sep 2021 11:25:18pm
    Author:  Utente

  ==============================================================================
*/

#pragma once

#include "FilterListener.h"
#include "TwoValueSliderAttachment.h"
#include <JuceHeader.h>

//==============================================================================
/*
*/
class FilterComponent  : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& apvts)
        //: highFreqListener(apvts, "HIGHFREQ", highpassFreq), lowFreqListener(apvts, "LOWFREQ", lowpassFreq)
        : sliderAttachment(apvts, "HIGHFREQ", "LOWFREQ", filterSlider)
    {



        filterSlider.setSliderStyle(juce::Slider::SliderStyle::TwoValueHorizontal);
        filterSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0, 0);

        

        highpassFreq.setColour(highpassFreq.textColourId, juce::Colours::black);
        lowpassFreq.setColour(lowpassFreq.textColourId, juce::Colours::black);

        highpassFreq.attachToComponent(&filterSlider, false);

        //filterSlider.getMinValueObject().addListener(&highFreqListener);
        //filterSlider.getMinValueObject().addListener(&lowFreqListener);
        
        

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
        const auto boxwidth = 100;

        filterSlider.setBounds(0, 0, bounds.getWidth(), sliderHeight);
        highpassFreq.setBounds(0, sliderHeight + 10, boxwidth, 30);
        lowpassFreq.setBounds(boxwidth + 10, sliderHeight + 10, boxwidth, 30);
    }

private:
    juce::Slider filterSlider;

    juce::Label lowpassFreq;
    juce::Label highpassFreq;

    //FilterListener highFreqListener;
    //FilterListener lowFreqListener;
    TwoValueSliderAttachment sliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
