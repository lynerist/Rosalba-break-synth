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
    volumeFader(audioProcessor.apvts), filter(audioProcessor.apvts), analyzer(p.analyzer)
{
    setSize (36 U, 23 U);
    addAndMakeVisible(osc1);
    addAndMakeVisible(osc2);
    addAndMakeVisible(volumeFader);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    filter.prepareToPlay();

    addAndMakeVisible(analyzer);
}

RosalbabreaksynthAudioProcessorEditor::~RosalbabreaksynthAudioProcessorEditor()
{
}

//==============================================================================
void RosalbabreaksynthAudioProcessorEditor::paint (juce::Graphics& g)
{

    g.fillAll(juce::Colours::black);

    LookAndFeel::setDefaultLookAndFeel(&LandF);
    g.setColour(juce::Colour(0xFFB4D989));
    g.setFont(0.7 U);
    g.drawText("Waveform A", 20 U, 5 U, 4 U, 2 U, juce::Justification::centred);
    g.drawText("Waveform B", 25 U, 5 U, 4 U, 2 U, juce::Justification::centred);
    g.drawText("Shift Octave", 30 U, 5 U, 4 U, 2 U, juce::Justification::centred);

    g.setColour(juce::Colours::whitesmoke);
    g.setFont(2 U);
    g.drawText("ROSALBA BREAK SYNTH", 4 U, 1 U, 28 U, 3 U, juce::Justification::centred);

}

void RosalbabreaksynthAudioProcessorEditor::resized()
{
    volumeFader.setBounds(2 U, 5 U, 17 U, 17 U);
    
    adsr.setBounds(7 U, 5 U, 12 U, 7 U);

    osc1.setBounds(20 U, 7 U, 4 U, 2 U);
    osc2.setBounds(25 U, 7 U, 9 U, 2 U);

    filter.setBounds(20 U, 18 U, 14 U, 4 U);

    analyzer.setBounds(20 U, 10 U, 14 U, 7 U);
}
