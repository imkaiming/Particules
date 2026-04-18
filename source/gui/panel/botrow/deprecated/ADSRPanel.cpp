#include "ADSRPanel.h"

#include <juce_gui_basics/juce_gui_basics.h>

#include "utils/struct/UIContext.h"
#include "framework/core/PluginParams.h"
#include "gui/lookandfeelv2/Colours.h"

namespace particules
{
    ADSRPanel::ADSRPanel(UIContext& uic) : uic{uic} {


    }
    void ADSRPanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::lightBlack); }
    void ADSRPanel::resized()
    {
        //juce::Rectangle<int> area = getLocalBounds(); 
        //const int padding = 5;

        //const int paramWidth = area.getWidth() / 8;

        // area = area.removeFromBottom(area.getHeight() * 0.5f);

        //attackSlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding));
        //decaySlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding));
        //sustainSlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding));
        //releaseSlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding)); 

    }

}