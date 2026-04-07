#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../framework/GuiTypes.h"
#include "../../utils/PluginParams.h"
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

        static constexpr const char* grainsEmissionId = grains::emission::id;
        static constexpr const char* grainsEmissionName = grains::emission::name;
        static constexpr const float grainsEmissionMin = grains::emission::min;
        static constexpr const float grainsEmissionMax = grains::emission::max;
        static constexpr const float grainsEmissionSkewFactor = grains::emission::skewFactor;

        static constexpr const char* grainsDurationId = grains::duration::id;
        static constexpr const char* grainsDurationName = grains::duration::name;
        static constexpr const float grainsDurationMin = grains::duration::min;
        static constexpr const float grainsDurationMax = grains::duration::max;
        static constexpr const float grainsDurationSkewFactor = grains::duration::skewFactor;

        static constexpr const char* grainsSpeedId = grains::speed::id;
        static constexpr const char* grainsSpeedName = grains::speed::name;
        static constexpr const float grainsSpeedMin = grains::speed::min;
        static constexpr const float grainsSpeedMax = grains::speed::max;
        static constexpr const float grainsSpeedSkewFactor = grains::speed::skewFactor;

        static constexpr const char* grainsSustainRatioId = grains::sustainRatio::id;
        static constexpr const char* grainsSustainRatioName = grains::sustainRatio::name;
        static constexpr const float grainsSustainRatioMin = grains::sustainRatio::min;
        static constexpr const float grainsSustainRatioMax = grains::sustainRatio::max;
        static constexpr const float grainsSustainRatioSkewFactor = grains::sustainRatio::skewFactor;

        static constexpr const char* grainsTraversalFreqId = grains::traversalFreq::id;
        static constexpr const char* grainsTraversalFreqName = grains::traversalFreq::name;
        static constexpr const float grainsTraversalFreqMin = grains::traversalFreq::min;
        static constexpr const float grainsTraversalFreqMax = grains::traversalFreq::max;
        static constexpr const float grainsTraversalFreqSkewFactor = grains::traversalFreq::skewFactor;

        static constexpr const char* grainsEnvelopeModeId = grains::envelopeMode::id;
        static constexpr const char* grainsEnvelopeModeName = grains::envelopeMode::name;

        static constexpr const char* grainsTraversalModeId = grains::traversalMode::id;
        static constexpr const char* grainsTraversalModeName = grains::traversalMode::name;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GrainsPanel)
    };
}