#include "SynthPanel.h"

#include <juce_graphics/juce_graphics.h>

#include "../../../../utils/struct/UIContext.h"
#include "../../../lookandfeel/MyColours.h"

namespace particules
{

    SynthPanel::SynthPanel(UIContext& uic) : meter{}
    {
        outputSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(uic.apvts, params::output::id, outputSlider);

        addAndMakeVisible(&meter);
        addAndMakeVisible(&outputSlider);

      
    }

    void SynthPanel::paint(juce::Graphics& g) { /* g.fillAll(juce::Colours::purple); */ }

    void SynthPanel::resized()
    {
        
        juce::Rectangle<int> area = getLocalBounds();
        meter.setBounds(area.removeFromRight(35));
    }

    void SynthPanel::updateMeter(float levelDB) { /* meter.pushLevel(levelDB); */}
}