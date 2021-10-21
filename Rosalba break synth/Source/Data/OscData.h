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
        setOctave(DEFAULT_OCTAVE);
        setWaveType(DEFAULT_OSC_TYPE);
    }
    
    void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) { 
        prepare(spec); 
    };    
    
    /* ==============================================================================
        p è una variabile complementare fra i due oscillatori, la somma delle due è sempre 1
        serve a definire la presenza di uno rispetto all'altro.
    ============================================================================== */
    void OscData::setWaveType(const int choice) {   
        switch (choice) 
        {
        case 0: //sine wave
            initialise([p = presence](float x) {return std::sin(x) * p; });
            waveChoice = 0;
            break;
        case 1: //saw tooth
            initialise([p = presence](float x) { return (x / PI) * p; });
            waveChoice = 1;
            break;
        case 2: //square
            initialise([p = presence](float x) { return (x < 0.0f ? -1.0f : 1.0f) * p; });
            waveChoice = 2;
            break;
        case 3: //plus
            initialise([p = presence](float x) { return (x < 0.0f ? (x / PI) + std::sin(x) : PI / x - std::cos(x)) * p; });
            waveChoice = 3;
            break;
        default:
            jassertfalse;
            break;
        }
    }
    
    void OscData::setWaveFrequency(const int midiNoteNumber) {
        setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
        DBG("setWaveFrequency: " << juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
    }

    void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
        process(juce::dsp::ProcessContextReplacing<float>(block));  
    }


    /* ==============================================================================
        2^(choice-2) = 1/4 * 2^choice (-1 serve a correggere l'ottava ottenendo A4 = 440Hz)
        i valori di choice vanno da 0 a 4 così si ottiene:
        c = 0 -> shift = 1/4 (-2 ottave)
        c = 1 -> shift = 1/2 (-1 ottave)
        c = 2 -> shift = 1   (+0 ottave)
        c = 3 -> shift = 2   (+1 ottave)
        c = 4 -> shift = 4   (+2 ottave)
     ============================================================================== */
    void OscData::setOctave(const int choice) {
        shiftOctave = exp2(choice - 2 - 1);
    }
    
    void OscData::setPresence(float value) {
        presence = value;
        setWaveType(waveChoice);
    }

    double OscData::getOctave() {
        return shiftOctave;
    }

private:
    double shiftOctave;
    float  presence;
    int waveChoice;
};