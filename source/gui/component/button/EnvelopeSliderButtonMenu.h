#pragma once

#include "../../../framework/GuiTypes.h"
#include "../../../framework/PluginParams.h"
#include "SliderButtonMenu.h"

namespace particules
{
    class EnvelopeSliderButtonMenu : public SliderButtonMenu, private ValueTreeState::Listener
    {
    public:
        EnvelopeSliderButtonMenu(ValueTreeState&, const str&, const str&);
        ~EnvelopeSliderButtonMenu() override;

        juce::Path createCurvePath(juce::Rectangle<float> bounds) override;
        void showPopupMenu() override;
        juce::Image createMenuIcon(int itemIndex) override;

        float getDragValue() const override;
        void setDragValue(float newValue) override;

    private:
        void parameterChanged(const str& parameterID, float newValue) override;
        EnvelopeMode getCurrentMode() const { return cachedMode; }
        int getCurrentModeIndex() const { return static_cast<int>(cachedMode); }
        float getCurrentSustain() const { return cachedEnvelopeRatio; }

        EnvelopeMode cachedMode;
        float cachedEnvelopeRatio;
        ValueTreeState& apvts;
        str envelopeModeId;
        str envelopeRatioId;
        juce::RangedAudioParameter* envelopeModeParam;
        juce::RangedAudioParameter* envelopeRatioParam;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeSliderButtonMenu)
    };
}