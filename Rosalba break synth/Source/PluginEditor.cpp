/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RosalbabreaksynthAudioProcessorEditor::RosalbabreaksynthAudioProcessorEditor (RosalbabreaksynthAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), osc1(audioProcessor.apvts, "OSC1WAVETYPE"), 
    osc2(audioProcessor.apvts, "OSC2WAVETYPE", "OSC2OCTAVE"), adsr(audioProcessor.apvts), 
    volumeFader(audioProcessor.apvts), filter(audioProcessor.apvts)
{
    setSize (800, 500);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(adsr);
    addAndMakeVisible(volumeFader);
    addAndMakeVisible(filter);
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
    osc1.setBounds(210, 10, 100, 30);
    osc2.setBounds(310, 10, 100, 50);
    adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
    volumeFader.setBounds(10, 50, 200, getHeight()-110);
    filter.setBounds(0, getHeight()-100, getWidth() / 2, 100);
}
