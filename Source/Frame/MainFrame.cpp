/*
  ==============================================================================

	MainFrame.cpp
	Created: 3 Feb 2023 1:21:53pm
	Author:  user

  ==============================================================================
*/

#include "MainFrame.h"

MainFrame::MainFrame() {
	addAndMakeVisible(titleFrame);
	addAndMakeVisible(synthFrame);
	addAndMakeVisible(grainsFrame);
	addAndMakeVisible(audioFileFrame);
	addAndMakeVisible(statusBarFrame);
}

MainFrame::~MainFrame() {
}

void MainFrame::paint(juce::Graphics& g) {
	//juce::Rectangle<float> mainFrame(5.f, 0.f, getWidth() - 10.f, getHeight());
	//g.setColour(juce::Colours::grey);
	//g.fillRect(mainFrame);
	//g.drawRect(mainFrame);
	//g.fillAll(juce)

}

void MainFrame::resized()
{
	juce::Rectangle<int> area = getLocalBounds();
	float heightComp = getHeight() / 30.f;
	float widthComp = getWidth() / 10.f;

	int w = getWidth() / 300;

	titleFrame.setBounds(area.removeFromTop(heightComp * 1));
	area.removeFromTop(w);
	synthFrame.setBounds(area.removeFromTop(heightComp * 9));
	area.removeFromTop(w);
	grainsFrame.setBounds(area.removeFromTop(heightComp * 9));
	area.removeFromTop(w);
	audioFileFrame.setBounds(area.removeFromTop(heightComp * 9));
	area.removeFromTop(w);
	statusBarFrame.setBounds(area.removeFromTop(heightComp * 2));


	// old positionning
	//titleFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 1);
	//statusBarFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 9);
	//synthFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 9);
	//grainsFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 9);
	//audioFileFrame.setBounds(0.f, 0.f, widthComp * 10, heightComp * 2);
}
