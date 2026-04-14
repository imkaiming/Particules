#include "RotaryMenuBase.h"
#include "../../../utils/math/MathConstants.h"

namespace particules
{
    RotaryMenuBase::RotaryMenuBase(juce::AudioProcessorValueTreeState& state, const str& mId, const str& mainPId, const str& aId,
        const str& name, const str& controlTypeTag)
        : apvts(state), modeId(mId), mainId(mainPId)
    {
        modeParam = apvts.getParameter(modeId);
        apvts.addParameterListener(modeId, this);

        mainSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        mainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        mainSlider.getProperties().set("RotaryType", static_cast<int>(RotaryType::rotaryMenuWithAux));
        mainSlider.getProperties().set("menuControlType", controlTypeTag);

        mainSlider.setRepaintsOnMouseActivity(true);
        mainSlider.setInterceptsMouseClicks(false, false);

        auxSlider.getProperties().set("RotaryType", static_cast<int>(RotaryType::aux));

        nameLabel.setText(name, juce::dontSendNotification);
        nameLabel.getProperties().set("isName", true);
        nameLabel.setJustificationType(juce::Justification::centred);
        nameLabel.setColour(juce::Label::textColourId, coloursv2::perleBlanc);
        nameLabel.setInterceptsMouseClicks(false, false);

        mainAttachment = std::make_unique<ValueTreeState::SliderAttachment>(apvts, mainId, mainSlider);
        if(!aId.isEmpty())
        {
            auxAttachment = std::make_unique<ValueTreeState::SliderAttachment>(apvts, aId, auxSlider);
        }

        currentMode = juce::roundToInt(apvts.getRawParameterValue(modeId)->load());
        mainSlider.getProperties().set("currentMode", currentMode);

        mainSlider.onValueChange = [this]() {
            updatePrimaryAngle();
            mainSlider.repaint();
        };

        auxSlider.onValueChange = [this]() {
            syncAuxDataToPrimary();
            mainSlider.repaint();
        };

        addAndMakeVisible(&mainSlider);
        addAndMakeVisible(&auxSlider);
        addAndMakeVisible(&nameLabel);
    }

    RotaryMenuBase::~RotaryMenuBase() { apvts.removeParameterListener(modeId, this); }

    void RotaryMenuBase::parameterChanged(const juce::String& parameterID, float newValue)
    {
        if(parameterID == modeId)
        {
            currentMode = juce::roundToInt(newValue);
            mainSlider.getProperties().set("currentMode", currentMode);
            juce::MessageManager::callAsync([this]() { mainSlider.repaint(); });
        }
    }

    void RotaryMenuBase::resized()
    {
        juce::Rectangle<float> bounds = getLocalBounds().toFloat();
        juce::Point<float> center = bounds.getCentre();

        const float minDim = juce::jmin(bounds.getWidth(), bounds.getHeight());

        const float maxExtentFromCenter = minDim * 0.62f;
        const float auxSize = juce::jlimit(18.0f, 32.0f, minDim * 0.18f);

        const float jitterOuterMultiplier = 1.10f;
        const float auxGap = 4.0f;
        const float safetyPadding = 3.0f;
        const float jitterLineWidthHalf = 1.0f;

        const float maxAuxCenterDist = maxExtentFromCenter - safetyPadding - (auxSize * 0.5f);
        const float availableForJitter = maxAuxCenterDist - auxGap - (auxSize * 0.5f);

        const float primaryVisualRadius = juce::jmax(1.0f, (availableForJitter - jitterLineWidthHalf) / jitterOuterMultiplier);
        const float jitterOuterEdge = (primaryVisualRadius * jitterOuterMultiplier) + jitterLineWidthHalf;

        const float auxDistance = jitterOuterEdge + auxGap + (auxSize * 0.5f);
        const float angleAux = -pi / 5.0f;
        const float ax = center.x + std::cos(angleAux) * auxDistance;
        const float ay = center.y + std::sin(angleAux) * auxDistance;

        const float primaryBoundsSize = (jitterOuterEdge + 1.0f) * 2.0f;

        mainSlider.getProperties().set("visualRadius", primaryVisualRadius);
        mainSlider.setBounds(juce::Rectangle<float>(primaryBoundsSize, primaryBoundsSize).withCentre(center).toNearestInt());

        auxSlider.setBounds(juce::Rectangle<float>(auxSize, auxSize).withCentre(juce::Point<float>(ax, ay)).toNearestInt());

        if(nameLabel.getText() != "")
        {
            const int labelHeight = (bounds.getHeight() - jitterOuterEdge) * 0.4f - safetyPadding;
            nameLabel.setBounds(0, getHeight() - labelHeight, getWidth(), labelHeight);
        }

        syncAuxDataToPrimary();
        updatePrimaryAngle();
    }

    void RotaryMenuBase::updatePrimaryAngle()
    {
        const float startAngle = pi * 1.25f;
        const float endAngle = pi * 2.25f;
        juce::NormalisableRange<float> range = apvts.getParameterRange(mainId);
        float value = mainSlider.getValue();
        float proportion = range.convertTo0to1(value);
        float angle = startAngle + proportion * (endAngle - startAngle);
        auxSlider.getProperties().set("primaryAngle", angle);
        auxSlider.repaint();
    }

    void RotaryMenuBase::syncAuxDataToPrimary()
    {
        const float auxNorm = juce::jlimit(0.0f, 1.0f, static_cast<float>(auxSlider.getValue()));
        mainSlider.getProperties().set("auxAmount", auxNorm);
    }

    void RotaryMenuBase::mouseDown(const juce::MouseEvent& e)
    {
        isDraggingSlider = false;
        mainSlider.mouseDown(e.getEventRelativeTo(&mainSlider));
    }

    void RotaryMenuBase::mouseDrag(const juce::MouseEvent& e)
    {
        if(!isDraggingSlider && e.getDistanceFromDragStart() > 3)
            isDraggingSlider = true;

        if(isDraggingSlider)
            mainSlider.mouseDrag(e.getEventRelativeTo(&mainSlider));
    }

    void RotaryMenuBase::mouseUp(const juce::MouseEvent& e)
    {
        mainSlider.mouseUp(e.getEventRelativeTo(&mainSlider));
        if(!isDraggingSlider)
        {
            showPopupMenu();
        }
        isDraggingSlider = false;
    }

    void RotaryMenuBase::showPopupMenu()
    {
        juce::PopupMenu menu;

        menu.setLookAndFeel(&getLookAndFeel());

        juce::StringArray options = modeParam->getAllValueStrings();

        for(int i = 0; i < options.size(); ++i)
        {
            menu.addItem(i + 1, "", true, false, createMenuIcon(i));
        }

        juce::PopupMenu::Options optionsPopup;
        optionsPopup =
            optionsPopup.withTargetScreenArea(juce::Rectangle<int>(getScreenPosition().x, getScreenPosition().y, 1, 1));

        menu.showMenuAsync(optionsPopup, [this, options](int result) {
            if(result > 0)
            {
                float newValue = static_cast<float>(result - 1) / (options.size() - 1);
                modeParam->setValueNotifyingHost(newValue);
            }
        });
    }
}