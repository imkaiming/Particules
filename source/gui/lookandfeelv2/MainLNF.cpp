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
        valueFont = juce::Font(geistTypeface).withHeight(14.0f);
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

        int knobStyle = slider.getProperties().getWithDefault("knobStyle", static_cast<int>(RotaryType::primary));

        //color drawColour = isHovered ? coloursv2::blackest.brighter(0.05f) : coloursv2::blackest;
        //g.setColour(drawColour);

        g.setColour(coloursv2::blackest);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

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
            {
                const float primaryAngle = slider.getProperties().getWithDefault("primaryAngle", startAngle);
                const float primaryArcRadius = slider.getProperties().getWithDefault("primaryArcRadius", radius * 0.9f);
                drawAuxKnob(g, cx, cy, innerR, startAngle, endAngle, sliderPos);
                break;
            }

            case static_cast<int>(RotaryType::primaryWithAux):
                drawPrimaryWithAuxKnob(g, cx, cy, radius, innerR, startAngle, endAngle, sliderPos, slider);
                break;
        }
    }

    void MainLNF::drawPrimaryWithAuxKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        const float valueLineWidth = radius * 0.03f;
        const float arcRadius = radius - valueLineWidth * 0.5f;
        const float primaryAngle = startAngle + sliderPos * (endAngle - startAngle);

        const color accentColor = colours::violetBleu;
        const color jitterColor = coloursv2::goldenYellow;
        const color contourColor = colours::smokyBlack;

        // 1. Background arc
        juce::Path arcBg;
        arcBg.addArc(cx - arcRadius, cy - arcRadius, arcRadius * 2, arcRadius * 2, startAngle, endAngle, true);
        g.setColour(colours::grisMoyen.withAlpha(0.1f));
        g.strokePath(arcBg, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 2. Primary value
        juce::Path arcVal;
        arcVal.addArc(cx - arcRadius, cy - arcRadius, arcRadius * 2, arcRadius * 2, startAngle, primaryAngle, true);

        // 3. primary glow
        const int glowSteps = 8;
        const float glowSpread = valueLineWidth * 12.0f;
        for(int i = glowSteps; i > 0; --i)
        {
            float t = (float)i / glowSteps;
            float currentWidth = valueLineWidth + (glowSpread * t);
            float alpha = 0.08f * std::pow(1.0f - t, 2.0f);
            g.setColour(accentColor.withAlpha(alpha));
            g.strokePath(arcVal, juce::PathStrokeType(currentWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        g.setColour(accentColor);
        g.strokePath(arcVal, juce::PathStrokeType(valueLineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 3. JITTER ARC Bipolar
        float jitterAmount = slider.getProperties().getWithDefault("auxAmount", 0.0f);
        if(jitterAmount > 0.001f)
        {
            float totalRange = endAngle - startAngle;
            float maxSpread = totalRange * 0.5f;
            float currentSpread = jitterAmount * maxSpread;

            float jitterStart = juce::jmax(startAngle, primaryAngle - currentSpread);
            float jitterEnd = juce::jmin(endAngle, primaryAngle + currentSpread);

            if(jitterEnd > jitterStart)
            {
                float jitterArcRadius = arcRadius * 2.f - innerR;

                juce::Path jitterArc;
                jitterArc.addArc(cx - jitterArcRadius, cy - jitterArcRadius, jitterArcRadius * 2, jitterArcRadius * 2,
                    jitterStart, jitterEnd, true);

                // jitter glow
                const int glowSteps = 8;
                const float glowSpread = valueLineWidth * 10.0f;

                for(int i = glowSteps; i > 0; --i)
                {
                    float t = (float)i / glowSteps;

                    float width = valueLineWidth + glowSpread * t;
                    float alpha = 0.05f * std::pow(1.0f - t, 2.2f);

                    g.setColour(jitterColor.withAlpha(alpha));
                    g.strokePath(
                        jitterArc, juce::PathStrokeType(width, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
                }

                // jitter arc
                float alpha = 0.5f + (jitterAmount * 0.3f);

                g.setColour(jitterColor.withAlpha(alpha));
                g.strokePath(jitterArc,
                    juce::PathStrokeType(valueLineWidth * 1.2f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            }
        }

        // 4. Contour
        g.setColour(contourColor.brighter(0.20f));
        g.drawEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2, 1.5f);

        // 5. Center Text
        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawAuxKnob(
        juce::Graphics& g, float cx, float cy, float innerR, float startAngle, float endAngle, float sliderPos)
    {
        /*
        // 1. Aux outline
        g.setColour(colours::grisMoyen.withAlpha(0.4f));
        g.drawEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2, 1.0f);

        // 2. Calculate indicator angle based on its own sliderPos
        float auxAngle = startAngle + sliderPos * (endAngle - startAngle);

        // 3. Indicator line
        float indicatorLen = innerR * 0.9f;
        float aex = cx + std::cos(auxAngle) * indicatorLen;
        float aey = cy + std::sin(auxAngle) * indicatorLen;

        g.setColour(colours::perleBlanc.withAlpha(0.9f));
        g.drawLine(cx, cy, aex, aey, 1.5f);

        // 4. Indicator dot
        float dotX = cx + std::cos(auxAngle) * (innerR - 1.5f);
        float dotY = cy + std::sin(auxAngle) * (innerR - 1.5f);
        g.fillEllipse(dotX - 1.0f, dotY - 1.0f, 2.0f, 2.0f);
        */
        // CONTOUR
        float border = 2.0f;
        g.setColour(juce::Colour(0xFF222222));
        g.fillEllipse(cx - innerR - border, cy - innerR - border, innerR * 2 + border * 2, innerR * 2 + border * 2);

        // 6. INDICATEUR
        const float startR = innerR * 0.33f;
        const float endR = innerR * 0.90f;

        // start point
        const float angle = -pi * 0.5f + sliderPos * (pi * 1.5f);
        const float startx = cx + std::sin(angle) * startR;
        const float startY = cy - std::cos(angle) * startR;

        // compute end point
        const float ex = cx + std::sin(angle) * endR;
        const float ey = cy - std::cos(angle) * endR;

        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.drawLine(startx, startY, ex, ey, 2.f);
    }

    void MainLNF::drawPrimaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        const float valueLineWidth = radius * 0.05f;
        const float arcRadius = radius - valueLineWidth * 0.5f;
        //const float lineWidth = juce::jmin(1.5f, innerR * 0.05f);
        const float glowWidth = valueLineWidth * 10.f;

        const color accentColor = colours::violetBleu;
        const color contourColor = colours::smokyBlack;

        // 1. BACKGROUND ARC
        juce::Path arcBg;
        arcBg.addArc(cx - arcRadius, cy - arcRadius, arcRadius * 2, arcRadius * 2, startAngle, endAngle, true);
        g.setColour(colours::grisMoyen.withAlpha(0.15f));
        g.strokePath(arcBg, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 2. PRIMARY VALUE ARC PATH
        const float primaryAngle = startAngle + sliderPos * (endAngle - startAngle);
        juce::Path arcVal;
        arcVal.addArc(cx - arcRadius, cy - arcRadius, arcRadius * 2, arcRadius * 2, startAngle, primaryAngle, true);

        // 3. GLOW EFFECT
        const int glowSteps = 8;
        const float glowSpread = valueLineWidth * 12.0f;

        for(int i = glowSteps; i > 0; --i)
        {
            float t = (float)i / glowSteps;
            float currentWidth = valueLineWidth + (glowSpread * t);
            float alpha = 0.09f * std::pow(1.0f - t, 2.0f); // quadratic fall off

            g.setColour(accentColor.withAlpha(alpha));
            g.strokePath(arcVal, juce::PathStrokeType(currentWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // 4. CORE VALUE ARC
        g.setColour(accentColor);
        g.strokePath(arcVal, juce::PathStrokeType(valueLineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 5. CONTOUR
        g.setColour(contourColor.brighter(0.20f));
        g.drawEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2, 1.5f);

        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawSecondaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        color accentColor = colours::violetBleu;
        const float lineWidth = juce::jmin(3.0f, innerR * 0.05f);
        const float arcRadius = radius - lineWidth * 0.5f;
        const float angle = startAngle + sliderPos * (endAngle - startAngle);

        g.setColour(slider.findColour(juce::Slider::rotarySliderFillColourId));
        juce::Path arcVal;
        arcVal.addArc(cx - innerR, cy - innerR, innerR * 2, innerR * 2, startAngle, angle, true);

        const int glowSteps = 8;
        const float valueLineWidth = radius * 0.05f;
        const float glowSpread = valueLineWidth * 12.0f;

        for(int i = glowSteps; i > 0; --i)
        {
            float t = (float)i / glowSteps;
            float currentWidth = valueLineWidth + (glowSpread * t);
            float alpha = 0.1f * std::pow(1.0f - t, 2.0f); // quadratic fall off

            g.setColour(accentColor.withAlpha(alpha));
            g.strokePath(arcVal, juce::PathStrokeType(currentWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // ARC VALUE
        g.setColour(accentColor);
        g.strokePath(arcVal, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        drawRotarySliderCenteredText(g, slider, cx, cy, radius);
    }

    void MainLNF::drawTertiaryKnob(juce::Graphics& g, float cx, float cy, float radius, float innerR, float startAngle,
        float endAngle, float sliderPos, juce::Slider& slider)
    {
        //const float radius = visualRadius * 0.7f;
        //const float innerR = radius * 0.88f;

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

        juce::ColourGradient falloff(
            juce::Colours::black.withAlpha(0.2f), lx, ly, juce::Colours::transparentBlack, sx, sy, false);
        g.setGradientFill(falloff);
        g.fillEllipse(cx - innerR, cy - innerR, innerR * 2, innerR * 2);

        // 6. INDICATEUR
        const float startR = innerR * 0.33f;
        const float endR = innerR * 0.90f;

        // start point
        const float angle = -pi * 0.5f + sliderPos * (pi * 1.5f);
        const float startx = cx + std::sin(angle) * startR;
        const float startY = cy - std::cos(angle) * startR;

        // compute end point
        const float ex = cx + std::sin(angle) * endR;
        const float ey = cy - std::cos(angle) * endR;

        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.drawLine(startx, startY, ex, ey, 2.f);
    }

    void MainLNF::drawRotarySliderCenteredText(juce::Graphics& g, juce::Slider& slider, float cx, float cy, float radius)
    {
        const bool isHovered = slider.isMouseOverOrDragging();

        juce::String text = slider.getTextFromValue(slider.getValue());
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

        g.setColour(isHovered ? slider.findColour(juce::Slider::textBoxTextColourId)
                              : slider.findColour(juce::Slider::textBoxTextColourId).withAlpha(0.7f));

        if(unitStr.isNotEmpty())
        {
            // Stack value above unit, centered as a group
            const float totalHeight = valueFontSize + unitFontSize + spacing;
            float yPos = cy - totalHeight * 0.5f;

            // Value
            g.setFont(valueFontSize);
            g.drawText(valueStr, cx - radius * 0.8f, yPos, radius * 1.6f, valueFontSize, juce::Justification::centred, false);

            // Unit suffix below
            yPos += valueFontSize + spacing;
            g.setFont(juce::Font(unitFontSize));
            g.drawText(unitStr, cx - radius * 0.8f, yPos, radius * 1.6f, unitFontSize, juce::Justification::centred, false);
        }
        else
        {
            // Single value centered vertically
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
        const float trackRadius = trackHeight * 0.5f;

        const float cy = bounds.getCentreY();

        const float handleSize = trackHeight * 0.8f;
        const float handleRadius = handleSize * 0.5f;

        juce::Rectangle<float> logicalTrack(bounds.getX(), cy - trackHeight * 0.5f, bounds.getWidth(), trackHeight);
        juce::Rectangle<float> visualTrack = logicalTrack;
        visualTrack.setX(logicalTrack.getX() - handleRadius);
        visualTrack.setWidth(logicalTrack.getWidth() + handleRadius * 2.0f);

        // draw track
        g.setColour(coloursv2::blackest);
        g.fillRoundedRectangle(visualTrack, trackRadius);

        const float minX = logicalTrack.getX() + handleRadius;
        const float maxX = logicalTrack.getRight() - handleRadius;

        const float cx = juce::jlimit(minX, maxX, sliderPos);

        juce::Rectangle<float> handle(cx - handleRadius, cy - handleRadius, handleSize, handleSize);

        g.setColour(colours::perleBlanc);
        g.fillEllipse(handle);
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