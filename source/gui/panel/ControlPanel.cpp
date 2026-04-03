#include "ControlPanel.h"

#include <juce_graphics/juce_graphics.h>

#include "../lookandfeel/MyColours.h"
#include "../../utils/struct/UIContext.h"

namespace particules
{

    ControlPanel::ControlPanel(UIContext& uic) : uic{uic}, adsrPanel{uic}, synthPanel{uic}, grainsPanel{uic}
    {
        addAndMakeVisible(&grainsPanel);
        addAndMakeVisible(&adsrPanel);
        addAndMakeVisible(&synthPanel);
    }

    void ControlPanel::paint(juce::Graphics& g)
    {
        const juce::Rectangle<float> inner = getLocalBounds().reduced(2).toFloat();
        //g.setColour(colours::accent::lightgreyAccent);
        g.setColour(colours::panel::controlPanel);
        g.fillRoundedRectangle(inner, 12.0f);

        const float lineThickness = 2.0f;

        g.setColour(colours::panel::contourPanel);
        g.drawRoundedRectangle(inner, 12.0f, lineThickness);
    }

    void ControlPanel::resized()
    {
        float ratio = 0.012f;
        const int padding = juce::roundToInt(std::min(getWidth(), getHeight()) * ratio);
        juce::Rectangle<int> area = getLocalBounds().reduced(padding);

        const float gap = (float)juce::roundToInt(getWidth() * 0.009f);

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.flexWrap = juce::FlexBox::Wrap::noWrap;
        fb.alignItems = juce::FlexBox::AlignItems::stretch;
        fb.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

        fb.items.add(juce::FlexItem(grainsPanel).withFlex(1.0f).withMargin(0));
        fb.items.add(juce::FlexItem(adsrPanel).withFlex(1.0f).withMargin({0, gap * 0.5f, 0, gap * 0.5f}));
        fb.items.add(juce::FlexItem(synthPanel).withFlex(1.0f).withMargin(0));

        fb.performLayout(area);
    }
}
