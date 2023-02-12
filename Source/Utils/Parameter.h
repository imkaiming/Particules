/*
  ==============================================================================

	Parameter.h
	Created: 12 Feb 2023 11:11:05am
	Author:  user

  ==============================================================================
*/

/**
 * This class is user to represent a parameter in the plugin. It in
 */

#pragma once
#include <JuceHeader.h>

class Parameter : public juce::AudioProcessorParameter
{
	// implements all the virtual methods
public:
	Parameter(const juce::String&, const juce::String&, float);
	~Parameter();
	float getValue() const override;
	void setValue(float newValue) override;
	float getDefaultValue() const override;
	juce::String getName(int) const override;
	juce::String getLabel() const override;
	int getNumSteps() const override;
	juce::String getText(float, int) const override;
	float getValueForText(const juce::String&) const override;

private:
	const juce::String paramId, paramName;
	const float defaultValue;
};
