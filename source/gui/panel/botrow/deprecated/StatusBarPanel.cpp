#include "StatusBarPanel.h"
#include "../../../../utils/struct/UIContext.h"
#include "../../../lookandfeelv2/Colours.h"

namespace particules
{

    StatusBarPanel::StatusBarPanel(UIContext& uic) : uic{uic}, audioState{uic.audioState}
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
        /*
        const juce::Rectangle<float> inner = getLocalBounds().reduced(2).toFloat();
        g.setColour(colours::panel::audioFilePanel);
        g.fillRoundedRectangle(inner, 12.0f);

        const float lineThickness = 2.0f;

        g.setColour(colours::panel::contourPanel);
        g.drawRoundedRectangle(inner, 12.0f, lineThickness);
        
        */

        g.fillAll(colours::grisAnthracite);
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
            (const str) "active grains: " + (const str)audioState.getNumActiveGrains(), juce::dontSendNotification);
        repaint();
    }
}