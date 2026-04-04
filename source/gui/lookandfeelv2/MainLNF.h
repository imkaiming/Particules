#pragma once

#include <juce_core/juce_core.h> // end macro
#include <juce_graphics/juce_graphics.h> // juce image
#include <juce_gui_basics/juce_gui_basics.h> // look and feel

#include "../../utils/math/MathConstants.h"
#include "BinaryData.h"
#include "Colours.h"

namespace particules
{

    class MainLNF : public juce::LookAndFeel_V4
    {
    public:
        MainLNF()
        {
            // 1. Charger le Typeface depuis le binaire (ex: Geist-Medium.ttf)
            // Note : Le nom dans BinaryData correspond au nom du fichier avec des underscores
            geistTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::GeistLight_ttf, BinaryData::GeistLight_ttfSize);
            cascadaTypeface =
                juce::Typeface::createSystemTypefaceFor(BinaryData::CascadiaMonoLight_otf, BinaryData::CascadiaMonoLight_otfSize);
            funnelTypeface = juce::Typeface::createSystemTypefaceFor(
                BinaryData::FunnelDisplayLight_ttf, BinaryData::FunnelDisplayLight_ttfSize);
            futuraTypeface = juce::Typeface::createSystemTypefaceFor(
                BinaryData::futura_medium_condensed_bt_ttf, BinaryData::futura_medium_condensed_bt_ttfSize);

            // 2. Initialiser la Font avec ce Typeface
            valueFont = juce::Font(geistTypeface).withHeight(14.0f);
        }

        ~MainLNF() = default;

        void drawRotarySlider(juce::Graphics& g, int x, int y, int w, int h, float sliderPos, float startAngle, float endAngle,
            juce::Slider& slider) override
        {
            const bool isPrimary = slider.getProperties().getWithDefault("knobStyle", "secondary").toString() == "primary";

            // Centre des bounds (qui incluent la marge pour le texte)
            const float cx = x + w * 0.5f;
            const float cy = y + h * 0.5f;

            // RÉCUPÉRER la taille visuelle stockée - C'EST LÀ LE TRUC
            float visualRadius =
                slider.getProperties().getWithDefault("visualRadius", juce::jmin(w, h) * 0.4f); // fallback si pas set

            const float radius = visualRadius;
            const float innerR = radius * 0.78f;

            const float lightAngle = -pi * 0.5f;
            const float shadowAngle = lightAngle + pi;

            // 1. BACKGROUND ARC
            juce::Path arcBg;
            const float arcDist = isPrimary ? radius : radius + 4;
            arcBg.addArc(cx - arcDist, cy - arcDist, arcDist * 2, arcDist * 2, startAngle, endAngle, true);
            g.setColour(colours::perleBlanc);
            g.strokePath(arcBg, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // 2. VALUE ARC
            const float angle = startAngle + sliderPos * (endAngle - startAngle);
            juce::Path arcVal;
            arcVal.addArc(cx - arcDist, cy - arcDist, arcDist * 2, arcDist * 2, startAngle, angle, true);
            g.setColour(colours::violetBleu);
            g.strokePath(arcVal, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

            // 3. DIFFUSE SHADOW
            const float shadowOffsetX = std::cos(shadowAngle) * (innerR * 0.2f);
            const float shadowOffsetY = std::sin(shadowAngle) * (innerR * 0.2f);
            juce::ColourGradient shadowGrad(juce::Colours::black.withAlpha(0.2f), cx + shadowOffsetX, cy + shadowOffsetY,
                juce::Colours::transparentBlack, cx + shadowOffsetX, cy + shadowOffsetY + innerR * 1.5f, true);
            g.setGradientFill(shadowGrad);
            g.fillEllipse(cx - innerR * 1.2f + shadowOffsetX, cy - innerR * 1.2f + shadowOffsetY, innerR * 2.4f, innerR * 2.4f);

            // 4. KNOB BASE
            juce::Colour faceCol = isPrimary ? colours::violetBleu : colours::perleBlanc;
            g.setColour(faceCol);
            g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

            // 5. EDGE FALLOFF
            const float lx = cx + std::cos(lightAngle) * innerR;
            const float ly = cy + std::sin(lightAngle) * innerR;
            const float sx = cx + std::cos(shadowAngle) * innerR;
            const float sy = cy + std::sin(shadowAngle) * innerR;
            juce::ColourGradient falloff(juce::Colours::transparentBlack, lx, ly,
                isPrimary ? juce::Colours::black.withAlpha(0.25f) : juce::Colours::black.withAlpha(0.12f), sx, sy, false);
            g.setGradientFill(falloff);
            g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

            // 6. HIGHLIGHT
            const float hx = cx + std::cos(lightAngle) * innerR;
            const float hy = cy + std::sin(lightAngle) * innerR;
            juce::ColourGradient highlight(
                juce::Colours::white.withAlpha(0.15f), hx, hy, juce::Colours::transparentWhite, cx, cy, true);
            g.setGradientFill(highlight);
            g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

            // 7. CONTOUR
            g.setColour(
                isPrimary ? colours::perleBlanc /* juce::Colours::white.withAlpha(0.6f) */ : colours::grisMoyen.withAlpha(0.75f));
            g.drawEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2, 1.5f);

            // 8. INDICATOR
            const float lineLen = innerR * 0.75f;
            const float lineOff = innerR * 0.25f;
            const float px = cx + std::sin(angle) * (lineOff + lineLen);
            const float py = cy - std::cos(angle) * (lineOff + lineLen);
            const float px0 = cx + std::sin(angle) * lineOff;
            const float py0 = cy - std::cos(angle) * lineOff;
            g.setColour(
                isPrimary ? colours::perleBlanc /* juce::Colours::white.withAlpha(0.6f) */ : colours::violetBleu.withAlpha(0.8f));
            g.drawLine(px0, py0, px, py, 2.f);

            // 9. VALUE
            g.setColour(colours::perleBlanc);
            //g.setColour(isPrimary ? juce::Colours::white.withAlpha(0.8f) : colours::violetBleu);
            g.setFont(valueFont.withHeight(14.0f));

            float angle2h;
            float valDist;

            if(isPrimary)
            {
                angle2h = -pi * 0.25f;
                valDist = radius * 1.6f;
            }
            else
            {
                angle2h = -pi * 0.25f;
                valDist = radius * 1.85f;
            }
            float vx = cx + std::cos(angle2h) * valDist - 6.f;
            float vy = cy + std::sin(angle2h) * valDist;

            juce::String text = slider.getTextFromValue(slider.getValue());
            g.drawText(text, vx - 25, vy - 10, 50, 20, juce::Justification::centred);
        }

        // ─────────────────────────────────────────
        //  BOUTONS — style pill / rectangle arrondi
        // ─────────────────────────────────────────
        /*
        void drawButtonBackground(
            juce::Graphics& g, juce::Button& btn, const juce::Colour& bgColor, bool isHighlighted, bool isDown) override
        {
            auto bounds = btn.getLocalBounds().toFloat().reduced(1.f);
            const bool isOn = btn.getToggleState();

            juce::Colour col = isOn ? juce::Colour(colours::btnActive) : juce::Colour(colours::btnInactive);

            if(isHighlighted)
                col = col.brighter(0.15f);
            if(isDown)
                col = col.darker(0.15f);

            g.setColour(col);
            g.fillRoundedRectangle(bounds, 4.f);

            g.setColour(juce::Colours::black.withAlpha(0.25f));
            g.drawRoundedRectangle(bounds, 4.f, 1.f);
        }

        // ─────────────────────────────────────────
        //  COMBO BOX — style "Menu" barre du haut
        // ─────────────────────────────────────────
        void drawComboBox(juce::Graphics& g, int w, int h, bool isDown, int bx, int by, int bw, int bh,
            juce::ComboBox& box) override
        {
            auto bounds = juce::Rectangle<float>(0, 0, (float)w, (float)h);
            g.setColour(juce::Colour(colours::bgMid));
            g.fillRoundedRectangle(bounds, 4.f);
            g.setColour(juce::Colour(colours::grisMoyen));
            g.drawRoundedRectangle(bounds.reduced(0.5f), 4.f, 1.f);
        }

        // ─────────────────────────────────────────
        //  Helpers publics pour les panels custom
        // ─────────────────────────────────────────
        static void paintPanelDark(juce::Graphics& g, juce::Rectangle<int> bounds)
        {
            g.setColour(juce::Colour(colours::bgDark));
            g.fillRect(bounds);
        }

        static void paintPanelLight(juce::Graphics& g, juce::Rectangle<int> bounds)
        {
            juce::ColourGradient grad(juce::Colour(0xFFD0D0DC), (float)bounds.getX(), (float)bounds.getY(),
                juce::Colour(0xFFB8B8C8), (float)bounds.getX(), (float)bounds.getBottom(), false);
            g.setGradientFill(grad);
            g.fillRect(bounds);
        }

        static void paintSectionLabel(
            juce::Graphics& g, const juce::String& text, juce::Rectangle<int> bounds, bool darkBg = true)
        {
            g.setColour(darkBg ? juce::Colours::white.withAlpha(0.55f) : juce::Colours::black.withAlpha(0.5f));
            g.setFont(juce::Font(11.f, juce::Font::plain));
            g.drawText(text, bounds, juce::Justification::centredBottom);
        }
    */

        juce::Typeface::Ptr getGeistTypeface() const noexcept { return geistTypeface; }
        juce::Typeface::Ptr getCascadaTypeface() const noexcept { return cascadaTypeface; }
        juce::Typeface::Ptr getFunnelTypeface() const noexcept { return funnelTypeface; }
        juce::Typeface::Ptr getFuturaTypeface() const noexcept { return futuraTypeface; }

    private:
        juce::Typeface::Ptr geistTypeface;
        juce::Typeface::Ptr funnelTypeface;
        juce::Typeface::Ptr cascadaTypeface;
        juce::Typeface::Ptr futuraTypeface;
        juce::Font valueFont;
    };
}