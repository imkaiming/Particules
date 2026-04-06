#include "PrimaryWithAux.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../../framework/bridge/EngineState.h"
#include "../../../utils/math/MathConstants.h"
#include "../../lookandfeelv2/Colours.h"

#include "AuxRotarySlider.h"
#include "RotarySlider.h"

namespace particules
{

    PrimaryWithAux::PrimaryWithAux(const str& name, EngineState& es) : engineState{es}
    {
        label.setText((const str)name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setInterceptsMouseClicks(false, false);
        label.setColour(juce::Label::textColourId, colours::perleBlanc);
        label.setFont(juce::Font(13.0f));

        primarySlider.getProperties().set("knobStyle", static_cast<int>(RotaryType::primaryWithAux));
        auxSlider.getProperties().set("knobStyle", static_cast<int>(RotaryType::aux));

        primarySlider.onValueChange = [this]() {
            updatePrimaryAngle();
            primarySlider.repaint();

            // If linked, notify the sibling (GrainsPanel will set this callback)
            if(engineState.getIsLinked() && onValueChanged)
            {
                onValueChanged(primarySlider.getValue());
            }
        };
        auxSlider.onValueChange = [this]() {
            syncAuxDataToPrimary();
            primarySlider.repaint();
        };

        addAndMakeVisible(&primarySlider);
        addAndMakeVisible(&auxSlider);
        addAndMakeVisible(&label);
    }

    void PrimaryWithAux::setPrimaryValue(double value, juce::NotificationType notify)
    {
        primarySlider.setValue(value, notify);
        if(notify == juce::dontSendNotification)
        {
            updatePrimaryAngle(); 
            primarySlider.repaint();
        }
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
        const float maxExtentFromCenter = minDim * 0.62f;

        const float auxSize = juce::jlimit(18.0f, 32.0f, minDim * 0.18f);

        const float jitterRadiusMultiplier = 1.07f;
        const float jitterLineWidthHalf = 1.0f;
        const float safetyPadding = 0.0f;
        const float gapBetweenJitterAndAux = 4.0f;

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

        primarySlider.getProperties().set("visualRadius", primaryVisualRadius);
        primarySlider.setBounds(
            juce::Rectangle<float>(primaryBoundsSize, primaryBoundsSize).withCentre(center.toFloat()).toNearestInt());

        auxSlider.setBounds(
            juce::Rectangle<float>(auxSize, auxSize).withCentre(juce::Point<float>((float)ax, (float)ay)).toNearestInt());

        const int labelHeight = 18;
        label.setBounds(0, getHeight() - labelHeight, getWidth(), labelHeight);

        syncAuxDataToPrimary();
        updatePrimaryAngle();
    }

    void PrimaryWithAux::syncAuxDataToPrimary()
    {
        float auxNorm = (float)auxSlider.getValue();
        primarySlider.getProperties().set("auxAmount", auxNorm);
    }

    void PrimaryWithAux::paint(juce::Graphics& g)
    {
        g.fillAll(coloursv2::deepBlack.brighter(0.2f));
        float startAngle = pi * 1.25f;
        float endAngle = pi * 2.25f;
        float primaryRange = primarySlider.getMaximum() - primarySlider.getMinimum();

        if(primaryRange > 0.0f)
        {
            float primaryNorm = (primarySlider.getValue() - primarySlider.getMinimum()) / primaryRange;
            float primaryAngle = startAngle + primaryNorm * (endAngle - startAngle);
            auxSlider.getProperties().set("primaryAngle", primaryAngle);
        }
    }

}
