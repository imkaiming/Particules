#pragma once

#include "../../framework/Core.h"
#include "../../framework/GuiTypes.h"

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    class LightRotarySliderLNF : public juce::LookAndFeel_V4
    {
    public:
        LightRotarySliderLNF()
        {
            setColour(juce::Slider::rotarySliderFillColourId, juce::Colour(0xFF4A90E2)); // blue accent
            setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colour(0xFFAAAAAA));
            setColour(juce::Label::textColourId, juce::Colour(0xFF4A90E2));
        }
        ~LightRotarySliderLNF() = default;

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float startAngle,
            float endAngle, juce::Slider& slider)
        {
            auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(8);

            float centreX = bounds.getCentreX();
            float centreY = bounds.getCentreY();
            float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f - 4.0f;

            g.setColour(juce::Colour(0xFFF5F5F5)); // light gray background
            g.fillEllipse(bounds);

            g.setColour(juce::Colour(0xFFDDDDDD));
            g.drawEllipse(bounds, 2.0f);

            juce::Path track;
            track.addCentredArc(centreX, centreY, radius, radius, 0.0f, startAngle, endAngle, true);

            g.setColour(juce::Colour(0xFFCCCCCC));
            g.strokePath(track, juce::PathStrokeType(6.0f, juce::PathStrokeType::curved));

            float angle = startAngle + sliderPos * (endAngle - startAngle);

            juce::Path valueArc;
            valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, startAngle, angle, true);

            g.setColour(juce::Colour(0xFF4A90E2)); 
            g.strokePath(valueArc, juce::PathStrokeType(6.0f, juce::PathStrokeType::curved));

            float indicatorLength = radius * 0.75f;
            float indicatorThickness = 5.0f;

            float ix = centreX;
            float iy = centreY - indicatorLength * 0.5f; 

            g.setColour(juce::Colour(0xFF2C5AA0)); 
            g.fillRect(juce::Rectangle<float>(ix - indicatorThickness / 2, iy, indicatorThickness, indicatorLength));

            g.setColour(juce::Colour(0xFF4A90E2));
            g.setFont(juce::Font(22.0f, juce::Font::bold));

            juce::String valueText = juce::String(slider.getValue(), 2);
            g.drawText(valueText, bounds.removeFromTop(bounds.getHeight() * 0.35f), juce::Justification::centredTop, true);

            if(slider.getName().isNotEmpty())
            {
                g.setColour(juce::Colours::grey);
                g.setFont(13.0f);
                g.drawText(slider.getName(), bounds.removeFromBottom(20), juce::Justification::centred, true);
            }
        }

        void drawLabel(juce::Graphics& g, juce::Label& label)
        {
            g.setColour(label.findColour(juce::Label::textColourId));
            g.setFont(label.getFont());
            g.drawText(label.getText(), label.getLocalBounds(), label.getJustificationType(), true);
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LightRotarySliderLNF)
    };
}