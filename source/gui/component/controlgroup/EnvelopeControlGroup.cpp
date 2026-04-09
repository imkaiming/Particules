#include "EnvelopeControlGroup.h"
#include "../../../framework/PluginParams.h"

namespace particules
{
    EnvelopeControlGroup::EnvelopeControlGroup(UIContext& uic)
        : ButtonSliderControlGroup{uic, (str) "Envelope"},
          buttonMenu{uic.apvts, params::envelopeMode::id, params::sustainRatio::id}
    {
        setupSlider(
            params::sustainRatio::id, params::sustainRatio::min, params::sustainRatio::max, params::sustainRatio::skewFactor);
        addAndMakeVisible(&buttonMenu);
    }
}