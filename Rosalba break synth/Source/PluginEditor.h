
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/GainComponent.h"
#include "UI/FilterComponent.h"
#include "UI/SpectrumAnalyzer.h"
#include "UI/LookAndFeelCustomization.h"

//==============================================================================

class RosalbabreaksynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RosalbabreaksynthAudioProcessorEditor (RosalbabreaksynthAudioProcessor&);
    ~RosalbabreaksynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    RosalbabreaksynthAudioProcessor& audioProcessor;
    OscComponent osc1;
    OscComponent osc2;
    AdsrComponent adsr;
    GainComponent volumeFader;
    FilterComponent filter;

    AnalyzerComponent& analyzer;

    LookAndFeelCustomization lookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RosalbabreaksynthAudioProcessorEditor)
};
