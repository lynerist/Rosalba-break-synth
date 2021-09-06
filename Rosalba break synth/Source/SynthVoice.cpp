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

    gain.setGainLinear(0.5f);
    
    isPrepared = true;
}

void SynthVoice::update(const float attack, const float decay, const float sustain, const float release, const float newGain, const float newPresence) {

    adsr.updateADSR(attack, decay, sustain, release);
    gain.setGainLinear(newGain);
    osc1.setPresence(newPresence);
    osc2.setPresence(1.0f- newPresence);
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
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {

        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (! adsr.isActive())
            clearCurrentNote();
    }
};
