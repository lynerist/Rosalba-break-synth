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
    const auto bounds = getLocalBounds().reduced(10);


    osc1.setBounds(210, 10, 90, 30);
    osc2.setBounds(310, 10, 90, 50);

    adsr.setBounds(bounds.getWidth() / 2, 0, bounds.getWidth() / 2, bounds.getHeight());
    
    volumeFader.setBounds(10, 50, 200, bounds.getHeight()-150);
    
    filter.setBounds(10, bounds.getHeight()-90, bounds.getWidth() / 2, 100);
}
