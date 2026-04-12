#include "TabPanel.h"

#include "../../../framework/PluginParams.h"
#include "../../../utils/struct/UIContext.h"
#include "../../lookandfeelv2/Colours.h"

#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    TabPanel::TabPanel(UIContext& uic) : uic{uic} {}

    void TabPanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::lightBlack); }
    void TabPanel::resized() {}

}