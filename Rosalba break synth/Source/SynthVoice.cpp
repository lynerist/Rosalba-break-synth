/*
  ==============================================================================

    SynthVoice.cpp
    Created: 16 Aug 2021 3:59:58pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include "SynthVoice.h"
#include <iostream>

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {

    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) 
{
    osc1.setWaveFrequency(midiNoteNumber);
    osc2.setWaveFrequency(midiNoteNumber);

    adsr.noteOn();
};

void SynthVoice::stopNote(float velocity, bool allowTailOff) {

    adsr.noteOff();

    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
};

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
};

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
}

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) 
{
    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    adsr.updateADSRParam(DEFAULT_ATTACK, 0);
    adsr.updateADSRParam(DEFAULT_DECAY, 1);
    adsr.updateADSRParam(DEFAULT_SUSTAIN, 2);
    adsr.updateADSRParam(DEFAULT_RELEASE, 3);

    osc1.prepareToPlay(spec);
    osc2.prepareToPlay(spec);
    
    presence1 = presence2 = presenceOld1 = presenceOld2 = sqrt(0.5f);

    osc1.setWaveType(DEFAULT_OSC_TYPE);
    osc2.setWaveType(DEFAULT_OSC_TYPE);
    osc2.setOctave(DEFAULT_OCTAVE);
    
    gain.prepare(spec);
    gain.setRampDurationSeconds(0.1);
    gain.setGainLinear(Decibels::decibelsToGain(DEFAULT_GAIN));
    //smoothedGain = currentGain = DEFAULT_GAIN;
    
    bitNumber = DEFAULT_BITNUMBER;

    highpassFilter.prepare(spec);
    lowpassFilter.prepare(spec);

    highpassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highpassFilter.setCutoffFrequency(MIN_FREQ);
    lowpassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    lowpassFilter.setCutoffFrequency(MAX_FREQ);

    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {

    jassert(isPrepared);

    if (! isVoiceActive())
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    synthBuffer2.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer2.clear();
    
    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    juce::dsp::AudioBlock<float> audioBlock2{ synthBuffer2 };
    
    osc1.getNextAudioBlock(audioBlock);
    osc2.getNextAudioBlock(audioBlock2);

    //presence
    if (applySmooth)
    {
        synthBuffer.applyGainRamp(0, synthBuffer.getNumSamples(), presenceOld1, presence1);
    
        for (int ch = synthBuffer.getNumChannels(); --ch >= 0;)
        {
            synthBuffer.addFromWithRamp(ch, 0, synthBuffer2.getReadPointer(ch), synthBuffer.getNumSamples(), presenceOld2, presence2);
        }

        applySmooth = false;
    }
    else 
    {
        synthBuffer.applyGain(presence1);

        for (int ch = synthBuffer.getNumChannels(); --ch >= 0;)
            synthBuffer.addFrom(ch, 0, synthBuffer2, ch, 0, synthBuffer.getNumSamples(), presence2);
    }
    

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    float quantizationSteps = exp2(bitNumber);

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {

        for (int sample = 0; sample < synthBuffer.getNumSamples(); sample++) {
            //Quantization noise distortion
            float processedSample = round((synthBuffer.getSample(channel, sample)* quantizationSteps))/ quantizationSteps;

            //Filter application
            processedSample = highpassFilter.processSample(channel, processedSample);
            processedSample = lowpassFilter.processSample(channel, processedSample);

            //Gain 
            processedSample = gain.processSample(processedSample);

            synthBuffer.setSample(channel, sample, processedSample);
        }

        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (! adsr.isActive())
            clearCurrentNote();
    }


    /*auto* channelData = synthBuffer.getArrayOfWritePointers();
    
    for (int sample = 0; sample < synthBuffer.getNumSamples(); ++sample)
    {
        smoothedGain += 0.003f * (currentGain - smoothedGain);
        for (int channel = 0; channel < synthBuffer.getNumChannels(); ++channel)
        {
            channelData[channel][sample] *= smoothedGain;
        }
    }

    smoothedGain = currentGain;
    
    for (int c = 0; c < synthBuffer.getNumChannels(); ++c)
    {
        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            outputBuffer.addFrom(c, startSample, synthBuffer, c, 0, numSamples);
        }
    }*/

};


void SynthVoice::parameterChanged(const String& paramID, float newValue)
{
    if (paramID == "OSC1WAVETYPE")
    {
        osc1.setWaveType(newValue);
    }

    if (paramID == "OSC2WAVETYPE")
    {
        osc2.setWaveType(newValue);
    }

    if (paramID == "OSC2OCTAVE")
    {
        osc2.setOctave(newValue);
    }

    if (paramID == "ATTACK")
    {
        adsr.updateADSRParam(newValue, 0);
    }

    if (paramID == "DECAY")
    {
        adsr.updateADSRParam(newValue, 1);
    }

    if (paramID == "SUSTAIN")
    {
        adsr.updateADSRParam(newValue, 2);
    }   
    
    if (paramID == "RELEASE")
    {
        adsr.updateADSRParam(newValue, 3);
    }

    if (paramID == "GAIN")
    {
        //currentGain = newValue;
        gain.setGainLinear(Decibels::decibelsToGain(newValue));
    }
    
    if (paramID == "PRESENCE")
    {
        setPresence(newValue);
    }

    if (paramID == "BITNUMBER")
    {
        bitNumber = newValue;
    }

    if (paramID == "HIGHFREQ")
    {
        highpassFilter.setCutoffFrequency(newValue);
    }

    if (paramID == "LOWFREQ")
    {
        lowpassFilter.setCutoffFrequency(newValue);
    }
};

void SynthVoice::setPresence(float newValue)
{
    presenceOld1 = presence1;
    presenceOld2 = presence2;

    presence1 = sqrt(1.0f - newValue);
    presence2 = sqrt(newValue);

    applySmooth = true;
}


//float SynthVoice::setGain(float processedSample)
//{
//    smoothedGain = Decibels::decibelsToGain(smoothedGain);
//
//    //auto* channelData = synthBuffer.getArrayOfWritePointers();
//
//    //if (abs(smoothedGain - smoothedGainOld) > 0.0001)
//        /*for (int sample = 0; sample < synthBuffer.getNumSamples(); ++sample)
//        {*/
//    smoothedGainOld += 0.003f * (smoothedGain - smoothedGainOld);
//    /*for (int channel = 0; channel < synthBuffer.getNumChannels(); ++channel)
//    {*/
//    //channelData[channel][sample] *= smoothedGainOld;
//    processedSample *= smoothedGainOld;
//    //}
////}
////else
//    //synthBuffer.applyGain(smoothedGain);
//
//    smoothedGainOld = smoothedGain;
//    return  processedSample;
//}