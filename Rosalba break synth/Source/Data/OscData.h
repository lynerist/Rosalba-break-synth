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
    }
    
    void OscData::prepareToPlay(juce::dsp::ProcessSpec& spec) { 
        prepare(spec); 
    };    
    
    /* ==============================================================================
        p è una variabile complementare fra i due oscillatori, la somma delle due è sempre 1
        serve a definire la presenza di uno rispetto all'altro.
    ============================================================================== */
    //void OscData::setWave() {
    //    switch (waveChoice)
    //    {
    //    case 0: //sine wave
    //        DBG("pronto polizia");
    //        initialise([p = presence](float x) {return std::sin(x) * p; });
    //        break;
    //    case 1: //saw tooth
    //        initialise([p = presence](float x) { return (x / PI) * p; });
    //        break;
    //    case 2: //square
    //        initialise([p = presence](float x) { return (x < 0.0f ? -1.0f : 1.0f) * p; });
    //        break;
    //    case 3: //plus
    //        initialise([p = presence](float x) { return (x < 0.0f ? (x / PI) + std::sin(x) : PI / x - std::cos(x)) * p; });
    //        break;
    //    default:
    //        jassertfalse;
    //        break;
    //    }
    //}

    void OscData::setWave() {
        switch (waveChoice)
        {
        case 0: //sine wave
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
        DBG("setWaveFrequency: " << juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
    }

    void OscData::getNextAudioBlock(juce::dsp::AudioBlock<float>& block) {
        DBG("getNextAudioBlock samples : " << block.getNumSamples());
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
        DBG("shiftOctave ");
    }

    //debug
    double OscData::getOctave() {
        return shiftOctave;
    }

    /*float OscData::getPresence()
    {
        return presence;
    }*/
    //fine debug

private:
    double shiftOctave;
    int waveChoice;  

    bool setWaveAvailable = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscData)
};


//class OscData
//{
//public:
//	OscData()
//	{
//		//frequency.setCurrentAndTargetValue(DEFAULT_RATE);
//		setOctave(DEFAULT_OCTAVE);
//	}
//
//	~OscData() {};
//
//	void prepareToPlay(double sr)
//	{
//		sampleRate = sr;
//		//frequency.reset(sr, RATE_SMTH);
//	}
//
//	void getNextAudioBlock(AudioBuffer<float>& buffer, const int numSamples)
//	{
//		const auto numChannels = buffer.getNumChannels();
//		auto** bufferData = buffer.getArrayOfWritePointers();
//
//		for (int smp = 0; smp < numSamples; ++smp)
//		{
//			const float sampleValue = getNextAudioSample();
//
//			for (int ch = 0; ch < numChannels; ++ch) 
//			{
//				bufferData[ch][smp] = sampleValue;
//			}
//		}
//	}
//
//	float getNextAudioSample()
//	{
//		float sampleValue;
//
//		switch (waveChoice)
//		{
//		case 0: // Sine
//			sampleValue = sin(2.0f * float_Pi * normalizedPhaseValue);
//			break;
//		case 1: // Sawtooth crescente(per abbassare il pitch)
//			sampleValue = 2.0f * normalizedPhaseValue - 1.0f;
//			break;
//		case 2: // Square
//			sampleValue = 0.0f;	//provvisorio
//			break;
//		case 3: // Plus
//			sampleValue = 0.0f;	//provvisorio
//			break;
//		default:
//			sampleValue = 0.0f;
//		}
//
//		const float phaseIncrement = frequency.getNextValue() / sampleRate;
//
//		normalizedPhaseValue += phaseIncrement; // + delta su uno dei canali se stereo
//		normalizedPhaseValue -= static_cast<int>(normalizedPhaseValue);
//
//		return sampleValue;
//	}
//
//	void OscData::setWaveFrequency(const int midiNoteNumber) {
//		frequency.setCurrentAndTargetValue(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
//		DBG("setWaveFrequency: " << juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) * (double)shiftOctave);
//	}
//
//	/*void OscData::setPresence(float value) {
//		if (presence != value) {
//		    presence = value;
//		    DBG("presence " << presence);
//		}
//	}*/
//
//	void OscData::setWaveType(const int choice)
//	{
//		waveChoice = choice;
//	}
//
//	void OscData::setOctave(const int choice) {
//		shiftOctave = exp2(choice - 2 - 1);
//		DBG("shiftOctave ");
//	}
//
//	//debug
//    double OscData::getOctave() {
//        return shiftOctave;
//    }
//
//    float OscData::getPresence()
//    {
//        return presence;
//    }
//
//	SmoothedValue<float, ValueSmoothingTypes::Multiplicative> OscData::getFrequency()
//	{
//		return frequency;
//	}
//    //fine debug
//
//private:
//	int waveChoice;
//	double sampleRate;
//	float normalizedPhaseValue = 0.0f;
//	SmoothedValue<float, ValueSmoothingTypes::Multiplicative> frequency;
//	double shiftOctave;
//
//	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OscData)
//};