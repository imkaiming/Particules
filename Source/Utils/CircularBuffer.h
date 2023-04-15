/*
  ==============================================================================

	CircularBuffer.h
	Created: 15 Apr 2023 12:39:57am
	Author:  user

  ==============================================================================
*/

#include <JuceHeader.h>

#pragma once

template <typename Type>
class CircularBuffer : public juce::AudioBuffer<Type>
{
public:
	CircularBuffer(int numChannels, int numSamples);
	~CircularBuffer();

	void clear();
	void pushSamples(const juce::AudioBuffer<Type>& bufferToPush);


private:
	//juce::AudioBuffer<float> audioBuffer;
	int writePosition;
};