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
        if (fifoIndex == fftSize)              
        {
            if (!nextFFTBlockReady)            
            {
                juce::zeromem(fftData, sizeof(fftData));
                memcpy(fftData, fifo, sizeof(fifo));
                nextFFTBlockReady = true;
            }

            fifoIndex = 0;
        }
        fifo[fifoIndex++] = sample;             
    }

    void drawNextFrameOfSpectrum()
    {
        // first apply a windowing function to our data
        window.multiplyWithWindowingTable(fftData, fftSize);       

        // then render our FFT data..
        forwardFFT.performFrequencyOnlyForwardTransform(fftData);  

        auto mindB = -100.0f;
        auto maxdB = 0.0f;

        for (int i = 0; i < scopeSize; ++i)                         
        {
            auto skewedProportionX = 1.0f - std::exp(std::log(1.0f - (float)i / (float)scopeSize) * 0.2f);
            auto fftDataIndex = juce::jlimit(0, fftSize / 2, (int)(skewedProportionX * (float)fftSize * 0.5f));
            auto level = juce::jmap(juce::jlimit(mindB, maxdB, juce::Decibels::gainToDecibels(fftData[fftDataIndex])
                - juce::Decibels::gainToDecibels((float)fftSize)),
                mindB, maxdB, 0.0f, 1.0f);

            scopeData[i] = level;                                  
        }
    }

    void drawFrame(juce::Graphics& g)
    {
        for (int i = 1; i < scopeSize; ++i)
        {
            auto width = getLocalBounds().getWidth();
            auto height = getLocalBounds().getHeight();

            g.setColour(juce::Colour(0xFFB4D989));

            g.drawLine({ (float)juce::jmap(i - 1, 0, scopeSize - 1, 0, width),
                                  juce::jmap(scopeData[i - 1], 0.0f, 1.0f, (float)height, 0.0f),
                          (float)juce::jmap(i,     0, scopeSize - 1, 0, width),
                                  juce::jmap(scopeData[i],     0.0f, 1.0f, (float)height, 0.0f) });
        }
    }

    enum
    {
        fftOrder = 11,             
        fftSize = 1 << fftOrder,  
        scopeSize = 512            
    };

private:
    juce::dsp::FFT forwardFFT;                     
    juce::dsp::WindowingFunction<float> window;    

    float fifo[fftSize];                           
    float fftData[2 * fftSize];                    
    int fifoIndex = 0;                             
    bool nextFFTBlockReady = false;                
    float scopeData[scopeSize];                    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalyzerComponent)
};
