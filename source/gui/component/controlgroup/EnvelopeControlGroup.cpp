#include "EnvelopeControlGroup.h"
#include "../../../framework/PluginParams.h"
#include "../../lookandfeelv2/Colours.h"

namespace particules
{
    EnvelopeControlGroup::EnvelopeControlGroup(UIContext& uic)
        : ButtonSliderControlGroup{uic, (str) "Envelope"},
          buttonMenu{uic.apvts, params::envelopeMode::id, params::envelopeRatio::id}
    {
        setupSlider(params::envelopeRatio::id);
        addAndMakeVisible(&buttonMenu);
    }
}