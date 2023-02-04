/*
  ==============================================================================

	MainFrame.cpp
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#include "MainFrame.h"

MainFrame::MainFrame() {
}

MainFrame::~MainFrame() {
}

void MainFrame::paint(juce::Graphics& g) {
	juce::Rectangle<float> mainFrame(5.f, 5.f, getWidth() - 5.f, getHeight() - 5.f);
	g.setColour(juce::Colours::indigo);
	g.fillRect(mainFrame);
	g.drawRect(mainFrame);

}

void MainFrame::resized() {
	spectrumComponent.setBounds(getLocalBounds());
}
