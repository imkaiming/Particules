#include "StatusBarPanel.h"
#include "../../utils/MyColours.h"
#include "../../utils/struct/UIContext.h"

namespace particules
{

    StatusBarPanel::StatusBarPanel(UIContext& uic) : uic{uic}, engineState{uic.engineState}
    {
        addAndMakeVisible(&statusLabel);
        startTimerHz(30);

        statusLabel.setColour(0, juce::Colours::white);
        statusLabel.setSize((int)(statusLabel.getWidth() * 0.5f), (int)(statusLabel.getHeight() * 0.5f));
        statusLabel.setJustificationType(juce::Justification::centredLeft);
        statusLabel.setColour(0, juce::Colours::white);
    }

    void StatusBarPanel::paint(juce::Graphics& g)
    {
        const juce::Rectangle<float> inner = getLocalBounds().reduced(2).toFloat();
        g.setColour(colours::panel::audioFilePanel);
        g.fillRoundedRectangle(inner, 12.0f);

        const float lineThickness = 2.0f;

        g.setColour(colours::panel::contourPanel);
        g.drawRoundedRectangle(inner, 12.0f, lineThickness);
    }

    void StatusBarPanel::resized()
    {
        //float h = getHeight() / 30.f;

        //juce::FlexBox flexbox;
        //flexbox.items.add(juce::FlexItem(statusLabel).withFlex(1).withMargin(h));
        //flexbox.performLayout(getLocalBounds().toFloat());
    }

    void StatusBarPanel::timerCallback()
    {
        statusLabel.setText(
            (const str) "active grains: " + (const str)engineState.getNumActiveGrains(), juce::dontSendNotification);
        repaint();
    }
}