/*
  ==============================================================================

    gainComponent.h
    Created: 24 Aug 2021 6:14:26pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "LookAndFeelCustomization.h"

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
        presenceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 0 U, 0 U);
        addAndMakeVisible(presenceSlider);
        

        // QUANTIZATION BIT NUMBER
        bitNumberAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "BITNUMBER", bitNumberSlider);

        bitNumberSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        bitNumberSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 60, 25);
        bitNumberSlider.setColour(bitNumberSlider.textBoxTextColourId,juce::Colours::black);
        addAndMakeVisible(bitNumberSlider);

    }

    ~GainComponent() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        //g.fillAll(juce::Colour(0xF0E5B8A1));
        g.fillAll(juce::Colours::black);
        
        g.setColour(juce::Colour(0xF0E5B8A1));
        g.setFont(0.7 U);
        g.drawText("A", Rectangle<int>(0 U, 14 U, 1 U, 2 U), juce::Justification::centred);
        //g.setColour(juce::Colour(0xFFB4D989));
        g.drawText("B", Rectangle<int>(16 U, 14 U, 1 U, 2 U), juce::Justification::centred);

        gainSlider.setColour(gainSlider.textBoxTextColourId, juce::Colours::whitesmoke);
        bitNumberSlider.setColour(bitNumberSlider.textBoxTextColourId, juce::Colours::whitesmoke);

        LandF.setColour(Slider::thumbColourId, juce::Colour(0xF0E5B8A1));
        LandF.setColour(Slider::trackColourId, juce::Colour(0x80E5B8A1));
        LandF.setColour(Slider::textBoxOutlineColourId, juce::Colour(0xF0B88546));
        gainSlider.setLookAndFeel(&LandF);
        bitNumberSlider.setLookAndFeel(&LandF);

        presenceSlider.setColour(presenceSlider.thumbColourId, juce::Colour(0xF0E5B8A1));
    }

    void resized() override
    {       
        gainSlider.setBounds(0 U, 0 U, 4 U, 12 U);
        bitNumberSlider.setBounds(5 U, 8 U, 12 U, 4 U);
        presenceSlider.setBounds(1 U, 13 U, 15 U, 4 U);

    }

private:
    juce::Slider gainSlider;
    juce::Slider presenceSlider;
    juce::Slider bitNumberSlider;

    //LookAndFeelCustomization LandF;
    LookAndFeel_V4 LandF;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> presenceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> bitNumberAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainComponent)
};
