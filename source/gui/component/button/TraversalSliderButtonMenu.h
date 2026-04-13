#pragma once

#include "../../../framework/GuiTypes.h"
#include "../../../framework/PluginParams.h"
#include "../../../utils/enum/TraversalMode.h"
#include "SliderButtonMenu.h"

namespace particules
{
    class TraversalSliderButtonMenu : public SliderButtonMenu, private ValueTreeState::Listener
    {
    public:
        TraversalSliderButtonMenu(ValueTreeState&, const str&, const str&);
        ~TraversalSliderButtonMenu() override;

        juce::Path createCurvePath(juce::Rectangle<float> bounds) override;
        void showPopupMenu() override;
        juce::Image createMenuIcon(int itemIndex) override;

        float getDragValue() const override;
        void setDragValue(float newValue) override;

    private:
        void parameterChanged(const str&, float) override;

        // APVTS
        TraversalMode getCurrentMode() const { return cachedMode; }
        float getCurrentFrequency() const { return traversalFreqParam->convertFrom0to1(traversalFreqParam->getValue()); }
        int getCurrentModeIndex() const { return static_cast<int>(cachedMode); }

        TraversalMode cachedMode;
        float cachedFreq;
        ValueTreeState& apvts;
        str traversalModeId;
        str traversalFreqId;
        juce::RangedAudioParameter* traversalModeParam;
        juce::RangedAudioParameter* traversalFreqParam;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TraversalSliderButtonMenu)
    };
}