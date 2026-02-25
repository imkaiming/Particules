/*
  ==============================================================================

	CircularBuffer.h
	Created: 15 Apr 2023 12:39:57am
	Author:  user

  ==============================================================================
*/

#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_basics/juce_audio_basics.h>

//template <typename Type>
class CircularBuffer : public juce::AudioBuffer<float>
{
public:
	CircularBuffer(int numChannels, int numSamples);
	~CircularBuffer();

	void clear();
	//void pushSamples(const juce::AudioBuffer<float>& bufferToPush);
	void fillBuffer(const juce::AudioBuffer<float>& buffer);

private:
	void updatePosition(int samples);
	//juce::AudioBuffer<float> audioBuffer;
	int writePosition;
};