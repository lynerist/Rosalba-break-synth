/*
  ==============================================================================

    OscData.cpp
    Created: 20 Aug 2021 5:40:13pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include "OscData.h"

OscData::OscData()
: Oscillator() {
    setOctave(DEFAULTOCTAVE);
}

void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) {

    prepare(spec);
}

void OscData::setWaveType(const int choice) {
    
    switch (choice) {
    case 0: //sine wave
        initialise([](float x) {return std::sin(x); });
        break;
    case 1: //saw tooth
        initialise([](float x) { return x / PI; });
        break;
    case 2: //square
        initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        break;
    case 3: //plus
        initialise([](float x) { return x < 0.0f ? (x / PI) + std::sin(x) : PI / x - std::cos(x); });
        break;
    default:
        jassertfalse; 
        break;
    }
}

void OscData::setWaveFrequency(const int midiNoteNumber) {
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
}

void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {

    process(juce::dsp::ProcessContextReplacing<float>(block));
}


void OscData::setOctave(const int choice) {
    shiftOctave = exp2(choice - 2 -1) ; // 2^(choice-2) = 1/4 * 2^choice (-1 serve a correggere l'ottava ottenendo A4 = 440Hz)
}