/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RosalbabreaksynthAudioProcessorEditor::RosalbabreaksynthAudioProcessorEditor (RosalbabreaksynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc(audioProcessor.apvts, "OSC1WAVETYPE"), adsr(audioProcessor.apvts), volumeFader(audioProcessor.apvts)
{
    setSize (400, 300);
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(volumeFader);
}

RosalbabreaksynthAudioProcessorEditor::~RosalbabreaksynthAudioProcessorEditor()
{
}

//==============================================================================
void RosalbabreaksynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void RosalbabreaksynthAudioProcessorEditor::resized()
{
    osc.setBounds(10, 10, 100, 30);
    adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
    volumeFader.setBounds(10, 50, 100, getHeight()-50);
}
