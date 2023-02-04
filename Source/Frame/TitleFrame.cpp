/*
  ==============================================================================

	TitleFrame.cpp
	Created: 4 Feb 2023 1:27:14pm
	Author:  user

  ==============================================================================
*/

#include "TitleFrame.h"

TitleFrame::TitleFrame() : titreLabel((const juce::String)"Particules")
{
	addAndMakeVisible(titreLabel);
	//titreLabel.setFont(juce::Font(16.0f, BinaryData::CascadiaMonoPLRegular_otf));
	titreLabel.setJustificationType(juce::Justification::centred);
}

TitleFrame::~TitleFrame() {

}

void TitleFrame::paint(juce::Graphics& g) {
	g.fillAll(juce::Colours::black);
}

void TitleFrame::resized() {
	titreLabel.setBounds(getLocalBounds());
}
