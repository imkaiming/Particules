#include "TraversalControlGroup.h"
#include "../../../framework/PluginParams.h"

namespace particules
{
    TraversalControlGroup::TraversalControlGroup(UIContext& uic)
        : ButtonSliderControlGroup{uic, (str) "Traversal"},
          buttonMenu{uic.apvts, params::traversalMode::id, params::traversalFreq::id}
    {
        setupSlider(
            params::traversalFreq::id, params::traversalFreq::min, params::traversalFreq::max, params::traversalFreq::skewFactor);
        addAndMakeVisible(&buttonMenu);
    }
}