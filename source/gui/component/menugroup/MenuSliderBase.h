#pragma once
#include "../../../framework/GuiTypes.h"
#include "../../../framework/PluginParams.h"
#include "../../lookandfeelv2/Colours.h"
#include "../slider/AuxRotarySlider.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    class MenuSliderBase : public juce::Component, protected ValueTreeState::Listener
    {
    public:
        MenuSliderBase(ValueTreeState& apvts, const str& modeParamId, const str& mainParamId,
            const str& auxParamId, const str& name, const str& controlTypeTag);
        ~MenuSliderBase() override;

        void resized() override;

        void mouseDown(const juce::MouseEvent& e) override;
        void mouseDrag(const juce::MouseEvent& e) override;
        void mouseUp(const juce::MouseEvent& e) override;

    protected:
        void parameterChanged(const juce::String& parameterID, float newValue) override;
        virtual void showPopupMenu();
        virtual juce::Image createMenuIcon(int itemIndex) = 0;

        juce::AudioProcessorValueTreeState& apvts;
        str modeId, mainId;
        juce::RangedAudioParameter* modeParam;

        juce::Slider mainSlider;
        AuxRotarySlider auxSlider;
        juce::Label nameLabel;

        std::unique_ptr<ValueTreeState::SliderAttachment> mainAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> auxAttachment;

        int currentMode = 0;
        bool isDraggingSlider = false;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MenuSliderBase)
    };
}