#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../framework/GuiTypes.h"
#include "../../framework/PluginParams.h"
#include "../Component/slider/MainSliderWithAux.h"
//#include "../Component/slider/RotarySlider.h"
#include "../Component/button/IconButton.h"
#include "../Component/button/EnvelopeButton.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    struct UIContext;
    class EngineState;
    class GrainsPanel : public juce::Component //, public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        GrainsPanel(UIContext& uic);
        ~GrainsPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        //void parameterChanged(const juce::String& parameterID, float newValue) override;
        void linkButtonClicked();
        void setLinkButtonImage();

        EngineState& engineState;

        MainSliderWithAux emissionSlider;
        MainSliderWithAux durationSlider;
        MainSliderWithAux speedSlider;
        MainSliderWithAux sustainRatioSlider;
        MainSliderWithAux traversalFreqSlider;

        IconButton linkBtn;
        std::unique_ptr<juce::Drawable> linkInIcon, linkOffIcon;

        //juce::Label speedLabel;
        //juce::Label sustainRatioLabel;
        //juce::Label traversalFreqLabel;
        juce::Label traversalModeLabel;
        juce::Label envModeLabel;

        std::unique_ptr<ValueTreeState::SliderAttachment> emissionSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> durationSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> speedSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> sustainRatioSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> traversalFreqSliderAttachment;

        EnvelopeButton envelopeModeButton;
        //juce::ComboBox envelopeModeList;
        //std::unique_ptr<ValueTreeState::ComboBoxAttachment> envelopeModeAttachment;

        juce::ComboBox traversalModeList;
        std::unique_ptr<ValueTreeState::ComboBoxAttachment> traversalModeAttachment;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsPanel)
    };
}