#include "MainSliderWithAux.h"

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../../framework/bridge/EngineState.h"
#include "../../../utils/math/MathConstants.h"
#include "../../lookandfeelv2/Colours.h"

#include "../../lookandfeelv2/MainLNF.h"
#include "AuxRotarySlider.h"
#include "RotarySlider.h"

namespace particules
{

    MainSliderWithAux::MainSliderWithAux(EngineState& es, RotaryType type, ValueTreeState& apvts, const str& name, const str& id)
        : engineState{es}, parameterID{id}, apvts{apvts}
    {
        label.setText(name, juce::dontSendNotification);
        label.setJustificationType(juce::Justification::centred);
        label.setInterceptsMouseClicks(false, false);
        label.setColour(juce::Label::textColourId, juce::Colours::white);

        mainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        mainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        if(type == RotaryType::primaryWithAux)
        {
            setColour(juce::Slider::rotarySliderFillColourId, coloursv2::cyan);
            setColour(juce::Slider::rotarySliderOutlineColourId, coloursv2::perleBlanc);
        }
        else if(type == RotaryType::secondaryWithAux)
        {
            setColour(juce::Slider::rotarySliderFillColourId, colours::violetBleu);
            setColour(juce::Slider::rotarySliderOutlineColourId, coloursv2::perleBlanc);
        }

        setRepaintsOnMouseActivity(true);
        mainSlider.getProperties().set("RotaryType", static_cast<int>(type));
        auxSlider.getProperties().set("RotaryType", static_cast<int>(RotaryType::aux));

        mainSlider.onValueChange = [this]() {
            updatePrimaryAngle();
            mainSlider.repaint();

            if(engineState.getIsLinked() && onValueChanged)
            {
                onValueChanged(mainSlider.getValue());
            }
        };

        auxSlider.onValueChange = [this]() {
            syncAuxDataToPrimary();
            mainSlider.repaint();
        };

        addAndMakeVisible(&mainSlider);
        addAndMakeVisible(&auxSlider);

        if(name != "")
            addAndMakeVisible(&label);
    }

    void MainSliderWithAux::lookAndFeelChanged()
    {
        if(MainLNF* lnf = dynamic_cast<MainLNF*>(&getLookAndFeel()))
            label.setFont(juce::Font(lnf->getGeistTypeface()).withHeight(14.0f).withExtraKerningFactor(0.2f));
    }

    void MainSliderWithAux::setPrimaryValue(double value, juce::NotificationType notify)
    {
        mainSlider.setValue(value, notify);
        if(notify == juce::dontSendNotification)
        {
            updatePrimaryAngle();
            mainSlider.repaint();
        }
    }

    void MainSliderWithAux::updatePrimaryAngle()
    {
        const float startAngle = pi * 1.25f;
        const float endAngle = pi * 2.25f;

        if(parameterID == "")
        {
            const float range = mainSlider.getMaximum() - mainSlider.getMinimum();
            const float norm = (mainSlider.getValue() - mainSlider.getMinimum()) / range;
            const float angle = startAngle + norm * (endAngle - startAngle);
            auxSlider.getProperties().set("primaryAngle", angle);
            auxSlider.repaint();
        }
        else
        {
            juce::NormalisableRange<float> range = apvts.getParameterRange(parameterID);
            float value = mainSlider.getValue();
            float proportion = range.convertTo0to1(value);
            float angle = startAngle + proportion * (endAngle - startAngle);
            auxSlider.getProperties().set("primaryAngle", angle);
            auxSlider.repaint();
        }
    }

    void MainSliderWithAux::setRange(float min, float max) noexcept { mainSlider.setRange(min, max); }

    void MainSliderWithAux::setSkewFactorFromMidPoint(float skew) noexcept { mainSlider.setSkewFactorFromMidPoint(skew); }

    std::unique_ptr<ValueTreeState::SliderAttachment> MainSliderWithAux::attachPrimaryToAPVTS(
        ValueTreeState& a, const str& id) noexcept
    {
        return std::make_unique<ValueTreeState::SliderAttachment>(a, id, mainSlider);
    }

    std::unique_ptr<ValueTreeState::SliderAttachment> MainSliderWithAux::attachAuxToAPVTS(
        ValueTreeState& apvts, const str& id) noexcept
    {
        return std::make_unique<ValueTreeState::SliderAttachment>(apvts, id, auxSlider);
    }

    void MainSliderWithAux::resized()
    {
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        juce::Point<float> center = bounds.getCentre();

        const float minDim = juce::jmin(bounds.getWidth(), bounds.getHeight());
        const float maxExtentFromCenter = minDim * 0.62f;

        const float auxSize = juce::jlimit(18.0f, 32.0f, minDim * 0.18f);

        // constants
        const float jitterOuterMultiplier = 1.10f; // 1.07f
        const float auxGap = 4.0f;
        const float safetyPadding = 3.0f;
        const float jitterLineWidthHalf = 1.0f;

        // reserve space
        const float maxAuxCenterDist = maxExtentFromCenter - safetyPadding - (auxSize * 0.5f);
        const float availableForJitter = maxAuxCenterDist - auxGap - (auxSize * 0.5f);

        // visual radius
        const float primaryVisualRadius = juce::jmax(1.0f, (availableForJitter - jitterLineWidthHalf) / jitterOuterMultiplier);
        const float jitterOuterEdge = (primaryVisualRadius * jitterOuterMultiplier) + jitterLineWidthHalf;

        // position
        const float auxDistance = jitterOuterEdge + auxGap + (auxSize * 0.5f);
        const float angleAux = -pi / 5.0f;
        const float ax = center.x + std::cos(angleAux) * auxDistance;
        const float ay = center.y + std::sin(angleAux) * auxDistance;

        // bounds
        const float primaryBoundsSize = (jitterOuterEdge + 1.0f) * 2.0f;

        mainSlider.getProperties().set("visualRadius", primaryVisualRadius);
        mainSlider.setBounds(
            juce::Rectangle<float>(primaryBoundsSize, primaryBoundsSize).withCentre(center.toFloat()).toNearestInt());

        auxSlider.setBounds(juce::Rectangle<float>(auxSize, auxSize)
                .withCentre(juce::Point<float>(static_cast<float>(ax), static_cast<float>(ay)))
                .toNearestInt());

        if(label.getText() != "")
        {
            const int labelHeight = (bounds.getHeight() - jitterOuterEdge) * 0.4f - safetyPadding;
            label.setBounds(0, getHeight() - labelHeight, getWidth(), labelHeight);
        }

        syncAuxDataToPrimary();
        updatePrimaryAngle();
    }

    void MainSliderWithAux::syncAuxDataToPrimary()
    {
        const float auxNorm = static_cast<float>(auxSlider.getValue());
        mainSlider.getProperties().set("auxAmount", auxNorm);
    }

    void MainSliderWithAux::paint(juce::Graphics& g)
    {
        g.fillAll(coloursv2::deepBlack.brighter(0.0f));

        const float startAngle = pi * 1.25f;
        const float endAngle = pi * 2.25f;
        const float primaryRange = mainSlider.getMaximum() - mainSlider.getMinimum();

        if(primaryRange > 0.0f)
        {
            const float primaryNorm = (mainSlider.getValue() - mainSlider.getMinimum()) / primaryRange;
            const float primaryAngle = startAngle + primaryNorm * (endAngle - startAngle);
            auxSlider.getProperties().set("primaryAngle", primaryAngle);
        }
    }

}
