/*
  ==============================================================================

	StatusBar.cpp
	Created: 4 Feb 2023 1:26:27pm
	Author:  user

  ==============================================================================
*/

#include "StatusBarFrame.h"
#include "../Utils/MyColours.h"

StatusBarFrame::StatusBarFrame() //UIContext& uic): uic{uic}
{
	addAndMakeVisible(&statusLabel);
}

void StatusBarFrame::paint(juce::Graphics& g)
{
	g.fillAll(MyColours::black);

	statusLabel.setText((const juce::String)"IFT3150", juce::dontSendNotification);
	statusLabel.setJustificationType(juce::Justification::centredLeft);
	statusLabel.setColour(0, juce::Colours::white);
	//statusLabel.setSize(statusLabel.getWidth() * 0.5f, statusLabel.getHeight() * 0.5f);
}
void StatusBarFrame::resized()
{
	float h = getHeight() / 30.f;

	juce::FlexBox flexbox;
	flexbox.items.add(juce::FlexItem(statusLabel).withFlex(1).withMargin(h));
	flexbox.performLayout(getLocalBounds().toFloat());
}