#include "MainLNF.h"

#include "../../framework/GuiTypes.h"
#include "../../utils/enum/RotaryType.h"
#include "../../utils/math/MathConstants.h"
#include "BinaryData.h"
#include "Colours.h"

namespace particules
{
    MainLNF::MainLNF()
    {
        // 1. Charger le Typeface depuis le binaire (ex: Geist-Medium.ttf)
        // Note : Le nom dans BinaryData correspond au nom du fichier avec des underscores
        geistTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::GeistLight_ttf, BinaryData::GeistLight_ttfSize);
        cascadaTypeface =
            juce::Typeface::createSystemTypefaceFor(BinaryData::CascadiaMonoLight_otf, BinaryData::CascadiaMonoLight_otfSize);
        funnelTypeface =
            juce::Typeface::createSystemTypefaceFor(BinaryData::FunnelDisplayLight_ttf, BinaryData::FunnelDisplayLight_ttfSize);
        futuraTypeface = juce::Typeface::createSystemTypefaceFor(
            BinaryData::futura_medium_condensed_bt_ttf, BinaryData::futura_medium_condensed_bt_ttfSize);

        // 2. Initialiser la Font avec ce Typeface
        valueFont = juce::Font(geistTypeface).withHeight(13.0f);
    }

    void MainLNF::drawRotarySlider(
        juce::Graphics& g, int x, int y, int w, int h, float sliderPos, float startAngle, float endAngle, juce::Slider& slider)
    {
        const float cx = x + w * 0.5f;
        const float cy = y + h * 0.5f;
        float visualRadius = slider.getProperties().getWithDefault("visualRadius", juce::jmin(w, h) * 0.4f);
        const float radius = visualRadius;
        const float innerR = radius * 0.88f;

        g.setFont(valueFont);
        const int knobStyle = slider.getProperties().getWithDefault("knobStyle", static_cast<int>(RotaryType::primary));

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
        }
    }

    void MainLNF::drawSecondaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        radius *= 0.95f;
        innerR *= 0.95f;

        const float baseLineWidth = radius * 0.03f;
        const float baseArcRadius = radius - baseLineWidth * 0.5f;
        // BACKGROUND
        g.setColour(coloursv2::blackest);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

        // Background arc
        drawBackgroundArc(g, cx, cy, baseArcRadius, startAngle, endAngle, 0.1f);

        const float angle = startAngle + sliderPos * (endAngle - startAngle);

        // 2. JITTER ARC using the helper
        float jitterAmount = slider.getProperties().getWithDefault("auxAmount", 0.0f);
        if(jitterAmount > 0.001f)
        {
            float jitterArcRadius = innerR;
            drawJitterArc(g, cx, cy, jitterArcRadius, startAngle, endAngle, angle, jitterAmount, 1.5f);
        }

        // 1. SECONDARY VALUE ARC
        auto arcVal = createArcPath(cx, cy, innerR, startAngle, angle);

        // Glow for secondary
        const float valueLineWidth = radius * 0.05f;
        drawArcGlow(g, arcVal, colours::violetBleu, valueLineWidth, 8, 12.0f, 0.1f);

        // Core arc
        g.setColour(colours::violetBleu);
        g.strokePath(arcVal, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 3. TEXT
        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawPrimaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        const float valueLineWidth = radius * 0.05f;
        const float arcRadius = radius - valueLineWidth * 0.5f;
        const float primaryAngle = startAngle + sliderPos * (endAngle - startAngle);

        // BACKGROUND
        g.setColour(coloursv2::blackest);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

        // Background arc
        drawBackgroundArc(g, cx, cy, arcRadius, startAngle, endAngle);

        // Value arc with glow
        auto arcVal = createArcPath(cx, cy, arcRadius, startAngle, primaryAngle);
        drawArcGlow(g, arcVal, colours::violetBleu, valueLineWidth);

        g.setColour(colours::violetBleu);
        g.strokePath(arcVal, juce::PathStrokeType(valueLineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Contour
        drawContour(g, cx, cy, innerR, colours::smokyBlack.brighter(0.20f));

        // Text
        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawPrimaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        // First draw the standard primary knob (background arc, value arc, contour, text)
        // Note: We use slightly different line width for the base arc in this variant
        const float baseLineWidth = radius * 0.03f;
        const float baseArcRadius = radius - baseLineWidth * 0.5f;
        const float primaryAngle = startAngle + sliderPos * (endAngle - startAngle);

        // BACKGROUND
        g.setColour(coloursv2::blackest);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

        // Background arc
        drawBackgroundArc(g, cx, cy, baseArcRadius, startAngle, endAngle, 0.1f);

        // Jitter arc
        const float jitterAmount = slider.getProperties().getWithDefault("auxAmount", 0.0f);
        if(jitterAmount > 0.001f)
        {
            drawJitterArc(g, cx, cy, baseArcRadius, startAngle, endAngle, primaryAngle, jitterAmount, baseLineWidth);
        }

        // Primary value arc with glow (thinner line)
        juce::Path arcVal = createArcPath(cx, cy, baseArcRadius, startAngle, primaryAngle);
        drawArcGlow(g, arcVal, colours::violetBleu, baseLineWidth);

        g.setColour(coloursv2::cyan);
        g.strokePath(arcVal, juce::PathStrokeType(baseLineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // Contour and text (same as standard primary)
        drawContour(g, cx, cy, innerR, colours::smokyBlack.brighter(0.20f));
        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawJitterArc(juce::Graphics& g, float cx, float cy, float baseArcRadius, float startAngle, float endAngle,
        float primaryAngle, float jitterAmount, float baseLineWidth)
    {
        const float totalRange = endAngle - startAngle;
        const float maxSpread = totalRange * 0.5f;
        const float currentSpread = jitterAmount * maxSpread;
        const float jitterStart = juce::jmax(startAngle, primaryAngle - currentSpread);
        const float jitterEnd = juce::jmin(endAngle, primaryAngle + currentSpread);

        if(jitterEnd <= jitterStart)
            return;

        const float jitterArcRadius = baseArcRadius * 2.0f - (baseArcRadius * 0.88f);
        juce::Path jitterArc = createArcPath(cx, cy, jitterArcRadius, jitterStart, jitterEnd);

        // Jitter glow
        drawArcGlow(g, jitterArc, coloursv2::yellow, baseLineWidth * 1.2f, 8, 10.0f, 0.05f);

        // Jitter core
        const float alpha = 0.5f + (jitterAmount * 0.3f);
        g.setColour(coloursv2::yellow.withAlpha(alpha));
        g.strokePath(
            jitterArc, juce::PathStrokeType(baseLineWidth * 1.2f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    void MainLNF::drawSecondaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        const float angle = startAngle + sliderPos * (endAngle - startAngle);
        juce::Path arcVal = createArcPath(cx, cy, innerR, startAngle, angle);

        // Glow for secondary
        const float valueLineWidth = radius * 0.05f;
        drawArcGlow(g, arcVal, colours::violetBleu, valueLineWidth, 8, 12.0f, 0.1f);

        g.setColour(colours::violetBleu);
        g.strokePath(arcVal, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawAuxKnob(
        juce::Graphics& g, float cx, float cy, float innerR, float startAngle, float endAngle, float sliderPos)
    {
        // BACKGROUND
        g.setColour(coloursv2::blackest);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

        // Border/background
        float border = 2.0f;
        g.setColour(juce::Colour(0xFF222222));
        g.fillEllipse(cx - innerR - border, cy - innerR - border, innerR * 2.0f + border * 2.0f, innerR * 2.0f + border * 2.0f);

        // Indicator line
        const float angle = -pi * 0.5f + sliderPos * (pi * 1.5f);
        const float startR = innerR * 0.33f;
        const float endR = innerR * 0.90f;

        const float startX = cx + std::sin(angle) * startR;
        const float startY = cy - std::cos(angle) * startR;
        const float endX = cx + std::sin(angle) * endR;
        const float endY = cy - std::cos(angle) * endR;

        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.drawLine(startX, startY, endX, endY, 2.0f);
    }

    void MainLNF::drawTertiaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        // Shadow
        const float shadowOffset = innerR * 0.15f;
        g.setColour(juce::Colours::black.withAlpha(0.3f));
        g.fillEllipse(cx - innerR + shadowOffset, cy - innerR + shadowOffset * 1.5f, innerR * 2.0f, innerR * 2.0f);

        // Border
        float border = 2.0f;
        g.setColour(juce::Colour(0xFF222222));
        g.fillEllipse(cx - innerR - border, cy - innerR - border, innerR * 2.0f + border * 2.0f, innerR * 2.0f + border * 2.0f);

        // Face
        fillKnobFace(g, cx, cy, innerR);

        // Edge falloff gradient
        const float lightAngle = -pi * 0.75f;
        const float shadowAngle = lightAngle + pi;
        const float lx = cx + std::cos(lightAngle) * innerR;
        const float ly = cy + std::sin(lightAngle) * innerR;
        const float sx = cx + std::cos(shadowAngle) * innerR;
        const float sy = cy + std::sin(shadowAngle) * innerR;

        juce::ColourGradient falloff(
            juce::Colours::black.withAlpha(0.2f), lx, ly, juce::Colours::transparentBlack, sx, sy, false);
        g.setGradientFill(falloff);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2.0f, innerR * 2.0f);

        // Indicator (same math as drawAuxKnob)
        const float angle = -pi * 0.5f + sliderPos * (pi * 1.5f);
        const float startR = innerR * 0.33f;
        const float endR = innerR * 0.90f;

        const float startX = cx + std::sin(angle) * startR;
        const float startY = cy - std::cos(angle) * startR;
        const float endX = cx + std::sin(angle) * endR;
        const float endY = cy - std::cos(angle) * endR;

        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.drawLine(startX, startY, endX, endY, 2.0f);
    }

    void MainLNF::drawRotarySliderCenteredText(juce::Graphics& g, juce::Slider& slider, float cx, float cy, float radius)
    {
        const bool isHovered = slider.isMouseOverOrDragging();
        juce::String text = slider.getTextFromValue(slider.getValue());

        // Split value and unit
        juce::String valueStr, unitStr;
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
        }

        const float valueFontSize = radius * 0.4f;
        const float unitFontSize = radius * 0.25f;
        const float spacing = radius * 0.05f;

        auto textColor = isHovered ? slider.findColour(juce::Slider::textBoxTextColourId)
                                   : slider.findColour(juce::Slider::textBoxTextColourId).withAlpha(0.7f);
        g.setColour(textColor);

        if(unitStr.isNotEmpty())
        {
            const float totalHeight = valueFontSize + unitFontSize + spacing;
            float yPos = cy - totalHeight * 0.5f;

            g.setFont(valueFontSize);
            g.drawText(valueStr, cx - radius * 0.8f, yPos, radius * 1.6f, valueFontSize, juce::Justification::centred, false);

            yPos += valueFontSize + spacing;
            g.setFont(juce::Font(unitFontSize));
            g.drawText(unitStr, cx - radius * 0.8f, yPos, radius * 1.6f, unitFontSize, juce::Justification::centred, false);
        }
        else
        {
            g.setFont(valueFontSize);
            g.drawText(valueStr, cx - radius * 0.8f, cy - valueFontSize * 0.5f, radius * 1.6f, valueFontSize,
                juce::Justification::centred, false);
        }
    }

    void MainLNF::drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos,
        float maxSliderPos, const juce::Slider::SliderStyle style, juce::Slider& slider)
    {
        if(style != juce::Slider::LinearHorizontal)
            return;

        auto bounds = juce::Rectangle<float>(x, y, width, height);
        const float trackHeight = juce::jmax(6.0f, bounds.getHeight() * 0.35f);
        const float cy = bounds.getCentreY();
        const float handleSize = trackHeight * 0.8f;
        const float handleRadius = handleSize * 0.5f;

        juce::Rectangle<float> track(
            bounds.getX() - handleRadius, cy - trackHeight * 0.5f, bounds.getWidth() + handleRadius * 2.0f, trackHeight);

        g.setColour(coloursv2::blackest);
        g.fillRoundedRectangle(track, trackHeight * 0.5f);

        const float minX = bounds.getX() + handleRadius;
        const float maxX = bounds.getRight() - handleRadius;
        const float cx = juce::jlimit(minX, maxX, sliderPos);

        g.setColour(colours::perleBlanc);
        g.fillEllipse(cx - handleRadius, cy - handleRadius, handleSize, handleSize);
    }

    // Helpers
    juce::Path MainLNF::createArcPath(float cx, float cy, float radius, float startAngle, float endAngle)
    {
        juce::Path path;
        path.addArc(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, startAngle, endAngle, true);
        return path;
    }

    void MainLNF::drawArcGlow(juce::Graphics& g, const juce::Path& path, juce::Colour color, float baseWidth, int steps,
        float spreadMultiplier, float alphaBase)
    {
        const float glowSpread = baseWidth * spreadMultiplier;
        for(int i = steps; i > 0; --i)
        {
            float t = static_cast<float>(i) / steps;
            float currentWidth = baseWidth + (glowSpread * t);
            float alpha = alphaBase * std::pow(1.0f - t, 2.0f);
            g.setColour(color.withAlpha(alpha));
            g.strokePath(path, juce::PathStrokeType(currentWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }
    }

    void MainLNF::drawBackgroundArc(
        juce::Graphics& g, float cx, float cy, float radius, float startAngle, float endAngle, float alpha)
    {
        juce::Path path = createArcPath(cx, cy, radius, startAngle, endAngle);
        g.setColour(colours::grisMoyen.withAlpha(alpha));
        g.strokePath(path, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    void MainLNF::drawContour(juce::Graphics& g, float cx, float cy, float radius, juce::Colour color, float thickness)
    {
        g.setColour(color);
        g.drawEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f, thickness);
    }

    void MainLNF::fillKnobFace(juce::Graphics& g, float cx, float cy, float radius)
    {
        juce::Colour faceCol(0xFF444444);
        juce::ColourGradient faceGrad(
            faceCol.darker(0.2f), cx, cy - radius * 0.5f, faceCol.brighter(0.03f), cx, cy + radius * 0.5f, false);
        g.setGradientFill(faceGrad);
        g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
    }

}

/*
    void MainLNF::drawRotarySlider(
        juce::Graphics& g, int x, int y, int w, int h, float sliderPos, float startAngle, float endAngle, juce::Slider& slider)

    {

        KnobStyle style = KnobStyle::Secondary;
        if(slider.getProperties().contains("knobStyle"))
        {
            juce::String s = slider.getProperties()["knobStyle"].toString();
            if(s == "primary")
                style = KnobStyle::Primary;
            else if(s == "tertiary")
                style = KnobStyle::Tertiary;
        }

        const float cx = x + w * 0.5f;
        const float cy = y + h * 0.5f;
        float visualRadius = slider.getProperties().getWithDefault("visualRadius", juce::jmin(w, h) * 0.4f);
        const float radius = visualRadius;
        const float innerR = radius * 0.78f;

        const float lightAngle = -pi * 0.5f;
        const float shadowAngle = lightAngle + pi;

        if(style == KnobStyle::Primary)
        {
            drawPrimaryKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
        }
        else if(style == KnobStyle::Secondary)
        {
            drawSecondaryKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
        }
        else // Tertiary
        {
            drawTertiaryKnob(g, cx, cy, radius, innerR, sliderPos, slider);
        }
}

void MainLNF::drawPrimaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle, float endAngle,
    float sliderPos, juce::Slider& slider)
{
    // 1. BACKGROUND ARC
    juce::Path arcBg;
    arcBg.addArc(cx - radius, cy - radius, radius * 2, radius * 2, startAngle, endAngle, true);
    g.setColour(colours::perleBlanc);
    g.strokePath(arcBg, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // 2. VALUE ARC
    const float angle = startAngle + sliderPos * (endAngle - startAngle);
    juce::Path arcVal;
    arcVal.addArc(cx - radius, cy - radius, radius * 2, radius * 2, startAngle, angle, true);
    g.setColour(colours::violetBleu);
    g.strokePath(arcVal, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    drawKnobBody(g, cx, cy, innerR, true, colours::violetBleu);
    drawIndicator(g, cx, cy, innerR, angle, true);
    drawValue(g, cx, cy, radius, true, -pi * 0.25f, radius * 1.6f, slider);
}

void MainLNF::drawSecondaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
    float endAngle, float sliderPos, juce::Slider& slider)
{
    // 1. BACKGROUND ARC
    juce::Path arcBg;
    const float arcDist = radius + 4;
    arcBg.addArc(cx - arcDist, cy - arcDist, arcDist * 2, arcDist * 2, startAngle, endAngle, true);
    g.setColour(colours::perleBlanc);
    g.strokePath(arcBg, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // 2. VALUE ARC
    const float angle = startAngle + sliderPos * (endAngle - startAngle);
    juce::Path arcVal;
    arcVal.addArc(cx - arcDist, cy - arcDist, arcDist * 2, arcDist * 2, startAngle, angle, true);
    g.setColour(colours::violetBleu);
    g.strokePath(arcVal, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    drawKnobBody(g, cx, cy, innerR, false, colours::perleBlanc);
    drawIndicator(g, cx, cy, innerR, angle, false);
    drawValue(g, cx, cy, radius, false, -pi * 0.25f, radius * 1.85f, slider);
}
void MainLNF::drawKnobBody(juce::Graphics& g, float cx, float cy, float innerR, bool isPrimary, juce::Colour faceCol)
{
    // 3. DROP SHADOW
    const float shadowAngle = pi * 0.5f;
    const float shadowOffsetX = std::cos(shadowAngle) * (innerR * 0.2f);
    const float shadowOffsetY = std::sin(shadowAngle) * (innerR * 0.2f);
    juce::ColourGradient shadowGrad(juce::Colours::black.withAlpha(0.2f), cx + shadowOffsetX, cy + shadowOffsetY,
        juce::Colours::transparentBlack, cx + shadowOffsetX, cy + shadowOffsetY + innerR * 1.5f, true);
    g.setGradientFill(shadowGrad);
    g.fillEllipse(cx - innerR * 1.2f + shadowOffsetX, cy - innerR * 1.2f + shadowOffsetY, innerR * 2.4f, innerR * 2.4f);

    // 4. FACE
    g.setColour(faceCol);
    g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

    // 5. EDGE FALLOFF
    const float lightAngle = -pi * 0.5f;
    const float lx = cx + std::cos(lightAngle) * innerR;
    const float ly = cy + std::sin(lightAngle) * innerR;
    const float sx = cx + std::cos(shadowAngle) * innerR;
    const float sy = cy + std::sin(shadowAngle) * innerR;
    juce::ColourGradient falloff(
        juce::Colours::transparentBlack, lx, ly, juce::Colours::black.withAlpha(isPrimary ? 0.25f : 0.12f), sx, sy, false);
    g.setGradientFill(falloff);
    g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

    // 6. HIGHLIGHT
    const float hx = cx + std::cos(lightAngle) * innerR;
    const float hy = cy + std::sin(lightAngle) * innerR;
    juce::ColourGradient highlight(juce::Colours::white.withAlpha(0.15f), hx, hy, juce::Colours::transparentWhite, cx, cy, true);
    g.setGradientFill(highlight);
    g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

    // 7. CONTOUR
    g.setColour(isPrimary ? colours::perleBlanc : colours::grisMoyen.withAlpha(0.75f));
    g.drawEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2, 1.5f);

}

void MainLNF::drawIndicator(juce::Graphics& g, float cx, float cy, float innerR, float angle, bool isPrimary)
{
    //8. INDICATOR
    const float lineLen = innerR * 0.75f;
    const float lineOff = innerR * 0.25f;
    const float px = cx + std::sin(angle) * (lineOff + lineLen);
    const float py = cy - std::cos(angle) * (lineOff + lineLen);
    const float px0 = cx + std::sin(angle) * lineOff;
    const float py0 = cy - std::cos(angle) * lineOff;
    g.setColour(isPrimary ? colours::perleBlanc : colours::violetBleu.withAlpha(0.8f));
    g.drawLine(px0, py0, px, py, 2.f);
}

void MainLNF::drawValue(
    juce::Graphics& g, float cx, float cy, float radius, bool isPrimary, float angle, float valDist, juce::Slider& slider)
{
    // 9. VALUE
    g.setColour(colours::perleBlanc);
    g.setFont(valueFont.withHeight(14.0f));
    float vx = cx + std::cos(angle) * valDist - 6.f;
    float vy = cy + std::sin(angle) * valDist;
    juce::String text = slider.getTextFromValue(slider.getValue());
    g.drawText(text, vx - 25, vy - 10, 50, 20, juce::Justification::centred);
}

void MainLNF::drawTertiaryKnob(
    juce::Graphics& g, float cx, float cy, float radius, float innerR, float sliderPos, juce::Slider& slider)
{
    const float angle = -pi * 0.5f + sliderPos * (pi * 1.5f);

    // 1. DIFFUSE SHADOW
    const float shadowOffset = innerR * 0.15f;
    g.setColour(juce::Colours::black.withAlpha(0.3f));
    g.fillEllipse(cx - innerR + shadowOffset, cy - innerR + shadowOffset * 1.5f, innerR * 2, innerR * 2);

    // 2. CONTOUR
    float border = 2.0f;
    g.setColour(juce::Colour(0xFF222222));
    g.fillEllipse(cx - innerR - border, cy - innerR - border, innerR * 2 + border * 2, innerR * 2 + border * 2);

    // 3. FACE DU KNOB
    juce::Colour faceCol(0xFF444444);
    juce::ColourGradient faceGrad(
        faceCol.darker(0.2f), cx, cy - innerR * 0.5f, faceCol.brighter(0.03f), cx, cy + innerR * 0.5f, false);
    g.setGradientFill(faceGrad);
    g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

    // 4. EDGE FALLOFF
    const float lightAngle = -pi * 0.75f;
    const float shadowAngle = lightAngle + pi;
    const float lx = cx + std::cos(lightAngle) * innerR;
    const float ly = cy + std::sin(lightAngle) * innerR;
    const float sx = cx + std::cos(shadowAngle) * innerR;
    const float sy = cy + std::sin(shadowAngle) * innerR;

    juce::ColourGradient falloff(juce::Colours::black.withAlpha(0.2f), lx, ly, juce::Colours::transparentBlack, sx, sy, false);
    g.setGradientFill(falloff);
    g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

    //// 5. HIGHLIGHT
    //juce::ColourGradient highlight(juce::Colours::black.withAlpha(0.1f) ,
    //    cx + std::cos(lightAngle) * innerR * 0.6f,
    //    cy + std::sin(lightAngle) * innerR * 0.6f, juce::Colours::transparentBlack, cx, cy, true);
    //g.setGradientFill(highlight);
    //g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

    // 6. INDICATEUR
    const float startR = innerR * 0.33f; // 85% of radius
    const float endR = innerR * 0.90f; // almost at the edge

    // compute start point
    const float startx = cx + std::sin(angle) * startR;
    const float startY = cy - std::cos(angle) * startR;

    // compute end point
    const float ex = cx + std::sin(angle) * endR;
    const float ey = cy - std::cos(angle) * endR;

    g.setColour(juce::Colours::white.withAlpha(0.9f));
    g.drawLine(startx, startY, ex, ey, 2.f);
}
     */

/*
    
    */

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