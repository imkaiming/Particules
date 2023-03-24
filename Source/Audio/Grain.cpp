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


Grain::Grain(int duration, int numChannel, int envelopeType, float speed, int width, int position, int selection, juce::AudioBuffer<float>* buffer) :
	duration(duration), numChannel(numChannel), position(position), selection(selection),
	window(getWindowingMethod(envelopeType)), speed(speed), width(width),
	fileBuffer(buffer)
{
	// on resample le buffer en fonction de la vitesse en appliquant le phase vocoder algorithme

	currentTime = 0;
	//active = true; // un grain est créer lorsqu'il est ajouté dans le vector dont il est joué tout de suite
}

Grain::~Grain()
{
	fileBuffer = nullptr;
}

void Grain::updateBuffer(juce::AudioBuffer<float>* buffer) 
{
	fileBuffer = buffer;
}



// on prend un grain du buffer du sample d'entré pour le mettre dans le buffer des grains
// si la duration est plus grande que la selection alors on loop 
float Grain::getCurrentSample(int channel)
{

	// on veut récupérer le sample dans une fenetre de positionSamples à positionSamples + selectionSamples 
	return fileBuffer->getSample(channel % numChannel, position + currentTime);
}


void Grain::applyCrossFade(int crossfade, bool atStart)
{
	// crossfade de début
	if (atStart) {
		// si la longueur restante du grain précédent excède la valeur total du grain 
		// qui démarre alors on prend la valeur minimale
		crossfade = std::min(crossfade, this->duration - 1);
		// lorsqu'on va copier le requerir le sample à cet index son amplitude sera déjà modifié
		fileBuffer->applyGainRamp(0, crossfade, 0, 1);
	}
	// crossfade de fin
	else {
		fileBuffer->applyGainRamp(this->duration - crossfade, crossfade, 1, 0);
	}
}

void Grain::update() // int channel)
{
	currentTime++;
}

int Grain::remainingLife()
{
	return duration - currentTime;
}

bool Grain::isActive()
{
	if (currentTime >= duration) {
		return false;
	}

	return true;
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

		sample* hannEnvelope(currentTime, duration);
		break;
	case triangular:
		//float halfSlots = static_cast<float> (0.5) * static_cast<float> (length - 1);
		//sample = static_cast<float> (1.0) - std::abs((static_cast<float> (currentTime) - halfSlots) / halfSlots);
		// 
		//sample* trapezoidalEnvelope(currentTime, length, 1.0f);

		sample* triangularEnvelope(currentTime, duration);
		break;

	case hamming:
		//float cos2 = ncos(2, currentTime, length);
		//sample = static_cast<float> (0.54 - 0.46 * cos2);
		sample* hammingEnvelope(currentTime, duration);
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