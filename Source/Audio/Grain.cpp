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


Grain::Grain(int length, int numChannel, int envelopeType, float speed, AudioBlock* audioBlock) :
	length(length), numChannel(numChannel),
	window(getWindowingMethod(envelopeType)), speed(speed),
	buffer(audioBlock->getNumChannels(), audioBlock->getNumSamples())
	/*	audioBlock(audioBlock) *///, envelopeType(envelopeType)
{
	//juce::Logger::outputDebugString("numChannel : " + (juce::String)audioBlock->getNumChannels() + " numSample : " + (juce::String)audioBlock->getNumSamples());
	audioBlock->copyTo(buffer);

	// on resample le buffer en fonction de la vitesse en appliquant le phase vocoder algorithme

	currentTime = 0;
	active = true; // un grain est créer lorsqu'il est ajouté dans le vector dont il est joué tout de suite
}

Grain::~Grain()
{
	//audioBlock = nullptr;
	//delete audioBlock;
}

void Grain::init()
{
	currentTime = 0;
	//currentTimeChannel.clear();

	//for (int i = 0; i < numChannel; ++i) {
	//	currentTimeChannel.push_back(0);
	//}
}

void Grain::updateBuffer(AudioBlock* audioBlock) {
	juce::Logger::outputDebugString("on copie le nouveau block entièrement dans le buffer du grain");
	buffer.setSize(audioBlock->getNumChannels(), audioBlock->getNumSamples());
	audioBlock->copyTo(buffer);
}

float Grain::applyEnvelope(float sample)
{
	switch (window)
	{
	case hann:

		//float cos2 = ncos(2, currentTime, length);
		//sample = static_cast<float> (0.5 - 0.5 * cos2);


		//if (currentTime < length)
		//{
		//	float phase = (2.0f * juce::MathConstants<float>::pi * float(currentTime)) / float(length - 1);
		//	sample = 0.5f * (1.0f - std::cos(phase));
		//}

		sample* hannEnvelope(currentTime, length);
		break;
	case triangular:
		//float halfSlots = static_cast<float> (0.5) * static_cast<float> (length - 1);
		//sample = static_cast<float> (1.0) - std::abs((static_cast<float> (currentTime) - halfSlots) / halfSlots);
		// 
		//sample* trapezoidalEnvelope(currentTime, length, 1.0f);

		sample* triangularEnvelope(currentTime, length);
		break;

	case hamming:
		//float cos2 = ncos(2, currentTime, length);
		//sample = static_cast<float> (0.54 - 0.46 * cos2);
		sample* hammingEnvelope(currentTime, length);
		break;
		/*
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
		*/

		return sample;
	}
}

// on prend un grain du buffer du sample d'entré pour le mettre dans le buffer des grains
float Grain::getCurrentSample(int channel)
{
	//juce::Logger::outputDebugString("numChannel : " + (juce::String)channel + " numSample : " + (juce::String)buffer.getNumSamples());
	return buffer.getSample(channel % numChannel, currentTime % buffer.getNumSamples());

	//return blockPtr[currentTime % numSample];
}


void Grain::applyCrossFade(int crossfade, bool atStart)
{
	if (atStart) {
		crossfade = std::min(crossfade, this->length - 1);
		buffer.applyGainRamp(0, crossfade, 0, 1);
	}
	else {
		buffer.applyGainRamp(this->length - crossfade, crossfade, 1, 0);
	}
}

void Grain::update() // int channel)
{
	currentTime++;
}

int Grain::remainingLife()
{
	return length - currentTime;
}

// pour savoir si on desactive un grain, il faut que tous 
// les temps courrant des channels soit supérieur à la durée
bool Grain::isActive()
{
	bool active = true;

	if (currentTime >= length) {
		//juce::Logger::outputDebugString("active = false");
		active = false;
	}

	return active;
}


float Grain::parabolicEnvelope(int index, int duration, float width)
{
	if (width == 0)
		return index == duration / 2 ? 1 : 0;
	int halfDuration = (int)duration / 2;
	int halfDurationPositive = halfDuration;
	float alpha = duration / (1 - 1 / width);

	if (duration % 2 == 0) {
		halfDurationPositive--;
	}
	float stdv = (duration - 1) / (2 * alpha);

	index = index - halfDurationPositive;
	return std::exp(-(index * index) / (2 * (alpha * alpha)));
}

float Grain::trapezoidalEnvelope(int index, int duration, float width)
{
	int sustain = width * duration;
	int attack = (duration - sustain) / 2;
	if (attack == 0)
		return 1;

	float angularCoeff = 1.0f / attack;

	if (index <= attack)
		return (angularCoeff * (float)index);
	if (index < sustain + attack)
		return 1;
	index -= sustain + attack;
	return 1 - angularCoeff * index;

}

float Grain::raisedCosineBellEnvelope(int index, int duration, float width)
{
	int sustain = width * duration;
	int attack = (duration - sustain) / 2;

	if (index < attack)
		return (1 + std::cos(juce::MathConstants<float>::pi + (juce::MathConstants<float>::pi * index / attack))) / 2;
	if (index < sustain + attack)
		return 1;
	index -= sustain + attack;
	return (1 - std::cos(juce::MathConstants<float>::pi - (juce::MathConstants<float>::pi * index / attack))) / 2;

}

float Grain::ncos(size_t order, size_t i, size_t size)
{
	return std::cos(static_cast<float> (order * i)
		* juce::MathConstants<float>::pi / static_cast<float> (size - 1));
}

float Grain::hannEnvelope(int indexSample, int length)
{
	float envelopeValue = 0.0f;
	if (indexSample < length)
	{
		float phase = (2.0f * juce::MathConstants<float>::pi * float(indexSample)) / float(length - 1);
		envelopeValue = 0.5f * (1.0f - std::cos(phase));
		//if (indexSample < 0.005 * length) {
		//	envelopeValue *= indexSample / (0.005 * length);
		//}
		//else if (indexSample > (1.0f - 0.005) * length) {
		//	envelopeValue *= (length - indexSample) / (0.005 * length);
		//}
	}

	return envelopeValue;
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

float Grain::hammingEnvelope(int indexSample, int length)
{
	float a = 0.54f;
	float b = 0.46f;
	float angle = 2.0f * juce::MathConstants<float>::pi * indexSample / (length - 1);
	return a - b * std::cos(angle);
}