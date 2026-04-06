#include "PrimaryWithAux.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../../utils/math/MathConstants.h"
#include "../../lookandfeelv2/Colours.h"
#include "AuxRotarySlider.h"
#include "RotarySlider.h"

namespace particules
{

    PrimaryWithAux::PrimaryWithAux()
    {
        addAndMakeVisible(primarySlider);
        addAndMakeVisible(auxSlider);

        primarySlider.getProperties().set("knobStyle", static_cast<int>(RotaryType::primaryWithAux));
        auxSlider.getProperties().set("knobStyle", static_cast<int>(RotaryType::primaryWithAux));

        primarySlider.onValueChange = [this]() { primarySlider.repaint(); };
        auxSlider.onValueChange = [this]() {
            syncAuxDataToPrimary();
            primarySlider.repaint();
        };
    }

    void PrimaryWithAux::updatePrimaryAngle()
    {
        const float startAngle = pi * 1.25f;
        const float endAngle = pi * 2.25f;

        float range = primarySlider.getMaximum() - primarySlider.getMinimum();
        float norm = (primarySlider.getValue() - primarySlider.getMinimum()) / range;
        float angle = startAngle + norm * (endAngle - startAngle);

        auxSlider.getProperties().set("primaryAngle", angle);
        auxSlider.repaint();
    }

    void PrimaryWithAux::setRange(float min, float max) noexcept { primarySlider.setRange(min, max); }

    void PrimaryWithAux::setSkewFactorFromMidPoint(float skew) noexcept { primarySlider.setSkewFactorFromMidPoint(skew); }

    std::unique_ptr<ValueTreeState::SliderAttachment> PrimaryWithAux::attachPrimaryToAPVTS(
        ValueTreeState& apvts, const str& id) noexcept
    {
        return std::make_unique<ValueTreeState::SliderAttachment>(apvts, id, primarySlider);
    }

    std::unique_ptr<ValueTreeState::SliderAttachment> PrimaryWithAux::attachAuxToAPVTS(
        ValueTreeState& apvts, const str& id) noexcept
    {
        return std::make_unique<ValueTreeState::SliderAttachment>(apvts, id, auxSlider);
    }

    void PrimaryWithAux::resized()
    {
        auto bounds = getLocalBounds().toFloat();
        auto center = bounds.getCentre();

        const float minDim = juce::jmin(bounds.getWidth(), bounds.getHeight());
        const float maxExtentFromCenter = minDim * 0.5f;

        const float auxSize = juce::jlimit(18.0f, 32.0f, minDim * 0.18f);

        const float jitterRadiusMultiplier = 1.07f;
        const float jitterLineWidthHalf = 1.0f; // Jitter line is 2.0px width
        const float safetyPadding = 3.0f;
        const float gapBetweenJitterAndAux = 4.0f; // Nice visual breathing room

        const float maxAllowedAuxCenterDistance = maxExtentFromCenter - safetyPadding - (auxSize * 0.5f);
        const float availableRadiusForJitter = maxAllowedAuxCenterDistance - gapBetweenJitterAndAux - (auxSize * 0.5f);
        const float primaryVisualRadius =
            juce::jmax(1.0f, (availableRadiusForJitter - jitterLineWidthHalf) / jitterRadiusMultiplier);
        const float jitterOuterEdge = (primaryVisualRadius * jitterRadiusMultiplier) + jitterLineWidthHalf;
        const float auxDistance = jitterOuterEdge + gapBetweenJitterAndAux + (auxSize * 0.5f);
        const float angleAux = -pi / 5.0f;
        const float ax = center.x + std::cos(angleAux) * auxDistance;
        const float ay = center.y + std::sin(angleAux) * auxDistance;
        const float primaryBoundsSize = (jitterOuterEdge + 1.0f) * 2.0f;

        primarySlider.setBounds(juce::Rectangle<float>(primaryBoundsSize, primaryBoundsSize).withCentre(center).toNearestInt());
        primarySlider.getProperties().set("visualRadius", primaryVisualRadius);

        auxSlider.setBounds(juce::Rectangle<float>(auxSize, auxSize).withCentre({ax, ay}).toNearestInt());

        syncAuxDataToPrimary();
    }

    void PrimaryWithAux::syncAuxDataToPrimary()
    {
        float auxNorm = (float)auxSlider.getValue(); // 0-1
        primarySlider.getProperties().set("auxAmount", auxNorm);
    }

    void PrimaryWithAux::paint(juce::Graphics& g)
    {
        g.fillAll(coloursv2::turquoise);
        // Calculate primary's current angle and pass to aux
        float startAngle = juce::MathConstants<float>::pi * 1.25f; // Your LnF start angle
        float endAngle = juce::MathConstants<float>::pi * 2.25f; // Your LnF end angle

        float primaryRange = primarySlider.getMaximum() - primarySlider.getMinimum();
        float primaryNorm = (primarySlider.getValue() - primarySlider.getMinimum()) / primaryRange;
        float primaryAngle = startAngle + primaryNorm * (endAngle - startAngle);

        auxSlider.getProperties().set("primaryAngle", primaryAngle);

        // Continue with normal painting (children will paint themselves)
        // Actually, since we're overriding paint, we need to call base or paint children
        // Better approach: update in timer or on value change:
    }

}
