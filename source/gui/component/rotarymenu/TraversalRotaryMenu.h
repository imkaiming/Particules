#pragma once
#include "RotaryMenuBase.h"

namespace particules
{
    class TraversalRotaryMenu : public RotaryMenuBase
    {
    public:
        TraversalRotaryMenu(ValueTreeState& apvts, const str& modeParamId, const str& freqParamId, const str& jitterParamId);

    protected:
        juce::Image createMenuIcon(int itemIndex) override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TraversalRotaryMenu)
    };
}