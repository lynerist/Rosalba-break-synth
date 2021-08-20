/*
  ==============================================================================

    AdsrData.cpp
    Created: 19 Aug 2021 6:24:38pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#include "AdsrData.h"

void AdsrData::updateADSR(const float attack, const float decay, const float sustain, const float release) {

    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;

    setParameters(adsrParams);
}
