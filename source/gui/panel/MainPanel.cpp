#include "MainPanel.h"
#include "../../framework/ParameterView.h"
#include "../../utils/struct/UIContext.h"
#include "../../utils/MyColours.h"


namespace particules
{
    MainPanel::MainPanel(UIContext& uic)
        : titlePanel(uic.customLookAndFeel), synthPanel(uic.apvts), grainsPanel(uic.apvts), audioFilePanel(uic),
          statusBarPanel(uic)
    {
        addAndMakeVisible(&titlePanel);
        addAndMakeVisible(&synthPanel);
        addAndMakeVisible(&grainsPanel);
        addAndMakeVisible(&audioFilePanel);
        addAndMakeVisible(&statusBarPanel);
    }

    void MainPanel::paint(juce::Graphics& g) { g.fillAll(MyColours::black); }

    void MainPanel::resized()
    {
        float heightComp = getHeight() / 30.f;

        juce::FlexBox flexbox;
        flexbox.flexDirection = juce::FlexBox::Direction::column;
        flexbox.flexWrap = juce::FlexBox::Wrap::noWrap;
        flexbox.alignContent = juce::FlexBox::AlignContent::stretch;
        flexbox.alignItems = juce::FlexBox::AlignItems::stretch;

        flexbox.items.add(juce::FlexItem(titlePanel).withHeight(heightComp * 2));
        flexbox.items.add(juce::FlexItem(synthPanel).withHeight(heightComp * 9).withMargin(1));
        flexbox.items.add(juce::FlexItem(grainsPanel).withHeight(heightComp * 9).withMargin(1));
        flexbox.items.add(juce::FlexItem(audioFilePanel).withHeight(heightComp * 9).withMargin(1));
        flexbox.items.add(juce::FlexItem(statusBarPanel).withHeight(heightComp * 1));

        flexbox.performLayout(getLocalBounds().toFloat());
    }

}
