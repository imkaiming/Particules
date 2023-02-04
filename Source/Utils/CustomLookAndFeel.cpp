/*
  ==============================================================================

	CustomLookAndFeel.cpp
	Created: 4 Feb 2023 3:43:16pm
	Author:  user

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

CustomLookAndFeel::CustomLookAndFeel()
{
	//setColourScheme(getColourScheme());
}

CustomLookAndFeel::~CustomLookAndFeel()
{
}

juce::Typeface::Ptr CustomLookAndFeel::getTypefaceForFont(const juce::Font& f)
{
	static juce::Typeface::Ptr myFont = juce::Typeface::createSystemTypefaceFor(BinaryData::CascadiaMonoPLRegular_otf,
		BinaryData::CascadiaMonoPLRegular_otfSize);
	return myFont;
}