#pragma once

#include "../../framework/Types.h"
#include "../../utils/PluginParams.h"

namespace particules
{
    struct UIContext;
    class ParameterView;
    class GrainsPanel : public juce::Component //, public juce::AudioProcessorValueTreeState::Listener
    {
    public:
        GrainsPanel(UIContext&);
        ~GrainsPanel() = default;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        //void parameterChanged(const juce::String& parameterID, float newValue) override;
        ValueTreeState& apvts;
        ParameterView& paramsView;

        juce::Slider emissionSlider;
        juce::Slider durationSlider;
        juce::Slider speedSlider;
        juce::Slider sustainRatioSlider;
        juce::Slider traversalFreqSlider;

        juce::Label emissionLabel;
        juce::Label durationLabel;
        juce::Label speedLabel;
        juce::Label sustainRatioLabel;
        juce::Label envModeLabel;
        juce::Label traversalModeLabel;
        juce::Label traversalFreqLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> emissionSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> durationSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> speedSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainRatioSliderAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> traversalFreqSliderAttachment;

        juce::ComboBox envelopeModeList;
        juce::ComboBox traversalModeList;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> envelopeModeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> traversalModeAttachment;

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