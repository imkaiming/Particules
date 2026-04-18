#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "framework/core/GuiTypes.h"
#include "framework/core/PluginParams.h"
#include "gui/lookandfeelv2/Colours.h"
#include "gui/component/slider/AuxRotarySlider.h"

namespace particules
{
    class RotaryMenuBase : public juce::Component, protected ValueTreeState::Listener
    {
    public:
        RotaryMenuBase(ValueTreeState& apvts, const str& modeParamId, const str& mainParamId, const str& auxParamId,
            const str& name, const str& controlTypeTag);
        ~RotaryMenuBase() override;

        void resized() override;

        void mouseDown(const juce::MouseEvent& e) override;
        void mouseDrag(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent& e) override;

    protected:
        void parameterChanged(const juce::String& parameterID, float newValue) override;
        virtual void showPopupMenu();
        virtual juce::Image createMenuIcon(int itemIndex) = 0;

        void updatePrimaryAngle();
        void syncAuxDataToPrimary();

        juce::AudioProcessorValueTreeState& apvts;
        str modeId, mainId;
        juce::RangedAudioParameter* modeParam;

        juce::Slider mainSlider;
        AuxRotarySlider auxSlider;
        juce::Label nameLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mainAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> auxAttachment;

        int currentMode = 0;
        bool isDraggingSlider = false;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RotaryMenuBase)
    };
}