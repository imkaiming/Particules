/*
  ==============================================================================

    SynthFrame.cpp
    Created: 3 Feb 2023 11:44:16pm
    Author:  user

  ==============================================================================
*/

#include "SynthFrame.h"

SynthFrame::SynthFrame(ValueTreeState* apvts, StateParameters* stateParams) : apvts(apvts), stateParams(stateParams)
{

}

SynthFrame::~SynthFrame() 
{
    apvts = nullptr;
    stateParams = nullptr;
}

void SynthFrame::paint(juce::Graphics& g) {
    //juce::Rectangle<float> synthFrame(0.f, 0.f, getWidth(), getHeight());
    //g.setColour(juce::Colours::slategrey);
    //g.fillRect(synthFrame);
    //g.drawRect(synthFrame);
    g.fillAll(juce::Colours::turquoise);
}

void SynthFrame::resized() {

}