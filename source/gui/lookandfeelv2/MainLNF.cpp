#include "MainLNF.h"

#include "../../utils/enum/RotaryType.h"
#include "../../utils/math/EnvelopeMath.h"
#include "../../utils/math/MathConstants.h"
#include "../../utils/math/TraversalMath.h"
#include "BinaryData.h"
#include "Colours.h"

namespace particules
{
    //==============================================================================
    //  STATIC HELPER
    namespace
    {
        void drawLineIndicator(
            juce::Graphics& g, float cx, float cy, float innerR, float sliderPos, float angleOffset, color c, float thickness)
        {
            const float angle = angleOffset + sliderPos * (pi * 1.5f);
            const float startR = innerR * 0.33f;
            const float endR = innerR * 0.90f;

            g.setColour(c);
            g.drawLine(cx + std::sin(angle) * startR, cy - std::cos(angle) * startR, cx + std::sin(angle) * endR,
                cy - std::cos(angle) * endR, thickness);
        }

        void splitValueAndUnit(const str& text, str& valueStr, str& unitStr)
        {
            int splitIdx = -1;
            for(int i = 0; i < text.length(); ++i)
            {
                juce::juce_wchar c = text[i];
                if(!juce::CharacterFunctions::isDigit(c) && c != '.' && c != ',' && c != '-' && c != ' ')
                {
                    splitIdx = i;
                    break;
                }
            }

            if(splitIdx > 0)
            {
                valueStr = text.substring(0, splitIdx).trimEnd();
                unitStr = text.substring(splitIdx).trimStart();
            }
            else
            {
                valueStr = text;
                unitStr = "";
            }
        }
    }

    MainLNF::MainLNF()
    {
        auto loadFont = [](const char* data, int size) {
            return juce::Typeface::createSystemTypefaceFor(
                juce::Span<const std::byte>(reinterpret_cast<const std::byte*>(data), size));
        };

        geistThin = loadFont(BinaryData::GeistThin_ttf, BinaryData::GeistThin_ttfSize);
        geistExtraLight = loadFont(BinaryData::GeistExtraLight_ttf, BinaryData::GeistExtraLight_ttfSize);
        geistLight = loadFont(BinaryData::GeistLight_ttf, BinaryData::GeistLight_ttfSize);
        geistRegular = loadFont(BinaryData::GeistRegular_ttf, BinaryData::GeistRegular_ttfSize);
        geistMedium = loadFont(BinaryData::GeistMedium_ttf, BinaryData::GeistMedium_ttfSize);
        geistSemiBold = loadFont(BinaryData::GeistSemiBold_ttf, BinaryData::GeistSemiBold_ttfSize);
        geistBold = loadFont(BinaryData::GeistBold_ttf, BinaryData::GeistBold_ttfSize);
        geistExtraBold = loadFont(BinaryData::GeistExtraBold_ttf, BinaryData::GeistExtraBold_ttfSize);
        geistBlack = loadFont(BinaryData::GeistBlack_ttf, BinaryData::GeistBlack_ttfSize);

        setDefaultSansSerifTypeface(geistRegular);
    }

    // DISPATCHER
    void MainLNF::drawRotarySlider(
        juce::Graphics& g, int x, int y, int w, int h, float sliderPos, float startAngle, float endAngle, juce::Slider& slider)
    {
        const float cx = x + w * 0.5f;
        const float cy = y + h * 0.5f;
        const float visualRadius = slider.getProperties().getWithDefault("visualRadius", juce::jmin(w, h) * 0.5f);
        const float radius = visualRadius - 3.f;
        const float innerR = radius * 0.88f;

        g.setFont(valueFont);
        const int knobStyle = slider.getProperties().getWithDefault("RotaryType", static_cast<int>(RotaryType::primary));

        switch(knobStyle)
        {
            case static_cast<int>(RotaryType::primary):
                drawPrimaryKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
                break;
            case static_cast<int>(RotaryType::secondary):
                drawSecondaryKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
                break;
            case static_cast<int>(RotaryType::tertiary):
                drawTertiaryKnob(g, cx, cy, visualRadius * 0.7f, visualRadius * 0.62f, startAngle, endAngle, sliderPos, slider);
                break;
            case static_cast<int>(RotaryType::aux):
                drawAuxKnob(g, cx, cy, innerR, startAngle, endAngle, sliderPos);
                break;
            case static_cast<int>(RotaryType::primaryWithAux):
                drawPrimaryWithAuxKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
                break;
            case static_cast<int>(RotaryType::secondaryWithAux):
                drawSecondaryWithAuxKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
                break;
            case static_cast<int>(RotaryType::rotaryMenuWithAux):
                drawRotaryMenuKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
                break;
        }
    }

    //  SINGLE KNOB
    void MainLNF::drawPrimaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider) const
    {
        const float thickness = radius * 0.03f;
        const float arcRadius = radius - thickness * 0.5f;
        const float angle = startAngle + sliderPos * (endAngle - startAngle);

        drawBorderArc(g, cx, cy, arcRadius, startAngle, endAngle);

        updateArcPath(tempArcPath, cx, cy, arcRadius, startAngle, angle);
        //drawArcGlow(g, tempArcPath, colours::violetBleu, thickness);
        drawColoredArc(g, cx, cy, arcRadius, startAngle, angle, colours::violetBleu, thickness);

        drawContour(g, cx, cy, innerR, colours::smokyBlack.brighter(0.20f));
        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawTertiaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider) const
    {
        const float shadowOffset = innerR * 0.15f;
        const float border = 2.0f;

        g.setColour(juce::Colours::black.withAlpha(0.3f));
        g.fillEllipse(cx - innerR + shadowOffset, cy - innerR + shadowOffset * 1.5f, innerR * 2.0f, innerR * 2.0f);

        g.setColour(juce::Colour(0xFF222222));
        g.fillEllipse(cx - innerR - border, cy - innerR - border, innerR * 2.0f + border * 2.0f, innerR * 2.0f + border * 2.0f);

        color faceCol = coloursv2::lightBlack;
        juce::ColourGradient faceGrad(
            faceCol.darker(0.2f), cx, cy - radius * 0.5f, faceCol.brighter(0.03f), cx, cy + radius * 0.5f, false);
        g.setGradientFill(faceGrad);
        g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);

        const float lightAngle = -pi * 0.75f;
        const float shadowAngle = lightAngle + pi;
        juce::ColourGradient falloff(juce::Colours::black.withAlpha(0.2f), cx + std::cos(lightAngle) * innerR,
            cy + std::sin(lightAngle) * innerR, juce::Colours::transparentBlack, cx + std::cos(shadowAngle) * innerR,
            cy + std::sin(shadowAngle) * innerR, false);
        g.setGradientFill(falloff);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

        drawLineIndicator(g, cx, cy, innerR, sliderPos, -pi * 0.75f, juce::Colours::white.withAlpha(0.9f), 2.0f);
    }

    void MainLNF::drawSecondaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider) const
    {
        const float angle = startAngle + sliderPos * (endAngle - startAngle);
        const float thickness = radius * 0.03f;

        updateArcPath(tempArcPath, cx, cy, innerR, startAngle, angle);
        //drawArcGlow(g, tempArcPath, colours::violetBleu, radius * 0.05f, 8, 12.0f, 0.1f);
        drawColoredArc(g, cx, cy, innerR, startAngle, angle, colours::violetBleu, thickness);

        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    // KNOBS + AUX
    void MainLNF::drawPrimaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider) const
    {
        const float thickness = radius * 0.03f;
        const float arcRadius = radius - thickness * 0.5f;
        const float angle = startAngle + sliderPos * (endAngle - startAngle);

        drawBorderArc(g, cx, cy, arcRadius, startAngle, endAngle);

        const float jitterAmount = slider.getProperties().getWithDefault("auxAmount", 0.0f);
        if(jitterAmount > 0.001f)
            drawJitterArc(g, cx, cy, arcRadius, startAngle, endAngle, angle, jitterAmount, 2.f);

        updateArcPath(tempArcPath, cx, cy, arcRadius, startAngle, angle);
        drawColoredArc(g, cx, cy, arcRadius, startAngle, angle, coloursv2::cyan, 2.f);

        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawSecondaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider) const
    {
        const float thickness = radius * 0.03f;
        const float arcRadius = radius - (thickness) * 0.5f;
        const float angle = startAngle + sliderPos * (endAngle - startAngle);

        drawBorderArc(g, cx, cy, arcRadius, startAngle, endAngle);

        const float jitterAmount = slider.getProperties().getWithDefault("auxAmount", 0.0f);
        if(jitterAmount > 0.001f)
            drawJitterArc(g, cx, cy, innerR, startAngle, endAngle, angle, jitterAmount, 2.f);

        updateArcPath(tempArcPath, cx, cy, innerR, startAngle, angle);
        //drawArcGlow(g, tempArcPath, colours::violetBleu, radius * 0.05f, 8, 12.0f, 0.1f);
        drawColoredArc(g, cx, cy, innerR, startAngle, angle, colours::violetBleu, 2.f);

        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawRotaryMenuKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider) const
    {
        const float thickness = radius * 0.03f;
        const float arcRadius = radius - thickness * 0.5f;
        const float angle = startAngle + sliderPos * (endAngle - startAngle);

        drawBorderArc(g, cx, cy, arcRadius, startAngle, endAngle, 0.4f, thickness);

        const float jitterAmount = slider.getProperties().getWithDefault("auxAmount", 0.0f);
        if(jitterAmount > 0.001f)
            drawJitterArc(g, cx, cy, arcRadius, startAngle, endAngle, angle, jitterAmount, thickness);

        updateArcPath(tempArcPath, cx, cy, arcRadius, startAngle, angle);
        drawColoredArc(g, cx, cy, arcRadius, startAngle, angle, coloursv2::cyan, thickness);

        juce::String type = slider.getProperties().getWithDefault("menuControlType", "");
        int mode = slider.getProperties().getWithDefault("currentMode", 0);
        float realValue = slider.getValue();

        const float curvePadding = innerR * 0.35f;
        const float curveW = (innerR - curvePadding) * 1.333f;
        const float curveH = curveW;
        const float startX = cx - (curveW * 0.5f);
        const float startY = cy - (curveH * 0.5f);

        tempMenuCurve.clear();
        const int numPoints = 60;

        for(int i = 0; i <= numPoints; ++i)
        {
            float phase = static_cast<float>(i) / numPoints;
            float val = 0.0f;

            if(type == "envelope")
            {
                val = gui::evaluateEnvelope(static_cast<EnvelopeMode>(mode), phase, realValue);
            }
            else if(type == "traversal")
            {
                val = gui::evaluateTraversal(static_cast<TraversalMode>(mode), phase, realValue);
            }

            val = juce::jlimit(0.0f, 1.0f, val);

            float px = startX + (phase * curveW);
            float py = startY + curveH * (1.0f - val);

            if(i == 0)
                tempMenuCurve.startNewSubPath(px, py);
            else
                tempMenuCurve.lineTo(px, py);
        }

        g.setColour(coloursv2::cyan);
        g.strokePath(tempMenuCurve, juce::PathStrokeType(2.0f, juce::PathStrokeType::mitered, juce::PathStrokeType::rounded));
    }

    void MainLNF::drawAuxKnob(
        juce::Graphics& g, float cx, float cy, float innerR, float startAngle, float endAngle, float sliderPos) const
    {
        drawContour(g, cx, cy, innerR, colours::grisMoyen.withAlpha(0.8f), innerR * 0.15f);
        drawLineIndicator(g, cx, cy, innerR, sliderPos, -pi * 0.75f,
            coloursv2::perleBlanc /* juce::Colours::white.withAlpha(0.9f) */, innerR * 0.15f);
    }

    void MainLNF::drawJitterArc(juce::Graphics& g, float cx, float cy, float baseArcRadius, float startAngle, float endAngle,
        float primaryAngle, float jitterAmount, float baseLineWidth) const
    {
        const float totalRange = endAngle - startAngle;
        const float maxSpread = totalRange * 0.5f;
        const float currentSpread = jitterAmount * maxSpread;

        if(currentSpread < 0.00001f)
            return;

        float jitterCenter = primaryAngle;

        if(primaryAngle - currentSpread < startAngle)
        {
            jitterCenter = startAngle + currentSpread;
        }
        else if(primaryAngle + currentSpread > endAngle)
        {
            jitterCenter = endAngle - currentSpread;
        }

        const float jitterStart = jitterCenter - currentSpread;
        const float jitterEnd = jitterCenter + currentSpread;

        const float jitterArcRadius = baseArcRadius * 1.125f;

        updateArcPath(tempArcPath, cx, cy, jitterArcRadius, jitterStart, jitterEnd);

        //drawArcGlow(g, tempArcPath, coloursv2::yellow, baseLineWidth, 8, 7.5f, 0.05f);
        drawColoredArc(g, cx, cy, jitterArcRadius, jitterStart, jitterEnd, colours::lavender /*, baseLineWidth * 1.2f*/);
    }

    void MainLNF::drawRotarySliderCenteredText(juce::Graphics& g, juce::Slider& slider, float cx, float cy, float radius) const
    {
        const bool isHovered = slider.isMouseOverOrDragging();
        str text = slider.getTextFromValue(slider.getValue());

        if(text.isEmpty())
        {
            text = juce::String(slider.getValue(), 2); // 2 décimales
        }

        str valueStr, unitStr;
        splitValueAndUnit(text, valueStr, unitStr);

        const float valueFontSize = radius * 0.4f;
        const float unitFontSize = radius * 0.3f;
        const float spacing = radius * 0.05f;

        color textColor = isHovered ? coloursv2::white : coloursv2::white.withAlpha(0.85f);
        g.setColour(textColor);

        if(unitStr.isNotEmpty())
        {
            const float totalHeight = valueFontSize + unitFontSize + spacing;
            float yPos = cy - totalHeight * 0.5f;

            g.setFont(juce::Font(geistRegular).withHeight(valueFontSize));
            g.drawText(valueStr, cx - radius * 0.8f, yPos, radius * 1.6f, valueFontSize, juce::Justification::centred, false);

            yPos += valueFontSize + spacing;
            g.setFont(juce::Font(geistRegular).withHeight(unitFontSize));
            g.drawText(unitStr, cx - radius * 0.8f, yPos, radius * 1.6f, unitFontSize, juce::Justification::centred, false);
        }
        else
        {
            g.setFont(juce::Font(geistRegular).withHeight(valueFontSize));
            g.drawText(valueStr, cx - radius * 0.8f, cy - valueFontSize * 0.5f, radius * 1.6f, valueFontSize,
                juce::Justification::centred, false);
        }
    }

    void MainLNF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minPos,
        float maxPos, const juce::Slider::SliderStyle style, juce::Slider& slider)
    {
        if(style == juce::Slider::LinearBarVertical || style == juce::Slider::LinearVertical)
        {
            juce::Rectangle<float> bounds = juce::Rectangle<int>(x, y, width, height).toFloat();

            const float textHeight = 35.0f;
            juce::Rectangle<float> trackBounds = bounds.withTrimmedBottom(textHeight);

            float proportion = 1.0f - ((sliderPos - bounds.getY()) / bounds.getHeight());
            float mappedSliderPos = trackBounds.getBottom() - proportion * trackBounds.getHeight();

            const float barW = juce::jmin(14.0f, trackBounds.getWidth());
            juce::Rectangle<float> barBounds = trackBounds.withSizeKeepingCentre(barW, trackBounds.getHeight());
            const float corner = juce::jmin(4.0f, barW * 0.5f);

            g.setColour(coloursv2::deepBlack);
            g.fillRoundedRectangle(barBounds, corner);

            float fillHeight = barBounds.getBottom() - mappedSliderPos;

            if(fillHeight > 0.1f)
            {
                juce::Rectangle<float> fillRect(barBounds.getX(), mappedSliderPos, barW, fillHeight);
                g.setColour(colours::grisAnthracite);
                g.fillRect(fillRect);

                g.setColour(coloursv2::perleBlanc);
                g.fillRect(barBounds.getX(), mappedSliderPos, barW, 2.0f);
            }

            str text = slider.getTextFromValue(slider.getValue());
            if(text.isEmpty())
                text = str(slider.getValue(), 2);

            str valueStr, unitStr;
            splitValueAndUnit(text, valueStr, unitStr);

            juce::Rectangle<float> textArea = bounds.withTop(trackBounds.getBottom());
            const float valueFontSize = 12.0f;
            const float unitFontSize = 10.0f;

            g.setColour(coloursv2::perleBlanc);

            if(unitStr.isNotEmpty())
            {
                g.setFont(juce::Font(geistBold).withHeight(valueFontSize));
                g.drawText(valueStr, textArea.withTrimmedBottom(textArea.getHeight() * 0.45f), juce::Justification::centredBottom,
                    false);

                g.setFont(juce::Font(geistLight).withHeight(unitFontSize));
                g.setColour(coloursv2::perleBlanc.withAlpha(0.6f));
                g.drawText(
                    unitStr, textArea.withTrimmedTop(textArea.getHeight() * 0.55f), juce::Justification::centredTop, false);
            }
            else
            {
                g.setFont(juce::Font(geistBold).withHeight(valueFontSize));
                g.drawText(valueStr, textArea, juce::Justification::centred, false);
            }

            return;
        }

        if(style == juce::Slider::LinearHorizontal)
        {
            auto bounds = juce::Rectangle<float>(x, y, width, height);
            const float trackHeight = juce::jmax(6.0f, bounds.getHeight() * 0.35f);
            const float cy = bounds.getCentreY();
            const float handleSize = trackHeight * 0.8f;
            const float handleRadius = handleSize * 0.5f;

            juce::Rectangle<float> track(
                bounds.getX() - handleRadius, cy - trackHeight * 0.5f, bounds.getWidth() + handleRadius * 2.0f, trackHeight);

            g.setColour(coloursv2::deepBlack);
            g.fillRoundedRectangle(track, trackHeight * 0.5f);

            const float minX = bounds.getX() + handleRadius;
            const float maxX = bounds.getRight() - handleRadius;
            const float slideX = juce::jlimit(minX, maxX, sliderPos);

            g.setColour(coloursv2::perleBlanc);
            g.fillEllipse(slideX - handleRadius, cy - handleRadius, handleSize, handleSize);
        }
    }

    void MainLNF::updateArcPath(juce::Path& path, float cx, float cy, float radius, float startAngle, float endAngle) const
    {
        path.clear();
        path.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, startAngle, endAngle, true);
    }

    void MainLNF::drawColoredArc(
        juce::Graphics& g, float cx, float cy, float radius, float startAngle, float endAngle, color color, float thickness) const
    {
        if(std::abs(endAngle - startAngle) < 0.001f)
            return;
        updateArcPath(tempArcPath, cx, cy, radius, startAngle, endAngle);
        g.setColour(color);
        g.strokePath(tempArcPath, juce::PathStrokeType(thickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    void MainLNF::drawFillBackground(juce::Graphics& g, float cx, float cy, float innerR, color col) const
    {
        g.setColour(col);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f * 2.0f, innerR * 2.0f);
    }

    void MainLNF::drawBorderArc(
        juce::Graphics& g, float cx, float cy, float radius, float startAngle, float endAngle, float alpha, float thickness) const
    {
        drawColoredArc(g, cx, cy, radius, startAngle, endAngle, colours::grisMoyen.withAlpha(alpha), thickness);
    }

    void MainLNF::drawContour(juce::Graphics& g, float cx, float cy, float radius, color c, float thickness) const
    {
        g.setColour(c);
        g.drawEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, thickness);
    }

    // TEXT BUTTONS & TABS
    void MainLNF::drawButtonBackground(
        juce::Graphics& g, juce::Button& button, const color& backgroundColour, bool isMouseOverButton, bool isButtonDown)
    {
        juce::Rectangle<float> bounds = button.getLocalBounds().toFloat();
        const bool isActive = button.getToggleState();

        if(button.getRadioGroupId() != 0)
        {
            juce::Colour bgCol = isActive ? coloursv2::deepBlack : coloursv2::deepBlack.darker(0.2f);

            // Slight hover effect for inactive tabs
            if(isMouseOverButton && !isActive)
                bgCol = bgCol.brighter(0.05f);

            g.setColour(bgCol);
            g.fillRect(bounds);

            if(isActive)
            {
                g.setColour(colours::lavender);
                g.fillRect(bounds.removeFromTop(2.0f));
            }
            else
            {
                // Shadow line at the bottom of inactive tabs to separate them from the panel
                //g.setColour(juce::Colours::black.withAlpha(0.4f));
                //g.fillRect(bounds.removeFromBottom(1.0f));

                // Subtle separator line between inactive tabs
                //g.setColour(juce::Colours::black.withAlpha(0.2f));
                //g.fillRect(bounds.removeFromRight(1.0f));
            }
        }
        else
        {
            // STANDARD BUTTON STYLE
            color col = backgroundColour;

            if(isButtonDown)
                col = col.darker(0.2f);
            else if(isMouseOverButton)
                col = col.brighter(0.1f);

            g.setColour(col);
            g.fillRoundedRectangle(bounds, 4.0f);

            g.setColour(juce::Colours::white.withAlpha(0.2f));
            g.drawRoundedRectangle(bounds.reduced(0.5f), 4.0f, 1.0f);
        }
    }

    void MainLNF::drawButtonText(juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown)
    {
        const bool isActive = button.getToggleState();
        const bool isTab = (button.getRadioGroupId() != 0) || button.getProperties().contains("isTab");

        if(isTab)
        {
            g.setFont(juce::Font(geistMedium).withHeight(16.0f).withExtraKerningFactor(0.05f));
            juce::Colour textCol = isActive ? juce::Colours::white : juce::Colours::white.withAlpha(0.7f);

            if(isMouseOverButton && !isActive)
                textCol = textCol.withAlpha(0.7f);

            g.setColour(textCol);
            g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
        }
        else
        {
            g.setFont(juce::Font(geistMedium).withHeight(14.0f));
            g.setColour(button.findColour(juce::TextButton::textColourOffId));
            g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred, true);
        }
    }

    juce::Font MainLNF::getLabelFont(juce::Label& label)
    {
        if(label.getProperties().contains("isTitle"))
            return juce::Font(geistSemiBold).withHeight(18.0f).withExtraKerningFactor(0.1f);

        if(label.getProperties().contains("isTab"))
            return juce::Font(geistMedium).withHeight(17.0f).withExtraKerningFactor(0.05f);

        if(label.getProperties().contains("isName"))
            return juce::Font(geistRegular).withHeight(16.0f).withExtraKerningFactor(0.05f);

        if(label.getProperties().contains("isValue"))
            return juce::Font(geistLight).withHeight(16.0f);

        if(label.getProperties().contains("isText"))
            return juce::Font(geistLight).withHeight(14.0f);

        if(label.getProperties().contains("isVersion"))
            return juce::Font(geistLight).withHeight(14.0f);

        return juce::Font(geistLight).withHeight(10.0f);
    }

    // POPUP MENU STYLE
    int MainLNF::getPopupMenuBorderSize() { return 1; }

    void MainLNF::getIdealPopupMenuItemSize(
        const str& text, bool isSeparator, int standardMenuItemHeight, int& idealWidth, int& idealHeight)
    {
        if(isSeparator)
        {
            idealWidth = 50;
            idealHeight = 5;
            return;
        }
        idealWidth = 60;
        idealHeight = 40;
    }

    void MainLNF::drawPopupMenuBackground(juce::Graphics& g, int width, int height)
    {
        g.setColour(coloursv2::deepBlack);
        g.fillRect(0, 0, width, height);
        g.setColour(coloursv2::deepBlack);
        g.drawRect(0, 0, width, height, 1);
    }

    void MainLNF::drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive,
        bool isHighlighted, bool isTicked, bool hasSubMenu, const str& text, const str& shortcutKeyText,
        const juce::Drawable* icon, const juce::Colour* textColour)
    {
        if(isSeparator)
            return;

        // on Hover effect
        if(isHighlighted && isActive)
        {
            g.setColour(coloursv2::lightBlack);
            g.fillRect(area);
        }

        if(icon != nullptr)
            icon->drawWithin(g, area.toFloat(), juce::RectanglePlacement::centred, 1.0f);

        else if(text.isNotEmpty())
        {
            g.setColour(isHighlighted ? coloursv2::cyan : coloursv2::perleBlanc);
            g.setFont(14.0f);
            g.drawText(text, area.reduced(10, 0), juce::Justification::centredLeft, true);
        }
    }
}

/*
    void MainLNF::drawArcGlow(
        juce::Graphics& g, const juce::Path& path, color c, float width, int steps, float spreadMultiplier, float alphaBase) const
    {
        const float glowSpread = width * spreadMultiplier;
        for(int i = steps; i > 0; --i)
        {
            const float t = static_cast<float>(i) / steps;
            const float currentWidth = width + (glowSpread * t);
            const float alpha = alphaBase * std::pow(1.0f - t, 2.0f);
            g.setColour(c.withAlpha(alpha));
            g.strokePath(path, juce::PathStrokeType(currentWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
    }
    */