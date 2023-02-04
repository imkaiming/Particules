/*
  ==============================================================================

	MainFrame.cpp
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#include "MainFrame.h"

MainFrame::MainFrame() {
	addAndMakeVisible(synthFrame);
	addAndMakeVisible(grainsFrame);
	addAndMakeVisible(audioFileFrame);
}

MainFrame::~MainFrame() {
}

void MainFrame::paint(juce::Graphics& g) {
	juce::Rectangle<float> mainFrame(0.f, 0.f, getWidth(), getHeight());
	g.setColour(juce::Colours::black);
	g.fillRect(mainFrame);
	g.drawRect(mainFrame);
	//g.fillAll(juce::Colours::darkgrey);

}

void MainFrame::resized() {
	synthFrame.setBounds(1.f, 1.f, 898.f, 148.f);
	grainsFrame.setBounds(1.f, 150.f, 898.f, 148.f);
	audioFileFrame.setBounds(1.f, 300.f, 898.f, 148.f);
}
