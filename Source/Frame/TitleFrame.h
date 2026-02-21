/*
  ==============================================================================

	TitleFrame.h
	Created: 4 Feb 2023 1:27:14pm
	Author:  user

  ==============================================================================
*/
#pragma once

#include "../Framework/Core.h"

class CustomLookAndFeel;
class TitleFrame : public juce::Component {
public:
	TitleFrame(CustomLookAndFeel& look);
	~TitleFrame() = default;
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	juce::Label titreLabel;
	CustomLookAndFeel& look;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitleFrame)
};