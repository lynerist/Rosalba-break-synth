/*
  ==============================================================================

    FilterComponent.h
    Created: 8 Sep 2021 11:25:18pm
    Author:  Leonardo Albani

  ==============================================================================
*/

#pragma once

#include "TwoValueSliderAttachment.h"
#include "../SynthVoice.h"
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
        
        // PER VISUALIZZARE COME INTERI I VALORI DI FREQUENZA DEL FILTRO (Che da filterSlider.getMinValueObject() sono double)

        highpassFreq.onTextChange = [h = &highpassFreq] {
            (*h).getTextValue().setValue(int((*h).getTextValue().getValue()));
        };

        lowpassFreq.onTextChange = [l = &lowpassFreq] {
            (*l).getTextValue().setValue(int((*l).getTextValue().getValue()));
        };

        

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
        filterSlider.setBounds(0 U, 0 U, 14 U, 3 U);

        highpassFreq.setBounds(1 U, 3 U, 2 U, 1 U);
        lowpassFreq.setBounds(11 U, 3 U, 2 U, 1 U);
    }

    void prepareToPlay() {
        highpassFreq.getTextValue().setValue(0);
        lowpassFreq.getTextValue().setValue(3000);
    }

private:
    juce::Slider filterSlider;

    juce::Label highpassFreq;
    juce::Label lowpassFreq;

    TwoValueSliderAttachment sliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
