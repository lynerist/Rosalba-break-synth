/*
  ==============================================================================

    FilterListener.h
    Created: 9 Sep 2021 4:56:43pm
    Author:  Utente

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class FilterListener : public juce::Value::Listener {


public:
    FilterListener(){}

    FilterListener(juce::AudioProcessorValueTreeState& apvts, const String id, juce::TextEditor& tbox) {
        parameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, id, paramSlider);
        juce::TextEditor& textbox = tbox;
    }
    
    void valueChanged(Value &value) override {
        paramSlider.setValue(value.getValue());
        (*textbox).setText(value.getValue());
    }

private:
    juce::Slider paramSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> parameterAttachment;
    std::unique_ptr<juce::TextEditor> textbox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FilterListener)
};