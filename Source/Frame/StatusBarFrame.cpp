/*
  ==============================================================================

	StatusBar.cpp
	Created: 4 Feb 2023 1:26:27pm
	Author:  user

  ==============================================================================
*/

#include "StatusBarFrame.h"


StatusBarFrame::StatusBarFrame(StateParameters* stateParams) : stateParams(stateParams)
{

}
StatusBarFrame::~StatusBarFrame() 
{
	stateParams = nullptr;
}
void StatusBarFrame::paint(juce::Graphics& g) {
	g.fillAll(MyColours::black);
}
void StatusBarFrame::resized() {

}