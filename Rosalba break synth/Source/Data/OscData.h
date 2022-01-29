/*
  ==============================================================================

    OscData.h
    Created: 20 Aug 2021 5:40:13pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define DEFAULT_OCTAVE 2
#define DEFAULT_OSC_TYPE 0
#define PI juce::MathConstants<float>::pi

class OscData : public juce::dsp::Oscillator<float>
{    
public:
    OscData(): Oscillator()
    {
        setWaveType(DEFAULT_OSC_TYPE);
        setOctave(DEFAULT_OCTAVE);
    }
    
    void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) { 
        prepare(spec); 
    };    
    
    void OscData::setWave() {
        switch (waveChoice)
        {
        case 0: //sine
            initialise([](float x) {return std::sin(x); });
            break;
        case 1: //saw tooth
            initialise([](float x) { return (x / PI); });
            break;
        case 2: //square
            initialise([](float x) { return (x < 0.0f ? -1.0f : 1.0f); });
            break;
        case 3: //plus
            initialise([](float x) { return (x < 0.0f ? (x / PI) + std::sin(x) : PI / x - std::cos(x)); });
            break;
        default:
            jassertfalse;
            break;
        }
    }

    void OscData::setWaveType(const int choice)
    {
        waveChoice = choice;
        setWave();
    }
    
    void OscData::setWaveFrequency(const int midiNoteNumber) {
        setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
    }

    void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
        process(juce::dsp::ProcessContextReplacing<float>(block));
    }

    /* ==============================================================================
        2^(choice-2) = 1/4 * 2^choice (-1 serve a correggere l'ottava ottenendo A4 = 440Hz)
        i valori di choice vanno da 0 a 4 così si ottiene:
        c = 0 -> shift = 1/4 (-2 ottave)        1/8
        c = 1 -> shift = 1/2 (-1 ottave)        1/4
        c = 2 -> shift = 1   (+0 ottave)        1/2
        c = 3 -> shift = 2   (+1 ottave)        1
        c = 4 -> shift = 4   (+2 ottave)        2
     ============================================================================== */
    void OscData::setOctave(const int choice) {
        shiftOctave = exp2(choice - 2 - 1);
    }

private:
    double shiftOctave;
    int waveChoice;  

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscData)
};