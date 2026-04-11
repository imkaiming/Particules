#include "EnvelopeControlGroup.h"
#include "../../../framework/PluginParams.h"

namespace particules
{
    EnvelopeControlGroup::EnvelopeControlGroup(UIContext& uic)
        : ButtonSliderControlGroup{uic, (str) "Envelope"},
          buttonMenu{uic.apvts, params::envelopeMode::id, params::envelopeRatio::id}
    {
        setupSlider(
            params::envelopeRatio::id, params::envelopeRatio::min, params::envelopeRatio::max, params::envelopeRatio::skewFactor);
        addAndMakeVisible(&buttonMenu);
    }
}