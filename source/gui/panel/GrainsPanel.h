#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../framework/GuiTypes.h"
#include "../../framework/PluginParams.h"
#include "../Component/slider/PrimaryWithAux.h"
#include "../Component/slider/RotarySlider.h"
#include "../Component/button/IconButton.h"

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

        PrimaryWithAux emissionSlider;
        PrimaryWithAux durationSlider;
        SecondaryRotarySlider speedSlider;
        SecondaryRotarySlider sustainRatioSlider;
        SecondaryRotarySlider traversalFreqSlider;

        IconButton linkBtn;
        std::unique_ptr<juce::Drawable> linkInIcon, linkOffIcon;

        juce::Label speedLabel;
        juce::Label sustainRatioLabel;
        juce::Label envModeLabel;
        juce::Label traversalModeLabel;
        juce::Label traversalFreqLabel;

        std::unique_ptr<ValueTreeState::SliderAttachment> emissionSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> durationSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> speedSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> sustainRatioSliderAttachment;
        std::unique_ptr<ValueTreeState::SliderAttachment> traversalFreqSliderAttachment;

        juce::ComboBox envelopeModeList;
        juce::ComboBox traversalModeList;

        std::unique_ptr<ValueTreeState::ComboBoxAttachment> envelopeModeAttachment;
        std::unique_ptr<ValueTreeState::ComboBoxAttachment> traversalModeAttachment;

        static constexpr const char* grainsEmissionId = params::emission::id;
        static constexpr const char* grainsEmissionName = params::emission::name;
        static constexpr const float grainsEmissionMin = params::emission::min;
        static constexpr const float grainsEmissionMax = params::emission::max;
        static constexpr const float grainsEmissionSkewFactor = params::emission::skewFactor;

        static constexpr const char* grainsDurationId = params::duration::id;
        static constexpr const char* grainsDurationName = params::duration::name;
        static constexpr const float grainsDurationMin = params::duration::min;
        static constexpr const float grainsDurationMax = params::duration::max;
        static constexpr const float grainsDurationSkewFactor = params::duration::skewFactor;

        static constexpr const char* grainsSpeedId = params::speed::id;
        static constexpr const char* grainsSpeedName = params::speed::name;
        static constexpr const float grainsSpeedMin = params::speed::min;
        static constexpr const float grainsSpeedMax = params::speed::max;
        static constexpr const float grainsSpeedSkewFactor = params::speed::skewFactor;

        static constexpr const char* grainsSustainRatioId = params::sustainRatio::id;
        static constexpr const char* grainsSustainRatioName = params::sustainRatio::name;
        static constexpr const float grainsSustainRatioMin = params::sustainRatio::min;
        static constexpr const float grainsSustainRatioMax = params::sustainRatio::max;
        static constexpr const float grainsSustainRatioSkewFactor = params::sustainRatio::skewFactor;

        static constexpr const char* grainsTraversalFreqId = params::traversalFreq::id;
        static constexpr const char* grainsTraversalFreqName = params::traversalFreq::name;
        static constexpr const float grainsTraversalFreqMin = params::traversalFreq::min;
        static constexpr const float grainsTraversalFreqMax = params::traversalFreq::max;
        static constexpr const float grainsTraversalFreqSkewFactor = params::traversalFreq::skewFactor;

        static constexpr const char* grainsEnvelopeModeId = params::envelopeMode::id;
        static constexpr const char* grainsEnvelopeModeName = params::envelopeMode::name;

        static constexpr const char* grainsTraversalModeId = params::traversalMode::id;
        static constexpr const char* grainsTraversalModeName = params::traversalMode::name;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsPanel)
    };
}