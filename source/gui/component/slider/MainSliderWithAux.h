#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "AuxRotarySlider.h"
#include "RotarySlider.h"
#include "framework/core/GuiTypes.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/math/MathConstants.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    class AudioState;
    class MainSliderWithAux : public juce::Component
    {
    public:
        MainSliderWithAux(const AudioState& as, RotaryType type, ValueTreeState& apvts, const str& name = "", const str& id = "");

        void updatePrimaryAngle();

        void setOnPrimaryValueChanged(ValueCallback callback) { onValueChanged = std::move(callback); }
        void setPrimaryValue(double value, juce::NotificationType notify = juce::dontSendNotification);

        double getPrimaryValue() const noexcept { return mainSlider.getValue(); }
        double getPrimaryMinimum() const noexcept { return mainSlider.getMinimum(); }
        double getPrimaryMaximum() const noexcept { return mainSlider.getMaximum(); }

        std::unique_ptr<ValueTreeState::SliderAttachment> attachPrimaryToAPVTS(ValueTreeState&, const str&) noexcept;
        std::unique_ptr<ValueTreeState::SliderAttachment> attachAuxToAPVTS(ValueTreeState&, const str&) noexcept;

        void resized() override;
        void paint(juce::Graphics& g) override;
        //void lookAndFeelChanged() override;

    private:
        void syncAuxDataToPrimary();

        str parameterID;
        ValueTreeState& apvts;
        const AudioState& audioState;
        ValueCallback onValueChanged;
        juce::Slider mainSlider;
        AuxRotarySlider auxSlider;
        juce::Label label;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainSliderWithAux)
    };

}