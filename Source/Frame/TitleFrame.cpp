/*
  ==============================================================================

	TitleFrame.cpp
	Created: 4 Feb 2023 1:27:14pm
	Author:  user

  ==============================================================================
*/

#include "TitleFrame.h"
//#include "../Framework/UIContext.h"
#include "../Utils/CustomLookAndFeel.h"
#include "../Utils/MyColours.h"

TitleFrame::TitleFrame(CustomLookAndFeel& look): look{look}
{
	addAndMakeVisible(&titreLabel);
}

void TitleFrame::paint(juce::Graphics& g)
{
	titreLabel.setText((const juce::String)"Particules", juce::dontSendNotification);
	titreLabel.setJustificationType(juce::Justification::centred);
	titreLabel.setColour(0, juce::Colours::white);

	// TODO check if it worked
	const juce::Font font(look.getFuturaTypeface());
	font.withHeight(16.f);
	titreLabel.setFont(font);
	g.fillAll(MyColours::smokyBlack);

	//const juce::Font f;
	//g.setFont((juce::Font)customLookAndFeel.getTypefaceForFont(f));
	//g.setColour(juce::Colours::black);
	//g.drawText("ParticulesS", getLocalBounds(), juce::Justification::centred, true);
}

void TitleFrame::resized()
{
	float h = getHeight() / 30.f;
	juce::FlexBox flexbox;
	flexbox.items.add(juce::FlexItem(titreLabel).withFlex(1).withMargin(h));
	flexbox.performLayout(getLocalBounds().toFloat());
	//titreLabel.setBounds(getLocalBounds());
}
