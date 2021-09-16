/*
  ==============================================================================

    LookAndFeelCustomization.h
    Created: 15 Sep 2021 4:45:51pm
    Author:  Cecilia Rossi

  ==============================================================================
*/

#pragma once
#include <juceHeader.h>

class LookAndFeelCustomization : public juce::LookAndFeel_V4 {

public:
    LookAndFeelCustomization() {}
    ~LookAndFeelCustomization() {}

    /*void LookAndFeelCustomization::drawComboBox(juce::Graphics& g, int width, int height, bool,
        int, int, int, int, juce::ComboBox& box) override {

        auto arrowZone = juce::Rectangle<int>(width - 30, 0, 30, height).reduced(0, 8);
        juce::Path path;

        path.addTriangle(juce::Point<float>((float)arrowZone.getX(), (float)arrowZone.getY()),
            juce::Point<float>((float)arrowZone.getRight(), (float)arrowZone.getY()),
            juce::Point<float>((float)arrowZone.getCentreX(), (float)arrowZone.getBottom()));


        g.setColour(box.findColour(juce::ComboBox::arrowColourId));
        g.fillPath(path);
    }*/

    void LookAndFeelCustomization::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area,
        const bool isSeparator, const bool isActive,
        const bool isHighlighted, const bool isTicked,
        const bool hasSubMenu, const juce::String& text,
        const juce::String& shortcutKeyText,
        const juce::Drawable* icon, const juce::Colour* const textColourToUse)
    {
        g.setColour(juce::Colour(0x80B4D989));

        if (isHighlighted && isActive)
        {
            g.fillRect(area);
            g.setColour(juce::Colours::whitesmoke);
        }

        g.setFont(16);
        g.drawFittedText(text, area.reduced(6, 0), juce::Justification::left, 1);
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

};