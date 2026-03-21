/*
  ==============================================================================

	StatusBar.cpp
	Created: 4 Feb 2023 1:26:27pm
	Author:  user

  ==============================================================================
*/

#include "StatusBarFrame.h"
#include "../PluginProcessor.h"
#include "../utils/struct/UIContext.h"
#include "../utils/MyColours.h"

StatusBarFrame::StatusBarFrame(UIContext& uic) : uic{uic}, audioProcessor{uic.audioProcessor}
{
    addAndMakeVisible(&statusLabel);
    startTimerHz(10);

    statusLabel.setColour(0, juce::Colours::white);
    statusLabel.setSize((int)(statusLabel.getWidth() * 0.5f), (int)(statusLabel.getHeight() * 0.5f));
    statusLabel.setJustificationType(juce::Justification::centredLeft);
    statusLabel.setColour(0, juce::Colours::white);
}

void StatusBarFrame::paint(juce::Graphics& g) { g.fillAll(MyColours::black); }

void StatusBarFrame::resized()
{
    float h = getHeight() / 30.f;

    juce::FlexBox flexbox;
    flexbox.items.add(juce::FlexItem(statusLabel).withFlex(1).withMargin(h));
    flexbox.performLayout(getLocalBounds().toFloat());
}

void StatusBarFrame::timerCallback()
{
    statusLabel.setText((const juce::String) "active grains: " + (const juce::String)audioProcessor.getNumActiveGrains(),
        juce::dontSendNotification);
    repaint();
}