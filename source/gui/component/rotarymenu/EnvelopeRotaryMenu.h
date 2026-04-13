#pragma once
#include "RotaryMenuBase.h"

namespace particules
{
    class EnvelopeRotaryMenu : public RotaryMenuBase
    {
    public:
        EnvelopeRotaryMenu(ValueTreeState& apvts, const str& modeParamId, const str& ratioParamId, const str& jitterParamId);

    protected:
        juce::Image createMenuIcon(int itemIndex) override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnvelopeRotaryMenu)
    };
}