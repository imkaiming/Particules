/*
  ==============================================================================

	CircularBuffer.cpp
	Created: 15 Apr 2023 12:39:57am
	Author:  user

  ==============================================================================
*/

#include "CircularBuffer.h"

//template <typename Type>
//CircularBuffer<Type>::
CircularBuffer::CircularBuffer(int numChannels, int numSamples) :
	juce::AudioBuffer<float>(numChannels, numSamples),
	writePosition(0)
{
	clear();
}


//template <typename Type>
//CircularBuffer<Type>::
CircularBuffer::~CircularBuffer()
{
}


//template <typename Type>
//void CircularBuffer<Type>::
void CircularBuffer::clear()
{
	juce::AudioBuffer<float>::clear();
	writePosition = 0;
}

//template <typename Type>
//void CircularBuffer<Type>::
//void CircularBuffer::pushSamples(const juce::AudioBuffer<float>& buffer)
//{
//	for (size_t i = 0; i < buffer.getNumSamples(); ++i)
//	{
//		for (size_t channel = 0; channel < this->getNumChannels(); ++channel)
//		{
//			const float* readPointer = buffer.getReadPointer(channel);
//			//float sample = buffer.getSample(channel, i);
//			setSample(channel, this->writePosition, readPointer[i]);
//
//		}
//		updatePosition();
//	}
//}

void CircularBuffer::fillBuffer(const juce::AudioBuffer<float>& buffer)
{
	const int bufferSize = buffer.getNumSamples();
	const int circularSize = this->getNumSamples();

	for (size_t channel = 0; channel < this->getNumChannels(); ++channel)
	{

		if (circularSize >= bufferSize + writePosition)
		{
			this->copyFrom(channel, writePosition,
				buffer.getReadPointer(channel), bufferSize);
		}
		else
		{
			int numSamplesToEnd = circularSize - writePosition;
			int numSamplesAtStart = bufferSize - numSamplesToEnd;

			copyFrom(channel, writePosition, buffer.getReadPointer(channel), numSamplesToEnd);
			copyFrom(channel, 0, buffer.getReadPointer(channel, numSamplesToEnd), numSamplesAtStart);
		}
	}

	updatePosition(bufferSize);
}

void CircularBuffer::updatePosition(int samples)
{
	this->writePosition += samples;
	this->writePosition %= this->getNumSamples();
}

//void CircularBuffer::pushSamples(const float sample, const int channel)
//{
//	for (size_t channel = 0; channel < this->getNumChannels(); ++channel)
//	{
//		const float* readPointer = buffer.getReadPointer(channel);
//		//float sample = buffer.getSample(channel, i);
//		setSample(channel, this->writePosition, readPointer[i]);
//
//	}
//	updatePosition();
//}
