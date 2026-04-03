#include "ADSRPanel.h"

//#include <juce_gui_basics/juce_gui_basics.h>
#include "../../utils/struct/UIContext.h"


namespace particules
{
    ADSRPanel::ADSRPanel(UIContext& uic) : uic{uic} { }
    void ADSRPanel::paint(juce::Graphics& g) { /*        g.fillAll(juce::Colours::beige);        */ }
    void ADSRPanel::resized() {}
}