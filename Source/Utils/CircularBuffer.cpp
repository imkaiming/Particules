/*
  ==============================================================================

	CircularBuffer.cpp
	Created: 15 Apr 2023 12:39:57am
	Author:  user

  ==============================================================================
*/

#include "CircularBuffer.h"

template <typename Type>
CircularBuffer<Type>::CircularBuffer(int numChannels, int numSamples) :
	juce::AudioBuffer<Type>(numChannels, numSamples), 
	writePosition(0)
{
	clear();
}


template <typename Type>
CircularBuffer<Type>::~CircularBuffer()
{
}


template <typename Type>
void CircularBuffer<Type>::clear()
{
	juce::AudioBuffer<Type>::clear();
	writePosition = 0;
}

template <typename Type>
void CircularBuffer<Type>::pushSamples(const juce::AudioBuffer<Type>& bufferToPush)
{
	int numChannels = getNumChannels();
	int numSamples = getNumSamples();
	int numSamplesToPush = bufferToPush.getNumSamples();
	int bufferToPushOffset = 0;

	for (int i = 0; i < numSamplesToPush; ++i)
	{
		for (int ch = 0; ch < numChannels; ++ch)
		{
			float sample = bufferToPush.getSample(ch, bufferToPushOffset + i);
			setSample(ch, writePosition, sample);
		}
		writePosition = ++writePosition % numSamples;
	}
}
