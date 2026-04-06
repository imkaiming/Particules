#pragma once

#include <juce_core/juce_core.h> // end macro
#include <juce_graphics/juce_graphics.h> // juce image
#include <juce_gui_basics/juce_gui_basics.h> // look and feel

namespace juce
{
    class Graphics;
}

namespace particules
{
    class MainLNF : public juce::LookAndFeel_V4
    {
    public:
        MainLNF();
        ~MainLNF() = default;

        void drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float sliderPos, float startAngle, float endAngle,
            juce::Slider& slider) override;

        void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
            float maxSliderPos, const juce::Slider::SliderStyle, juce::Slider& slider) override;

        juce::Typeface::Ptr getGeistTypeface() const noexcept { return geistTypeface; }
        juce::Typeface::Ptr getCascadaTypeface() const noexcept { return cascadaTypeface; }
        juce::Typeface::Ptr getFunnelTypeface() const noexcept { return funnelTypeface; }
        juce::Typeface::Ptr getFuturaTypeface() const noexcept { return futuraTypeface; }

    private:
        void drawRotarySliderCenteredText(juce::Graphics& g, juce::Slider& slider, float cx, float cy, float radius);

        void drawPrimaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
            float endAngle, float sliderPos, juce::Slider& slider);

        void drawPrimaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle, float endAngle,
            float sliderPos, juce::Slider& slider);

        void drawSecondaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
            float endAngle, float sliderPos, juce::Slider& slider);

        void drawTertiaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle, float endAngle,
            float sliderPos, juce::Slider& slider);

        void drawAuxKnob(juce::Graphics& g, float cx, float cy, float innerR, float startAngle, float endAngle, float sliderPos);


        //void drawValue(juce::Graphics& g, float cx, float cy, float radius, bool isPrimary, float angle, float valDist,
        //    juce::Slider& slider);
        //void drawIndicator(juce::Graphics& g, float cx, float cy, float innerR, float angle, bool isPrimary);
        //void drawKnobBody(juce::Graphics& g, float cx, float cy, float innerR, bool isPrimary, juce::Colour faceCol);

        juce::Typeface::Ptr geistTypeface;
        juce::Typeface::Ptr funnelTypeface;
        juce::Typeface::Ptr cascadaTypeface;
        juce::Typeface::Ptr futuraTypeface;
        juce::Font valueFont;
    };
}