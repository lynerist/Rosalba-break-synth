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
        

    /* ==============================================================================
        Per visualizzare come interi i valori di taglio del filtro
        (Che da filterSlider.getMinValueObject().getValue() sono double)
    ============================================================================== */
        highpassFreq.onTextChange = [h = &highpassFreq] {
            (*h).getTextValue().setValue(int((*h).getTextValue().getValue()));
        };
        lowpassFreq.onTextChange = [l = &lowpassFreq] {
            (*l).getTextValue().setValue(int((*l).getTextValue().getValue()));
        };
        
        addAndMakeVisible(filterSlider);
        addAndMakeVisible(highpassFreq);
        addAndMakeVisible(lowpassFreq);

    }

    ~FilterComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll(juce::Colour(0xE0E5B8A1)); 

        filterSlider.setColour(filterSlider.thumbColourId, juce::Colour(0xFFB88546));
        filterSlider.setColour(filterSlider.trackColourId, juce::Colour(0xC094B969));
    }

    void resized() override
    {
        filterSlider.setBounds(0 U, 0 U, 14 U, 3 U);

        highpassFreq.setBounds(1 U, 3 U, 2 U, 1 U);
        lowpassFreq.setBounds(11 U, 3 U, 2 U, 1 U);
    }

    /* ==============================================================================
        Altrimenti i valori delle frequenze di taglio all'inizio non sono visualizzati
     ============================================================================== */
    void prepareToPlay() {
        highpassFreq.getTextValue().setValue(MIN_FREQ);
        lowpassFreq.getTextValue().setValue(MAX_FREQ);
    }

private:
    juce::Slider filterSlider;

 /* ==============================================================================
    Le due label servono a visualizzare i valori del twoValueSlider
 ============================================================================== */
    juce::Label highpassFreq;
    juce::Label lowpassFreq;

    TwoValueSliderAttachment sliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
