/*
  ==============================================================================

    OscData.cpp
    Created: 20 Aug 2021 5:40:13pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include "OscData.h"

OscData::OscData() {
    setOctave(DEFAULTOCTAVE);
    Oscillator();
}

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {

    prepare(spec);
}

void OscData::setWaveType(const int choice) {
    
    switch (choice) {
    case 0: //sine wave
        initialise([so=shiftOctave](float x) {return std::sin(x*so); });
        break;
    case 1: //saw tooth
        initialise([so = shiftOctave](float x) { return x*so / PI; });
        break;
    case 2: //square
        initialise([so = shiftOctave](float x) { return x*so < 0.0f ? -1.0f : 1.0f; });
        break;
    case 3: //plus
        initialise([so = shiftOctave](float x) { return x*so < 0.0f ? ((x*so) / PI) + std::sin(x*so) : PI / (x*so) - std::cos(x*so); });
        break;
    default:
        jassertfalse; 
        break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber) {

    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {

    process(juce::dsp::ProcessContextReplacing<float>(block));
}


void OscData::setOctave(const int choice) {

    shiftOctave = (1 / 4) * exp2(choice);
}