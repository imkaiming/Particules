#include "ADSRPanel.h"

#include <juce_gui_basics/juce_gui_basics.h>
#include "../../utils/struct/UIContext.h"
#include "../../framework/PluginParams.h"
#include "../lookandfeelv2/Colours.h"

namespace particules
{
    ADSRPanel::ADSRPanel(UIContext& uic) : uic{uic} {

        attackSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(uic.apvts, params::attack::id, attackSlider);
        decaySliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(uic.apvts, params::decay::id, decaySlider);
        sustainSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(uic.apvts, params::sustain::id, sustainSlider);
        releaseSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(uic.apvts, params::release::id, releaseSlider);

        attackSlider.setSliderStyle(juce::Slider::LinearBarVertical);
        decaySlider.setSliderStyle(juce::Slider::LinearBarVertical);
        sustainSlider.setSliderStyle(juce::Slider::LinearBarVertical);
        releaseSlider.setSliderStyle(juce::Slider::LinearBarVertical);

        addAndMakeVisible(&attackSlider);
        addAndMakeVisible(&decaySlider);
        addAndMakeVisible(&sustainSlider);
        addAndMakeVisible(&releaseSlider);
    }
    void ADSRPanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::lightBlack); }
    void ADSRPanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds(); 
        const int padding = 5;

        const int paramWidth = area.getWidth() / 8;

         area = area.removeFromBottom(area.getHeight() * 0.5f);

        attackSlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding));
        decaySlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding));
        sustainSlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding));
        releaseSlider.setBounds(area.removeFromLeft(paramWidth).reduced(padding)); 

    }

}