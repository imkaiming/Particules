#include "MainPanel.h"
#include "../../framework/bridge/ParameterView.h"
#include "../../utils/struct/UIContext.h"
#include "../lookandfeel/MyColours.h"

namespace particules
{
    MainPanel::MainPanel(UIContext& uic) : titlePanel{uic}, tabPanel{uic}, audioFilePanel{uic} 
    {
        addAndMakeVisible(&titlePanel);
        addAndMakeVisible(&audioFilePanel);
        addAndMakeVisible(&tabPanel);

        //addAndMakeVisible(&vignette);
        //vignette.setInterceptsMouseClicks(false, false);
    }


    void MainPanel::paint(juce::Graphics& g) {}

    void MainPanel::resized()
    {
        const float ratio = 0.012f;
        const int padding = juce::roundToInt(std::min(getWidth(), getHeight()) * ratio);
        juce::Rectangle<int> area = getLocalBounds();

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.alignItems = juce::FlexBox::AlignItems::stretch;

        fb.items.add(juce::FlexItem(titlePanel).withFlex(0.0f).withMinHeight(50));
        fb.items.add(juce::FlexItem(audioFilePanel).withFlex(0.45f));
        fb.items.add(juce::FlexItem(tabPanel).withFlex(0.55f));

        fb.performLayout(area);
        //vignette.setBounds(getLocalBounds());
    }

}
