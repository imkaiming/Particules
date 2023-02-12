/*
  ==============================================================================

	Parameter.cpp
	Created: 12 Feb 2023 11:11:05am
	Author:  user

  ==============================================================================
*/

#include "Parameter.h"

Parameter::Parameter(const juce::String& paramId, const juce::String& paramName, float defaultValue)
	: paramId(paramId), paramName(paramName), defaultValue(defaultValue)
{
}

Parameter::~Parameter()
{
}
float Parameter::getValue() const
{
	return 0.0f;
}

void Parameter::setValue(float newValue)
{
}
float Parameter::getDefaultValue() const
{
	return 0.0f;
}

juce::String Parameter::getName(int) const
{
	return "paramName";
}
juce::String Parameter::getLabel() const
{
	return "paramLabel";
}

int Parameter::getNumSteps() const
{
	return 0;
}
juce::String Parameter::getText(float value, int maxLength) const
{
	return "paramText";
}

float Parameter::getValueForText(const juce::String& text) const
{
	return 0.0f;
}
