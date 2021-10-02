/*
  ==============================================================================

    OscData.h
    Created: 20 Aug 2021 5:40:13pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

#define DEFAULTOCTAVE 2
#define PI juce::MathConstants<float>::pi

class OscData : public juce::dsp::Oscillator<float>
{    
public:
    OscData(): Oscillator()
    {
        setOctave(DEFAULTOCTAVE);
    }
    
    void prepareToPlay(juce::dsp::ProcessSpec& spec) { prepare(spec); };    //check se devi aggiungere override
    
    /* ==============================================================================
        p è una variabile complementare fra i due oscillatori, la somma delle due è sempre 1
        serve a definire la presenza di uno rispetto all'altro.
    ============================================================================== */
    void setWaveType(const int choice) {
        switch (choice) {
        case 0: //sine wave
            initialise([p = presence](float x) {return std::sin(x) * p; });
            break;
        case 1: //saw tooth
            initialise([p = presence](float x) { return (x / PI) * p; });
            break;
        case 2: //square
            initialise([p = presence](float x) { return (x < 0.0f ? -1.0f : 1.0f) * p; });
            break;
        case 3: //plus
            initialise([p = presence](float x) { return (x < 0.0f ? (x / PI) + std::sin(x) : PI / x - std::cos(x)) * p; });
            break;
        default:
            jassertfalse;
            break;
        }
    }
    
    void setWaveFrequency(const int midiNoteNumber) {
        setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
    }

    void getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
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
    void setOctave(const int choice) {
        shiftOctave = exp2(choice - 2 - 1);
    }
    
    void setPresence(float value) {
        presence = value;
    }

private:
    double shiftOctave;
    float  presence;
};