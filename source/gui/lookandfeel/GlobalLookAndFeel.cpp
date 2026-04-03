#include "GlobalLookAndFeel.h"
#include "../../utils/math/MathConstants.h"
#include "BinaryData.h"
#include "MyColours.h"


namespace particules
{

    GlobalLookAndFeel::GlobalLookAndFeel()
    {
        metalKnobImage = juce::ImageFileFormat::loadFrom(BinaryData::metal_knob_02_png, BinaryData::metal_knob_02_pngSize);
        metalKnobImage = metalKnobImage.convertedToFormat(juce::Image::ARGB);

        futuraTypeface = juce::Typeface::createSystemTypefaceFor(BinaryData::FuturaMedium_cpp, BinaryData::FuturaMedium_cppSize);
    }

    void GlobalLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float startAngle, float endAngle, juce::Slider& slider)
    {
        if(metalKnobImage.isNull())
            return;

        // BOUNDS
        juce::Rectangle<float> bounds = juce::Rectangle<float>(x, y, width, height);

        const float imgW = (float)metalKnobImage.getWidth();
        const float imgH = (float)metalKnobImage.getHeight();

        const float scale = juce::jmin(bounds.getWidth() / imgW, bounds.getHeight() / imgH);

        const float drawnW = imgW * scale;
        const float drawnH = imgH * scale;
        const float radius = juce::jmin(drawnW, drawnH) * 0.5f;

        const float cx = bounds.getCentreX();
        const float cy = bounds.getCentreY();

        // ANGLES
        const float lightAngle = -pi * 0.75f; // 11h
        const float shadowAngle = lightAngle + pi; // 5h

        const float baseAngle = startAngle + sliderPos * (endAngle - startAngle);
        const float knobAngle = baseAngle + 0.f;

        g.setImageResamplingQuality(juce::Graphics::highResamplingQuality);

        // 1. BACKGROUND ARC (RANGE TRACK) : OK
        {
            const float arcRadius = radius + 5.0f;
            const float arcThickness = 1.0f;

            juce::Path rangeArc;
            rangeArc.addCentredArc(cx, cy, arcRadius, arcRadius, 0.0f, startAngle, endAngle, true);

            g.setColour(juce::Colours::white.withAlpha(0.4f));
            g.strokePath(rangeArc, juce::PathStrokeType(arcThickness, juce::PathStrokeType::curved, juce::PathStrokeType::butt));
        }

        // 2. DIFFUSE SHADOW : OK
        {
            const float dist = radius * 1.4f;

            const float sx = cx + std::cos(shadowAngle) * dist;
            const float sy = cy + std::sin(shadowAngle) * dist;

            juce::ColourGradient grad(
                juce::Colours::black.withAlpha(0.8f), cx, cy, juce::Colours::transparentBlack, sx, sy, true);
            g.setGradientFill(grad);
            //g.setColour(juce::Colours::red);
            g.fillEllipse(cx - radius * 0.6f, cy - radius * 0.6f, radius * 2.f, radius * 2.f);
        }

        // 3. CONTACT SHADOW : OK
        {
            const float shadowDist = radius * 0.06f;
            const float contactRadius = radius * 0.95f;

            const float csx = cx + std::cos(shadowAngle) * shadowDist;
            const float csy = cy + std::sin(shadowAngle) * shadowDist;

            g.setColour(juce::Colours::black.withAlpha(0.8f));
            g.fillEllipse(csx - contactRadius, csy - contactRadius, contactRadius * 2.0f, contactRadius * 2.0f);
        }

        // 4. MAIN KNOB : OK
        {
            juce::AffineTransform knob = juce::AffineTransform::translation(-imgW * 0.5f, -imgH * 0.5f)
                                             .scaled(scale)
                                             .rotated(knobAngle)
                                             .translated(cx, cy);

            g.drawImageTransformed(metalKnobImage, knob, false);
        }

        // 5. CONTOUR DIRECTIONAL EDGE FALLOFF : OK
        {
            const float lx = cx + std::cos(lightAngle) * radius;
            const float ly = cy + std::sin(lightAngle) * radius;

            const float sx = cx + std::cos(shadowAngle) * radius;
            const float sy = cy + std::sin(shadowAngle) * radius;

            juce::ColourGradient falloff(
                juce::Colours::transparentBlack, lx, ly, juce::Colours::black.withAlpha(0.7f), sx, sy, false);

            g.setGradientFill(falloff);

            g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
        }

        // 6. DIFFUSE HIGHLIGHT (11h) : OK
        {
            const float hx = cx + std::cos(lightAngle) * radius * 0.85f;
            const float hy = cy + std::sin(lightAngle) * radius * 0.85f;

            juce::ColourGradient highlight(
                juce::Colours::white.withAlpha(0.53f), hx, hy, juce::Colours::transparentWhite, cx, cy, true);

            g.setGradientFill(highlight);

            g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
        }

        // 7. SUBTLE EDGE FALLOFF (DEPTH) : OK
        //{
        //    juce::ColourGradient falloff(juce::Colours::transparentBlack, cx, cy, juce::Colours::black.withAlpha(0.31f),
        //        cx - std::cos(lightAngle) * radius, cy - std::sin(lightAngle) * radius, false);

        //    g.setGradientFill(falloff);

        //    g.fillEllipse(cx - radius, cy - radius, radius * 2.0f, radius * 2.0f);
        //}
    }

}

/*
    void GlobalLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
        float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
    {
        const float diameter = static_cast<float>(juce::jmin(width, height)) - 4.0f;
        const float radius = diameter / 2.0f;
        const float centerX = static_cast<float>(x + width / 2.0f);
        const float centerY = static_cast<float>(y + height / 2.0f);
        const float radiusX = centerX - radius;
        const float radiusY = centerY - radius;

        const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        const float lineWidth = juce::jmin(15.0f, radius * 0.1f);
        const float arcRadius = radius - lineWidth * 0.5f;

        g.setColour(colours::black);
        g.fillEllipse(radiusX, radiusY, radius * 2.0f, radius * 2.0f);

        g.setColour(slider.findColour(juce::Slider::rotarySliderOutlineColourId));
        juce::Path outlineArc;
        outlineArc.addCentredArc(centerX, centerY, arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.strokePath(outlineArc, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        g.setColour(colours::jungleGreen);
        juce::Path valueArc;
        valueArc.addCentredArc(centerX, centerY, arcRadius, arcRadius, 0.0f, rotaryStartAngle, angle, true);
        g.strokePath(valueArc, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }*/
/*

void GlobalLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    float startAngle, float endAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(6.0f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto centreX = bounds.getCentreX();
    auto centreY = bounds.getCentreY();

    const float knobRadius = radius * 0.82f; // corps du knob
    const float trackRadius = radius * 0.95f; // piste extérieure
    const float trackThickness = radius * 0.16f;

    const float angle = startAngle + sliderPos * (endAngle - startAngle);

    // 1. Fond extérieur sombre + bordure fine
    g.setColour(juce::Colour(0xFF1A1A1A));
    g.fillEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f);

    g.setColour(juce::Colour(0xFF00F5FF).withAlpha(0.25f));
    g.drawEllipse(centreX - radius, centreY - radius, radius * 2.0f, radius * 2.0f, 1.5f);

    // 2. Piste de fond (track sombre)
    juce::Path backgroundTrack;
    backgroundTrack.addCentredArc(centreX, centreY, trackRadius, trackRadius, 0.0f, startAngle, endAngle, true);
    g.setColour(juce::Colour(0xFF222222));
    g.strokePath(backgroundTrack, juce::PathStrokeType(trackThickness, juce::PathStrokeType::curved));

    // 3. Arc de valeur avec glow neon (le plus important pour le look mockup)
    juce::Path valueTrack;
    valueTrack.addCentredArc(centreX, centreY, trackRadius, trackRadius, 0.0f, startAngle, angle, true);

    // Glow externe
    g.setColour(juce::Colour(0xFF00F5FF).withAlpha(0.45f));
    g.strokePath(valueTrack, juce::PathStrokeType(trackThickness + 6.0f));

    // Arc principal neon cyan
    g.setColour(juce::Colour(0xFF00F5FF));
    g.strokePath(valueTrack, juce::PathStrokeType(trackThickness));

    // 4. Corps du knob métallique (dégradé 3D réaliste)
    juce::ColourGradient metallicGrad =
        juce::ColourGradient::vertical(juce::Colour(0xFFEEEEEE), centreY - knobRadius * 0.85f, // highlight haut
            juce::Colour(0xFF444444), centreY + knobRadius * 0.85f); // ombre bas

    g.setGradientFill(metallicGrad);
    g.fillEllipse(centreX - knobRadius, centreY - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);

    // Bordure intérieure du knob
    g.setColour(juce::Colour(0xFF1F1F1F));
    g.drawEllipse(centreX - knobRadius * 0.92f, centreY - knobRadius * 0.92f, knobRadius * 1.84f, knobRadius * 1.84f, 3.5f);

    // 5. Indicateur (petit trait + pointe brillante)
    float indicatorLen = knobRadius * 0.68f;
    float ix = centreX + std::sin(angle) * indicatorLen;
    float iy = centreY - std::cos(angle) * indicatorLen;

    // Ligne indicateur blanche fine
    g.setColour(juce::Colours::white);
    g.drawLine(centreX, centreY, ix, iy, 3.5f);

    // Pointe ronde brillante
    g.setColour(juce::Colour(0xFFFFFFFF));
    g.fillEllipse(ix - 4.5f, iy - 4.5f, 9.0f, 9.0f);

    // Petite ombre sur la pointe pour plus de profondeur
    g.setColour(juce::Colours::black.withAlpha(0.4f));
    g.fillEllipse(ix - 3.0f, iy - 2.5f, 4.0f, 4.0f);

    // 6. Valeur texte en dessous (comme dans le mockup)
    if(slider.getTextBoxPosition() != juce::Slider::NoTextBox)
    {
        g.setColour(juce::Colours::white.withAlpha(0.9f));
        g.setFont(juce::Font(13.5f, juce::Font::plain));
        g.drawText((str)slider.getValue(), bounds.removeFromBottom(22).reduced(4), juce::Justification::centred, true);
    }
}
    */
