#pragma once
#include "MenuSliderBase.h"

namespace particules
{
    class TraversalMenuControl : public MenuSliderBase
    {
    public:
        TraversalMenuControl(ValueTreeState& apvts, const str& modeParamId, const str& freqParamId, const str& jitterParamId);

    protected:
        juce::Image createMenuIcon(int itemIndex) override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TraversalMenuControl)
    };
}