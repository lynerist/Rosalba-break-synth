/*
  ==============================================================================

    AdsrData.h
    Created: 19 Aug 2021 6:24:38pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdsrData : public juce::ADSR
{
public:
    void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release) {

        adsrParams.attack = attack;
        adsrParams.decay = decay;
        adsrParams.sustain = sustain;
        adsrParams.release = release;

        setParameters(adsrParams);
    }

private:
    juce::ADSR::Parameters adsrParams;
};
