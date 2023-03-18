/*
  ==============================================================================

	GrainsFrame.cpp
	Created: 3 Feb 2023 11:44:06pm
	Author:  user

  ==============================================================================
*/

#include "GrainsFrame.h"

GrainsFrame::GrainsFrame(ValueTreeState* apvts, StateParameters* stateParams) : apvts(apvts), stateParams(stateParams)
{

}
GrainsFrame::~GrainsFrame() 
{
	apvts = nullptr;
	stateParams = nullptr;
}

void GrainsFrame::paint(juce::Graphics& g) {
	//juce::Rectangle<float> grainsFrame(0.f, 0.f, getWidth(), getHeight());
	//g.setColour(juce::Colours::slategrey);
	//g.fillRect(grainsFrame);
	//g.drawRect(grainsFrame);
	g.fillAll(juce::Colours::slategrey);
}

void GrainsFrame::resized() {

}