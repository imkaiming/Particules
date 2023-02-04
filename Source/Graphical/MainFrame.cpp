/*
  ==============================================================================

	MainFrame.cpp
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#include "MainFrame.h"

MainFrame::MainFrame() {
	addAndMakeVisible(audioFileFrame);
}

MainFrame::~MainFrame() {
}

void MainFrame::paint(juce::Graphics& g) {
	juce::Rectangle<float> mainFrame(1.f, 1.f, getWidth() - 2.f, getHeight() - 2.f);
	g.setColour(juce::Colours::darkslategrey);
	g.fillRect(mainFrame);
	g.drawRect(mainFrame);

}

void MainFrame::resized() {
	audioFileFrame.setBounds(getLocalBounds());
}
