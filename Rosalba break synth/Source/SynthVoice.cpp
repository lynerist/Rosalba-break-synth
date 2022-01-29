/*
  ==============================================================================

    SynthVoice.cpp
    Created: 16 Aug 2021 3:59:58pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include "SynthVoice.h"
#include <iostream>

SynthVoice::SynthVoice()
{
    adsr.updateADSRParam(DEFAULT_ATTACK, 0);
    adsr.updateADSRParam(DEFAULT_DECAY, 1);
    adsr.updateADSRParam(DEFAULT_SUSTAIN, 2);
    adsr.updateADSRParam(DEFAULT_RELEASE, 3);

    bitNumber = DEFAULT_BITNUMBER;
    
    presence1 = presence2 = presenceOld1 = presenceOld2 = sqrt(DEFAULT_PRESENCE);

    highpassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highpassFilter.setCutoffFrequency(MIN_FREQ);
    lowpassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    lowpassFilter.setCutoffFrequency(MAX_FREQ);

    gain.setRampDurationSeconds(0.09);
    gain.setGainLinear(Decibels::decibelsToGain(DEFAULT_GAIN));
}

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
    
    synthBuffer.setSize(1, samplesPerBlock, false, false, true);
    synthBuffer2.setSize(1, samplesPerBlock, false, false, true);

    osc1.prepareToPlay(spec);
    osc2.prepareToPlay(spec);
    
    gain.prepare(spec);
    
    highpassFilter.prepare(spec);
    lowpassFilter.prepare(spec);

    const auto corrCoeff = IIRCoefficients::makeHighPass(sampleRate, 20.0, 1.0 / sqrt(2.0));
    const auto corrCoeff2 = IIRCoefficients::makeLowPass(sampleRate, 20000.0, 1.0 / sqrt(2.0));
    
    correctionFilter.setCoefficients(corrCoeff);
    correctionFilter.reset();

    correctionFilter2.setCoefficients(corrCoeff2);
    correctionFilter2.reset();

    correctionFilter3.setCoefficients(corrCoeff);
    correctionFilter3.reset();

    correctionFilter4.setCoefficients(corrCoeff2);
    correctionFilter4.reset();

    correctionFilter5.setCoefficients(corrCoeff);
    correctionFilter5.reset();

    correctionFilter6.setCoefficients(corrCoeff2);
    correctionFilter6.reset();

    correctionFilter7.setCoefficients(corrCoeff);
    correctionFilter7.reset();

    correctionFilter8.setCoefficients(corrCoeff2);
    correctionFilter8.reset();

    isPrepared = true;
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {

    jassert(isPrepared);

    if (! isVoiceActive())
        return;

    synthBuffer.clear();
    synthBuffer2.clear();
    
    dsp::AudioBlock<float> audioBlock{ synthBuffer };
    dsp::AudioBlock<float> audioBlock2{ synthBuffer2 };
    
    osc1.getNextAudioBlock(audioBlock);
    osc2.getNextAudioBlock(audioBlock2);

    auto synthBufferData = synthBuffer.getWritePointer(0);
    auto synthBufferData2 = synthBuffer2.getWritePointer(0);

    correctionFilter.processSamples(synthBufferData + startSample, numSamples);
    correctionFilter2.processSamples(synthBufferData + startSample, numSamples);

    correctionFilter3.processSamples(synthBufferData2 + startSample, numSamples);
    correctionFilter4.processSamples(synthBufferData2 + startSample, numSamples);

    //presence
    if (applySmooth)
    {
        synthBuffer.applyGainRamp(startSample, numSamples, presenceOld1, presence1);
    
       /* for (int ch = synthBuffer.getNumChannels(); --ch >= 0;)
        {*/
            synthBuffer.addFromWithRamp(0, startSample, synthBufferData2, numSamples, presenceOld2, presence2);
        //}

        applySmooth = false;
    }
    else 
    {
        synthBuffer.applyGain(presence1);

        /*for (int ch = synthBuffer.getNumChannels(); --ch >= 0;)
        {*/
            synthBuffer.addFrom(0, startSample, synthBuffer2, 0, startSample, numSamples, presence2);
        //}
    }
    

    adsr.applyEnvelopeToBuffer(synthBuffer, startSample, numSamples);
    
    float quantizationSteps = exp2(bitNumber);
    float quantizationStepsReversed = 1.0f / quantizationSteps;

    //for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {

        for (int sample = startSample; sample < (startSample + numSamples); ++sample) {
            //Quantization noise distortion
            float processedSample = round((synthBuffer.getSample(0, sample)* quantizationSteps)) * quantizationStepsReversed;

            //Filter application
            processedSample = highpassFilter.processSample(0, processedSample);
            processedSample = lowpassFilter.processSample(0, processedSample);

            //Gain 
            processedSample = gain.processSample(processedSample);

            synthBuffer.setSample(0, sample, processedSample);
        }

    //}

        correctionFilter5.processSamples(synthBufferData + startSample, numSamples);
        correctionFilter6.processSamples(synthBufferData + startSample, numSamples);

        correctionFilter7.processSamples(synthBufferData2 + startSample, numSamples);
        correctionFilter8.processSamples(synthBufferData2 + startSample, numSamples);

        outputBuffer.addFrom(0, startSample, synthBuffer, 0, startSample, numSamples);

        if (! adsr.isActive())
            clearCurrentNote();

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
