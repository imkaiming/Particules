#include "MenuSliderBase.h"

namespace particules
{
    MenuSliderBase::MenuSliderBase(juce::AudioProcessorValueTreeState& state, const str& mId, const str& mainPId, const str& aId,
        const str& name, const str& controlTypeTag)
        : apvts(state), modeId(mId), mainId(mainPId)
    {
        modeParam = apvts.getParameter(modeId);
        apvts.addParameterListener(modeId, this);

        mainSlider.setSliderStyle(juce::Slider::LinearVertical);
        mainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        mainSlider.getProperties().set("knobStyle", static_cast<int>(RotaryType::rotaryMenuWithAux));
        mainSlider.getProperties().set("menuControlType", controlTypeTag);
        mainSlider.setRepaintsOnMouseActivity(true);
        mainSlider.setInterceptsMouseClicks(false, false);

        nameLabel.setText(name, juce::dontSendNotification);
        nameLabel.setJustificationType(juce::Justification::centred);
        nameLabel.setColour(juce::Label::textColourId, coloursv2::perleBlanc);

        mainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, mainId, mainSlider);
        if(!aId.isEmpty())
        {
            auxAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, aId, auxSlider);
        }

        currentMode = juce::roundToInt(modeParam->getValue() * (modeParam->getAllValueStrings().size() - 1));
        mainSlider.getProperties().set("currentMode", currentMode);

        addAndMakeVisible(mainSlider);
        addAndMakeVisible(auxSlider);
        addAndMakeVisible(nameLabel);
    }

    MenuSliderBase::~MenuSliderBase() { apvts.removeParameterListener(modeId, this); }

    void MenuSliderBase::parameterChanged(const juce::String& parameterID, float newValue)
    {
        if(parameterID == modeId)
        {
            currentMode = juce::roundToInt(newValue * (modeParam->getAllValueStrings().size() - 1));
            mainSlider.getProperties().set("currentMode", currentMode);
            juce::MessageManager::callAsync([this]() { mainSlider.repaint(); });
        }
    }

    void MenuSliderBase::resized()
    {
        auto area = getLocalBounds();
        nameLabel.setBounds(area.removeFromBottom(20));

        auto boxArea = area.reduced(5);
        mainSlider.setBounds(boxArea);

        int auxSize = 16;
        auxSlider.setBounds(boxArea.getRight() - auxSize, boxArea.getY(), auxSize, auxSize);
    }

    void MenuSliderBase::mouseDown(const juce::MouseEvent& e)
    {
        isDraggingSlider = false;
        mainSlider.mouseDown(e.getEventRelativeTo(&mainSlider));
    }

    void MenuSliderBase::mouseDrag(const juce::MouseEvent& e)
    {
        if(!isDraggingSlider && e.getDistanceFromDragStart() > 3)
            isDraggingSlider = true;

        if(isDraggingSlider)
            mainSlider.mouseDrag(e.getEventRelativeTo(&mainSlider));
    }

    void MenuSliderBase::mouseUp(const juce::MouseEvent& e)
    {
        mainSlider.mouseUp(e.getEventRelativeTo(&mainSlider));
        if(!isDraggingSlider)
        {
            showPopupMenu();
        }
        isDraggingSlider = false;
    }

    void MenuSliderBase::showPopupMenu()
    {
        juce::PopupMenu menu;
        juce::StringArray options = modeParam->getAllValueStrings();

        for(int i = 0; i < options.size(); ++i)
        {
            menu.addItem(i + 1, "", true, false, createMenuIcon(i));
        }

        menu.showMenuAsync(juce::PopupMenu::Options().withTargetComponent(this), [this, options](int result) {
            if(result > 0)
            {
                float newValue = static_cast<float>(result - 1) / (options.size() - 1);
                modeParam->setValueNotifyingHost(newValue);
            }
        });
    }
}