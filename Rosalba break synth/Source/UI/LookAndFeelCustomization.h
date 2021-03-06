/*
  ==============================================================================

    LookAndFeelCustomization.h
    Created: 15 Sep 2021 4:45:51pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once
#include <juceHeader.h>
#include "BinaryData.h"

class LookAndFeelCustomization : public juce::LookAndFeel_V4 {

public:
    LookAndFeelCustomization() {}
    ~LookAndFeelCustomization() {}

    void LookAndFeelCustomization::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
        const bool isSeparator, const bool isActive,
        const bool isHighlighted, const bool isTicked,
        const bool hasSubMenu, const juce::String& text,
        const juce::String& shortcutKeyText,
        const juce::Drawable* icon, const juce::Colour* const textColourToUse)
    {
        g.setColour(juce::Colour(0xC0B4D989));

        if (isHighlighted && isActive)
        {
            g.fillRect(area);
            g.setColour(juce::Colours::black);
        }

        g.setFont(LookAndFeelCustomization::getCustomFont().withHeight(0.7 U));
        g.drawFittedText(text, area.reduced(6, 0), juce::Justification::centred, 1);
    }

    void LookAndFeelCustomization::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
    {
        g.fillAll(juce::Colours::black);

        g.setColour(juce::Colour(0xF0B88546));
        g.drawRect(0.f, 0.f, (float)width, (float)height, 1.5f);
    }

    void LookAndFeelCustomization::getIdealPopupMenuItemSize(const juce::String& text, const bool isSeparator,
        int standardMenuItemHeight, int& idealWidth, int& idealHeight)
    {
        idealHeight = standardMenuItemHeight;
        idealWidth = 0;
    }

    //font embedding
    static const Font getCustomFont()
    {
        static auto typeface = Typeface::createSystemTypefaceFor(BinaryData::AvaraBold_ttf, BinaryData::AvaraBold_ttfSize);
        return Font(typeface);
    }

    Typeface::Ptr getTypefaceForFont(const Font& f) override
    {
        return getCustomFont().getTypeface();
    }
    
    Font getComboBoxFont(ComboBox& cBox) override
    {
        return getCustomFont().withHeight(0.7 U);
    }

    Font getLabelFont(Label& l) override
    {
        return getCustomFont().withHeight(0.7 U);
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeelCustomization)
};