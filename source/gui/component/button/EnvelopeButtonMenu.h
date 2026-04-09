#pragma once

#include "../../../framework/GuiTypes.h"
#include "../../../framework/PluginParams.h"
#include "DrawButtonMenu.h"

namespace particules
{
    class EnvelopeButtonMenu : public DrawButtonMenu, private ValueTreeState::Listener
    {
    public:
        EnvelopeButtonMenu(ValueTreeState&, const str&, const str&);
        ~EnvelopeButtonMenu() override;

        juce::Path createCurvePath(juce::Rectangle<float> bounds) override;
        void showPopupMenu() override;
        juce::Image createMenuIcon(int itemIndex) override;

    private:
        void parameterChanged(const str& parameterID, float newValue) override;
        EnvelopeMode getCurrentMode() const { return cachedMode; }
        int getCurrentModeIndex() const { return static_cast<int>(cachedMode); }
        float getCurrentSustain() const { return cachedSustain; }

        EnvelopeMode cachedMode;
        float cachedSustain;
        ValueTreeState& apvts;
        str envelopeModeId;
        str sustainRatioId;
        juce::RangedAudioParameter* envelopeModeParam;
        juce::RangedAudioParameter* sustainParam;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeButtonMenu)
    };
}