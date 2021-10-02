#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
RosalbabreaksynthAudioProcessor::RosalbabreaksynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), false)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
     ), apvts(*this, nullptr, Identifier("RosalbaBreakSynthParameters"), createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
    /*synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());
    synth.addVoice(new SynthVoice());*/

   /* for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {*/

            auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(0));
            apvts.addParameterListener("OSC1WAVETYPE", voice);
            apvts.addParameterListener("OSC2WAVETYPE", voice);
            apvts.addParameterListener("OSC2OCTAVE", voice);
            apvts.addParameterListener("GAIN", voice);
            apvts.addParameterListener("PRESENCE", voice);
            apvts.addParameterListener("BITNUMBER", voice);
            apvts.addParameterListener("HIGHFREQ", voice);
            apvts.addParameterListener("LOWFREQ", voice);
            apvts.addParameterListener("ATTACK", voice);
            apvts.addParameterListener("DECAY", voice);
            apvts.addParameterListener("SUSTAIN", voice);
            apvts.addParameterListener("RELEASE", voice);
    /*    }
    }*/

}

RosalbabreaksynthAudioProcessor::~RosalbabreaksynthAudioProcessor()
{
}

//==============================================================================
const juce::String RosalbabreaksynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RosalbabreaksynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool RosalbabreaksynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool RosalbabreaksynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double RosalbabreaksynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int RosalbabreaksynthAudioProcessor::getNumPrograms()
{
    return 1;   
}

int RosalbabreaksynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void RosalbabreaksynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String RosalbabreaksynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void RosalbabreaksynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void RosalbabreaksynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); i++) {

        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {

            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void RosalbabreaksynthAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool RosalbabreaksynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void RosalbabreaksynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
   
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    //for (int i = 0; i < synth.getNumVoices(); ++i) {
    //
    //    if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
    //        
    //        //Oscillator 1
    //        auto& osc1WaveChoice = *apvts.getRawParameterValue("OSC1WAVETYPE");
    //        
    //        //Oscillator 2
    //        auto& osc2WaveChoice = *apvts.getRawParameterValue("OSC2WAVETYPE");
    //        auto& osc2OctaveChoice = *apvts.getRawParameterValue("OSC2OCTAVE");

    //        //Gain Presence BitNumber
    //        auto& gain = *apvts.getRawParameterValue("GAIN");
    //        auto& presence = *apvts.getRawParameterValue("PRESENCE");
    //        auto& bitNumber = *apvts.getRawParameterValue("BITNUMBER");

    //        //Filter
    //        auto& highFreq = *apvts.getRawParameterValue("HIGHFREQ");
    //        auto& lowFreq = *apvts.getRawParameterValue("LOWFREQ");

    //        //ADSR
    //        auto& attack = *apvts.getRawParameterValue("ATTACK");
    //        auto& decay = *apvts.getRawParameterValue("DECAY");
    //        auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
    //        auto& release = *apvts.getRawParameterValue("RELEASE");
    //        
    //        //voice->update(attack.load(), decay.load(), sustain.load(), release.load(), gain.load(), presence.load(), bitNumber.load(), highFreq.load(), lowFreq.load());
    //        voice->getOscillator(1).setWaveType(osc1WaveChoice);
    //        voice->getOscillator(2).setWaveType(osc2WaveChoice);
    //        voice->getOscillator(2).setOctave(osc2OctaveChoice);
    //    }
    //}

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    //Serve per visualizzare lo spettro
    analyzer.getNextAudioBlock(buffer, 0);
}
                                
//==============================================================================
bool RosalbabreaksynthAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* RosalbabreaksynthAudioProcessor::createEditor()
{
    return new RosalbabreaksynthAudioProcessorEditor (*this);
}

//==============================================================================
void RosalbabreaksynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void RosalbabreaksynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RosalbabreaksynthAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout RosalbabreaksynthAudioProcessor::createParams() {

    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    //OSC 1
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC1WAVETYPE", "Osc 1 Wave Type", juce::StringArray{ "Sine", "Saw", "Square", "Plus" }, 0));
    
    //OSC 2
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2WAVETYPE", "Osc 2 Wave Type", juce::StringArray{ "Sine", "Saw", "Square", "Plus" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC2OCTAVE", "Octave Shift", juce::StringArray{ "-2", "-1", "0", "+1", "+2"}, 2));

    //GAIN, PRESENCE and BITNUMBER
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", juce::NormalisableRange<float>{-48.00f, 6.00f, INTERVAL_VALUE}, DEFAULT_GAIN));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRESENCE", "Presence", juce::NormalisableRange<float>{0.0f, 1.0f}, DEFAULT_PRESENCE));
    //params.push_back(std::make_unique<juce::AudioParameterChoice>("BITNUMBER", "Bit Number", juce::StringArray{"1","2","3","4","5","6","7","8","9","10","12","16","24"}, 12));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BITNUMBER", "Bit Number", juce::NormalisableRange<float>{1.00f, 24.00f, INTERVAL_VALUE}, DEFAULT_BITNUMBER)); //check again
    //FILTER
    params.push_back(std::make_unique<juce::AudioParameterInt>("HIGHFREQ", "Highpass cutoff frequency", MIN_FREQ, MAX_FREQ, MIN_FREQ));
    params.push_back(std::make_unique<juce::AudioParameterInt>("LOWFREQ", "Lowpass cutoff frequency", MIN_FREQ, MAX_FREQ, MAX_FREQ));   

    //ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float>{MIN_ADSR, MAX_ADSR, INTERVAL_VALUE}, DEFAULT_ATTACK));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float>{MIN_ADSR, MAX_ADSR, INTERVAL_VALUE}, DEFAULT_DECAY));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float>{MIN_ADSR, 1.00f, INTERVAL_VALUE}, DEFAULT_SUSTAIN));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float>{MIN_ADSR, MAX_ADSR, INTERVAL_VALUE}, DEFAULT_RELEASE));
    
    return { params.begin(), params.end() };
}

