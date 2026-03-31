#include "MainPanel.h"
#include "../../framework/ParameterView.h"
#include "../../utils/MyColours.h"
#include "../../utils/struct/UIContext.h"

namespace particules
{
    MainPanel::MainPanel(UIContext& uic) : titlePanel{uic}, controlPanel{uic}, audioFilePanel{uic}, statusBarPanel{uic}
    {
        addAndMakeVisible(&titlePanel);
        addAndMakeVisible(&audioFilePanel);
        addAndMakeVisible(&controlPanel);
        addAndMakeVisible(&statusBarPanel);
    }

    void MainPanel::paint(juce::Graphics& g) { g.fillAll(colours::panel::outerPanel); }

    void MainPanel::resized()
    {
        float ratio = 0.012f;
        const int padding = juce::roundToInt(std::min(getWidth(), getHeight()) * ratio);
        juce::Rectangle<int> area = getLocalBounds().reduced(padding);

        float verticalGap = (float)juce::roundToInt(getHeight() * 0.01f);

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.alignItems = juce::FlexBox::AlignItems::stretch;

        fb.items.add(juce::FlexItem(titlePanel).withFlex(0.0f).withMinHeight(48).withMargin({0, 0, verticalGap, 0}));
        fb.items.add(juce::FlexItem(audioFilePanel).withFlex(0.45f).withMargin({0, 0, verticalGap, 0}));
        fb.items.add(juce::FlexItem(controlPanel).withFlex(0.40f).withMargin({0, 0, verticalGap, 0}));
        fb.items.add(juce::FlexItem(statusBarPanel).withFlex(0.0f).withMinHeight(24).withMargin(0));

        fb.performLayout(area);
    }

}
