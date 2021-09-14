/*
  ==============================================================================

    SynthVoice.h
    Created: 16 Aug 2021 3:59:58pm
    Author:  Cecilia Rossi

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include "../SynthVoice.h"

//==============================================================================
class AnalyzerComponent : public juce::Component,
    private juce::Timer
{
public:
    AnalyzerComponent()
        : forwardFFT(fftOrder),
        window(fftSize, juce::dsp::WindowingFunction<float>::hann)
    {
        setOpaque(true);
        startTimerHz(30);
        setSize(14 U, 7 U);
    }

    ~AnalyzerComponent() override
    {
    }

    //==============================================================================

    void getNextAudioBlock(const juce::AudioBuffer<float>& bufferToFill, int startSample)
    {

        if (bufferToFill.getNumChannels() > 0)
        {
            auto* channelData = bufferToFill.getReadPointer(0, startSample);

            for (auto i = 0; i < bufferToFill.getNumSamples(); ++i)
                pushNextSampleIntoFifo(channelData[i]);
        }
    }

    //==============================================================================
    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::black);

        g.setOpacity(1.0f);
        g.setColour(juce::Colours::white);
        drawFrame(g);
    }

    void timerCallback() override
    {
        if (nextFFTBlockReady)
        {
            drawNextFrameOfSpectrum();
            nextFFTBlockReady = false;
            repaint();
        }
    }

    void pushNextSampleIntoFifo(float sample) noexcept
    {
        // if the fifo contains enough data, set a flag to say
        // that the next frame should now be rendered..
        if (fifoIndex == fftSize)               // [11]
        {
            if (!nextFFTBlockReady)            // [12]
            {
                juce::zeromem(fftData, sizeof(fftData));
                memcpy(fftData, fifo, sizeof(fifo));
                nextFFTBlockReady = true;
            }

            fifoIndex = 0;
        }

        fifo[fifoIndex++] = sample;             // [12]
    }

    void drawNextFrameOfSpectrum()
    {
        // first apply a windowing function to our data
        window.multiplyWithWindowingTable(fftData, fftSize);       // [1]

        // then render our FFT data..
        forwardFFT.performFrequencyOnlyForwardTransform(fftData);  // [2]

        auto mindB = -100.0f;
        auto maxdB = 0.0f;

        for (int i = 0; i < scopeSize; ++i)                         // [3]
        {
            auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
            auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
            auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftData[fftDataIndex])
                - juce::Decibels::gainToDecibels((float)fftSize)),
                mindB, maxdB, 0.0f, 1.0f);

            scopeData[i] = level;                                   // [4]
        }
    }

    void drawFrame(juce::Graphics& g)
    {
        for (int i = 1; i < scopeSize; ++i)
        {
            auto width = getLocalBounds().getWidth();
            auto height = getLocalBounds().getHeight();

            g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                                  juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                          (float)juce::jmap(i,     0, scopeSize - 1, 0, width),
                                  juce::jmap(scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
        }
    }

    enum
    {
        fftOrder = 11,             // [1]
        fftSize = 1 << fftOrder,  // [2]
        scopeSize = 512             // [3]
    };

private:
    juce::dsp::FFT forwardFFT;                      // [4]
    juce::dsp::WindowingFunction<float> window;     // [5]

    float fifo[fftSize];                           // [6]
    float fftData[2 * fftSize];                    // [7]
    int fifoIndex = 0;                              // [8]
    bool nextFFTBlockReady = false;                 // [9]
    float scopeData[scopeSize];                    // [10]

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyzerComponent)
};
