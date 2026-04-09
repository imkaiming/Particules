#pragma once

#include "../../framework/GuiTypes.h"
#include <juce_core/juce_core.h> // end macro
#include <juce_graphics/juce_graphics.h> // juce image
#include <juce_gui_basics/juce_gui_basics.h> // look and feel

namespace juce
{
    class Graphics;
}

namespace particules
{
    /**
     * @class MainLNF
     * @brief Custom LookAndFeel implementation for the Particules plugin suite.
     */
    class MainLNF : public juce::LookAndFeel_V4
    {
    public:
        MainLNF();
        ~MainLNF() override = default;

        //==============================================================================
        // juce::LookAndFeel_V4 Overrides
        void drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float sliderPos, float startAngle, float endAngle,
            juce::Slider& slider) override;

        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
            float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override;

        //==============================================================================
        // Typeface & Font Accessors
        [[nodiscard]] juce::Typeface::Ptr getGeistTypeface() const noexcept { return geistTypeface; }
        [[nodiscard]] juce::Typeface::Ptr getCascadaTypeface() const noexcept { return cascadaTypeface; }
        [[nodiscard]] juce::Typeface::Ptr getFunnelTypeface() const noexcept { return funnelTypeface; }
        [[nodiscard]] juce::Typeface::Ptr getFuturaTypeface() const noexcept { return futuraTypeface; }

        [[nodiscard]] juce::Font getFont() const noexcept { return valueFont; }

    private:
        //==============================================================================
        // Internal Knob Drawing Logic
        void drawRotarySliderCenteredText(juce::Graphics& g, juce::Slider& slider, float cx, float cy, float radius) const;

        void drawPrimaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
            float endAngle, float sliderPos, juce::Slider& slider) const;

        void drawSecondaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
            float endAngle, float sliderPos, juce::Slider& slider) const;

        void drawPrimaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle, float endAngle,
            float sliderPos, juce::Slider& slider) const;

        void drawSecondaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
            float endAngle, float sliderPos, juce::Slider& slider) const;

        void drawTertiaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle, float endAngle,
            float sliderPos, juce::Slider& slider) const;

        void drawAuxKnob(
            juce::Graphics& g, float cx, float cy, float innerR, float startAngle, float endAngle, float sliderPos) const;

        //==============================================================================
        // Visual Primitives & Effects
        void drawJitterArc(juce::Graphics& g, float cx, float cy, float baseArcRadius, float startAngle, float endAngle,
            float primaryAngle, float jitterAmount, float baseLineWidth) const;

        void drawArcGlow(juce::Graphics& g, const juce::Path& path, color c, float baseWidth, int steps = 8,
            float spreadMultiplier = 12.0f, float alphaBase = 0.1f) const;

        void drawColoredArc(juce::Graphics& g, float cx, float cy, float radius, float startAngle, float endAngle,
            juce::Colour color, float thickness) const;

        void drawBorderArc(juce::Graphics& g, float cx, float cy, float radius, float startAngle, float endAngle,
            float alpha = 0.15f, float thickness = 2.f) const;

        //==============================================================================
        // Low-level Geometry Helpers
        [[nodiscard]] juce::Path createArcPath(float cx, float cy, float radius, float startAngle, float endAngle) const;

        void drawContour(juce::Graphics& g, float cx, float cy, float radius, color c, float thickness = 1.5f) const;

        void drawFillBackground(juce::Graphics& g, float cx, float cy, float innerR, color c) const;

        void fillKnobFace(juce::Graphics& g, float cx, float cy, float radius) const;

        //==============================================================================
        // Resources
        juce::Typeface::Ptr geistTypeface;
        juce::Typeface::Ptr funnelTypeface;
        juce::Typeface::Ptr cascadaTypeface;
        juce::Typeface::Ptr futuraTypeface;

        juce::Font valueFont;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLNF)
    };
}