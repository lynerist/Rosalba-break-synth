/*
  ==============================================================================

    SynthVoice.cpp
    Created: 16 Aug 2021 3:59:58pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include "SynthVoice.h"


bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {

    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
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

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {

    adsr.setSampleRate(sampleRate);

    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    osc1.prepareToPlay(spec);
    osc2.prepareToPlay(spec);
    gain.prepare(spec);

    gain.setGainLinear(DEFAULT_GAIN);
    bitNumber = 24;

    highpassFilter.prepare(spec);
    lowpassFilter.prepare(spec);

    highpassFilter.setType(juce::dsp::StateVariableTPTFilterType::highpass);
    highpassFilter.setCutoffFrequency(MIN_FREQ);
    lowpassFilter.setType(juce::dsp::StateVariableTPTFilterType::lowpass);
    lowpassFilter.setCutoffFrequency(MAX_FREQ);
        
    isPrepared = true;
}

void SynthVoice::update(const float attack, const float decay, const float sustain, const float release, const float newGain, const float newPresence, const int newBitNumber, const int highfreq, const int lowfreq) {

    adsr.updateADSR(attack, decay, sustain, release);
    gain.setGainLinear(newGain);
    osc1.setPresence(1.0f - newPresence);
    osc2.setPresence(newPresence);
    bitNumber = newBitNumber;

    highpassFilter.setCutoffFrequency(highfreq);
    lowpassFilter.setCutoffFrequency(lowfreq);
}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {

    jassert(isPrepared);

    if (! isVoiceActive())
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };  //audioBlock alias di buffer di classe AudioBlock

    osc1.getNextAudioBlock(audioBlock);
    osc2.getNextAudioBlock(audioBlock);
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    int quantizationSteps = exp2(bitNumber);

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
};
