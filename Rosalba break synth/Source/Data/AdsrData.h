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
    void AdsrData::updateADSRParam(const float newValue, int adsrID)
    {
        switch (adsrID)
        {
        case 0:
            adsrParams.attack = newValue;
            break;
        case 1:
            adsrParams.decay = newValue;
            break;
        case 2:
            adsrParams.sustain = newValue;
            break;
        case 3:
            adsrParams.release = newValue;
            break;
        default:
            jassertfalse;
            break;
        }

        setParameters(adsrParams);
    }

private:
    juce::ADSR::Parameters adsrParams;
};
