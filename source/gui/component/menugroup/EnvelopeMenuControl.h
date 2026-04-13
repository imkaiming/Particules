#pragma once
#include "MenuSliderBase.h"

namespace particules
{
    class EnvelopeMenuControl : public MenuSliderBase
    {
    public:
        EnvelopeMenuControl(ValueTreeState& apvts, const str& modeParamId, const str& ratioParamId, const str& jitterParamId);

    protected:
        juce::Image createMenuIcon(int itemIndex) override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeMenuControl)
    };
}