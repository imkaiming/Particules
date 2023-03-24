/*
  ==============================================================================

	Grain.cpp
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#include "Grain.h"

WindowingMethod Grain::getWindowingMethod(int val) {
	WindowingMethod window = WindowingMethod::hann;
	switch (val) {
	case 1:
		window = WindowingMethod::hann;
		break;
	case 2:
		window = WindowingMethod::triangular;
		break;
	case 3:
		window = WindowingMethod::hamming;
		break;
	default:
		break;
	}

	return window;
}


Grain::Grain(int duration, int numChannels, int envelopeType, float speed, int envelopeWidth, int position, int selection, juce::AudioBuffer<float>* buffer) :
	duration(duration), numChannels(numChannels), position(position), selection(selection),
	speed(speed), envelopeWidth(envelopeWidth), grainBuffer(numChannels, duration), envelopeType(envelopeType) //, window(getWindowingMethod(envelopeType))
{
	DBG("duration : " << duration);
	DBG("position : " << position);
	DBG("selection : " << selection);
	DBG("buffer->getNumSamples() : " << buffer->getNumSamples());
	// on ajoute le contenu du buffer dans le buffer local
	for (size_t channel = 0; channel < numChannels; ++channel)
	{
		const float* readerPointer = buffer->getReadPointer(channel);
		float* writerPointer = grainBuffer.getWritePointer(channel);

		for (int sample = position; sample < duration; ++sample)
		{
			writerPointer[sample] = readerPointer[sample];
			//grainBuffer.copyFrom(channel, position, buffer->getReadPointer(channel), selection);
		}
	}

	// on resample le buffer en fonction de la vitesse en appliquant le phase vocoder algorithme
	applyEnvelope();

	// on applique l'envelope à partir de la duration et de envelope width


	currentTime = 0;
}

Grain::~Grain()
{
}

// on prend un grain du buffer du sample d'entré pour le mettre dans le buffer des grains
// si la duration est plus grande que la selection alors on loop 
float Grain::getCurrentSample(int channel)
{

	// on veut récupérer le sample dans une fenetre de positionSamples à positionSamples + selectionSamples 
	return grainBuffer.getSample(channel, currentTime);
}


void Grain::update() // int channel)
{
	currentTime++;
}

bool Grain::isActive()
{
	if (currentTime >= duration) {
		return false;
	}

	return true;
}


void Grain::applyEnvelope()
{
	const int fadeIn = (duration - envelopeWidth) / 2; // 0 to fadeIn
	const int fadeOut = fadeIn + envelopeWidth; // fadeOut to numSamples


	switch (envelopeType)
	{
	case 1:
	{
		for (size_t channel = 0; channel < numChannels; channel++)
		{

			float* channelData = grainBuffer.getWritePointer(channel);

			for (int sample = 0; sample < fadeIn; ++sample)
				channelData[sample] *= hannEnvelope(sample, fadeIn + fadeOut);

			for (int sample = fadeOut; sample < duration; ++sample)
				channelData[sample] *= hannEnvelope(sample, fadeIn + fadeOut);

		}
		break;
	}
	case 2:
	{
		for (size_t channel = 0; channel < numChannels; channel++)
		{

			float* channelData = grainBuffer.getWritePointer(channel);

			for (int sample = 0; sample < fadeIn; ++sample)
				channelData[sample] *= triangularEnvelope(sample, fadeIn + fadeOut);


			for (int sample = fadeOut; sample < duration; ++sample)
				channelData[sample] *= triangularEnvelope(sample, fadeIn + fadeOut);

		}
		break;
	}
	case 3:
	{
		for (size_t channel = 0; channel < numChannels; channel++)
		{

			float* channelData = grainBuffer.getWritePointer(channel);

			for (int sample = 0; sample < fadeIn; ++sample)
				channelData[sample] *= hammingEnvelope(sample, fadeIn + fadeOut);

			for (int sample = fadeOut; sample < duration; ++sample)
				channelData[sample] *= hammingEnvelope(sample, fadeIn + fadeOut);

		}
		break;
	}
	}
}


float Grain::hannEnvelope(int indexSample, int length)
{
	float envelopeValue = 0.0f;
	if (indexSample < length)
	{
		float phase = (2.0f * juce::MathConstants<float>::pi * float(indexSample)) / float(length - 1);
		envelopeValue = 0.5f * (1.0f - std::cos(phase));
	}
	return envelopeValue;
}

float Grain::triangularEnvelope(int indexSample, int length)
{
	float envelopeValue = 0.0f;
	if (indexSample < length)
	{
		float halfSlots = static_cast<float>(0.5) * static_cast<float> (length - 1);
		envelopeValue = static_cast<float> (1.0) - std::abs((static_cast<float> (indexSample) - halfSlots) / halfSlots);

	}
	return envelopeValue;
}


float Grain::hammingEnvelope(int indexSample, int length)
{
	float envelopeValue = 0.0f;
	if (indexSample < length)
	{
		float cos2 = ncos(2, indexSample, length);
		envelopeValue = static_cast<float> (0.54 - 0.46 * cos2);
	}
	return envelopeValue;
}


float Grain::ncos(size_t order, size_t i, size_t size)
{
	return std::cos(static_cast<float> (order * i)
		* juce::MathConstants<float>::pi / static_cast<float> (size - 1));
}


/*

float Grain::applyEnvelope(float sample)
{
	switch (window)
	{

		case blackman:
		{
			constexpr FloatType alpha = 0.16f;

			for (size_t i = 0; i < size; ++i)
			{
				auto cos2 = ncos<FloatType>(2, i, size);
				auto cos4 = ncos<FloatType>(4, i, size);

				samples[i] = static_cast<FloatType> (0.5 * (1 - alpha) - 0.5 * cos2 + 0.5 * alpha * cos4);
			}
		}
		break;

		case blackmanHarris:
		{
			for (size_t i = 0; i < size; ++i)
			{
				auto cos2 = ncos<FloatType>(2, i, size);
				auto cos4 = ncos<FloatType>(4, i, size);
				auto cos6 = ncos<FloatType>(6, i, size);

				samples[i] = static_cast<FloatType> (0.35875 - 0.48829 * cos2 + 0.14128 * cos4 - 0.01168 * cos6);
			}
		}
		break;


		return sample;
	}
}

float Grain::triangularEnvelope(int indexSample, int length)
{
	int midpoint = length / 2;
	float slope = 1.0f / midpoint;

	if (indexSample <= midpoint)
		return slope * indexSample;
	else
		return 1.0f - slope * (indexSample - midpoint);
}

*/