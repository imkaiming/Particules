#pragma once

#include "../../../framework/GuiTypes.h"
#include "../../../utils/struct/UIContext.h"
#include "../../lookandfeelv2/Colours.h"
#include "../slider/MainSliderWithAux.h"

namespace particules
{
    class ButtonSliderControlGroup : public juce::Component
    {
    public:
        ButtonSliderControlGroup(UIContext&, const str&);
        ~ButtonSliderControlGroup() = default;

        void paint(juce::Graphics& g) override;
        void resized() override;

    protected:
        void setupSlider(const str& paramId); //, float min, float max, float skewFactor);

        str name;
        UIContext& uic;
        MainSliderWithAux slider;
        std::unique_ptr<ValueTreeState::SliderAttachment> attachment;

    private:
        virtual juce::Component& getButton() = 0;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ButtonSliderControlGroup)
    };
}