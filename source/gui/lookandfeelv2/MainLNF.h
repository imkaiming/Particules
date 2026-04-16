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

        void drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float sliderPos, float startAngle, float endAngle,
            juce::Slider& slider) override;

        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
            float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider) override;

        void drawButtonBackground(juce::Graphics& g, juce::Button& button, const color& backgroundColour, bool isMouseOverButton,
            bool isButtonDown) override;

        void drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override;

        juce::Font getLabelFont(juce::Label& label) override;

        int getPopupMenuBorderSize() override;

        void getIdealPopupMenuItemSize(
            const str& text, bool isSeparator, int standardMenuItemHeight, int& idealWidth, int& idealHeight) override;

        void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;

        void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive,
            bool isHighlighted, bool isTicked, bool hasSubMenu, const str& text, const str& shortcutKeyText,
            const juce::Drawable* icon, const color* textColour) override;

        juce::Typeface::Ptr getGeistLight() const { return geistLight; }

    private:
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

        void drawRotaryMenuKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
            float endAngle, float sliderPos, juce::Slider& slider) const;

        // Visual Effects
        void drawJitterArc(juce::Graphics& g, float cx, float cy, float baseArcRadius, float startAngle, float endAngle,
            float primaryAngle, float jitterAmount, float baseLineWidth = 2.f) const;

        void drawArcGlow(juce::Graphics& g, const juce::Path& path, color c, float width, int steps = 8,
            float spreadMultiplier = 12.0f, float alphaBase = 0.1f) const;

        void drawColoredArc(juce::Graphics& g, float cx, float cy, float radius, float startAngle, float endAngle, color c,
            float thickness = 2.f) const;

        void drawBorderArc(juce::Graphics& g, float cx, float cy, float radius, float startAngle, float endAngle,
            float alpha = 0.2f, float thickness = 2.f) const;

        // Helpers
        void updateArcPath(juce::Path& path, float cx, float cy, float radius, float startAngle, float endAngle) const;

        void drawContour(juce::Graphics& g, float cx, float cy, float radius, color c, float thickness = 2.f) const;

        void drawFillBackground(juce::Graphics& g, float cx, float cy, float innerR, color c) const;

        // Fonts
        juce::Typeface::Ptr geistThin;
        juce::Typeface::Ptr geistExtraLight;
        juce::Typeface::Ptr geistLight;
        juce::Typeface::Ptr geistRegular;
        juce::Typeface::Ptr geistMedium;
        juce::Typeface::Ptr geistSemiBold;
        juce::Typeface::Ptr geistBold;
        juce::Typeface::Ptr geistExtraBold;
        juce::Typeface::Ptr geistBlack;

        juce::Font valueFont;

        // cached variable to avoid recomputing it
        mutable juce::Path tempArcPath;
        mutable juce::Path tempMenuCurve;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainLNF)
    };
}