#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "../../../framework/GuiTypes.h"
#include "../../../utils/math/MathConstants.h"
#include "../../lookandfeelv2/Colours.h"
#include "AuxRotarySlider.h"
#include "RotarySlider.h"

namespace juce
{
    class Graphics;
}

namespace particules
{

    class PrimaryWithAux : public juce::Component
    {
    public:
        PrimaryWithAux();

        void setRange(float min, float max) noexcept;
        void updatePrimaryAngle();
        void setSkewFactorFromMidPoint(float skew) noexcept;

        std::unique_ptr<ValueTreeState::SliderAttachment> attachPrimaryToAPVTS(ValueTreeState&, const str&) noexcept;
        std::unique_ptr<ValueTreeState::SliderAttachment> attachAuxToAPVTS(ValueTreeState&, const str&) noexcept;

        void resized() override;
        void paint(juce::Graphics& g) override;

    private:
        void syncAuxDataToPrimary();

        PrimaryRotarySlider primarySlider;
        AuxRotarySlider auxSlider;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PrimaryWithAux)
    };

}