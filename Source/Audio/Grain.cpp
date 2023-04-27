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
	case 4:
		window = WindowingMethod::rectangular;
		break;
	case 5:
		window = WindowingMethod::blackman;
		break;
	case 6:
		window = WindowingMethod::blackmanHarris;
		break;
	case 7:
		window = WindowingMethod::flatTop;
		break;
	default:
		break;
	}

	return window;
}


Grain::Grain(int duration, int numChannels, int envelopeType, float speed, int envelopeWidth, int position, juce::AudioBuffer<float>* buffer) :
	duration(duration),
	numChannels(numChannels),
	position(position),
	//selection(selection),
	speed(speed),
	envelopeWidth(static_cast<int>(envelopeWidth* duration)),
	buffer(buffer),
	envelopeType(getWindowingMethod(envelopeType)),
	fadeIn((duration - envelopeWidth) / 2),
	fadeOut(fadeIn + envelopeWidth),
	envelopeSize(duration - envelopeWidth)//, window(getWindowingMethod(envelopeType))
{
	currentTime = 0;
	// on resample le buffer en fonction de la vitesse en appliquant le phase vocoder algorithme

	//DBG("speed : " << speed);
	//float interval = 1.0f / speed;



}

Grain::~Grain()
{
	buffer = nullptr;
}

// on prend un grain du buffer du sample d'entré pour le mettre dans le buffer des grains
// si la duration est plus grande que la selection alors on loop 
float Grain::getCurrentSample(const int channel)
{

	// on veut récupérer le sample dans une fenetre de positionSamples à positionSamples + selectionSamples 
	//return grainBuffer.getSample(channel, currentTime);
	const float* sample = buffer->getReadPointer(channel % numChannels);


	// On a la position, la selection, l'envelopeWidth et la duration
	// On veut la position de l'index dans l'envelope
	// on vérifie la position du grain a extraire
	int readPosition = currentTime + position;
	readPosition %= buffer->getNumSamples();

	grainPoint.setSamplePos(readPosition);

	// on applique l'envelope sur le grain en fonction de l'envelope type
	float sampleValue = 0.0f;

	if (currentTime < fadeIn)
	{
		sampleValue = sample[readPosition] * applyEnvelope(currentTime);
	}
	else if (fadeOut <= currentTime)
	{
		sampleValue = sample[readPosition] * applyEnvelope(duration - currentTime);
	}
	else
	{
		sampleValue = sample[readPosition];
	}

	float x = std::clamp(sampleValue, 0.f, 1.f);
	grainPoint.setOpacity(curve(x, 5.f));
	return sampleValue;

}


void Grain::update()
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

float Grain::applyEnvelope(const int index)
{

	switch (envelopeType)
	{
	case hann:
	{
		return hannEnvelope(index);
	}
	break;

	case triangular:
	{
		return triangularEnvelope(index);
	}
	break;
	case hamming:
	{
		return hammingEnvelope(index);
	}
	break;

	case rectangular:
	{
		return rectangularEnvelope(index);
	}
	break;
	case blackman:
	{
		return blackmanEnvelope(index);
	}
	break;

	case blackmanHarris:
	{
		return blackmanHarrisEnvelope(index);
	}
	break;
	case flatTop:
	{
		return flatTopEnvelope(index);
	}
	break;
	default:
		jassertfalse;
		break;
	}
}

// envelope types computations
// https://en.wikipedia.org/wiki/Window_function#A_list_of_window_functions

float Grain::ncos(size_t order, size_t i, size_t size)
{
	return std::cos(static_cast<float> (order * i)
		* juce::MathConstants<float>::pi / static_cast<float> (size - 1));
}

float Grain::hannEnvelope(const int index)
{
	float cos2 = ncos(2, index, envelopeSize);
	return (0.5 - 0.5 * cos2);

}

float Grain::triangularEnvelope(const int index)
{
	float halfSlots = static_cast<float> (0.5) * static_cast<float> (envelopeSize - 1);
	return static_cast<float> (1.0) - std::abs((static_cast<float> (index) - halfSlots) / halfSlots);
}

float Grain::rectangularEnvelope(const int index)
{
	return static_cast<float> (0);
}

float Grain::hammingEnvelope(const int index)
{
	float cos2 = ncos(2, index, envelopeSize);
	return static_cast<float> (0.54 - 0.46 * cos2);
}

float Grain::blackmanEnvelope(const int index)
{
	constexpr float alpha = 0.16f;
	float cos2 = ncos(2, index, envelopeSize);
	float cos4 = ncos(4, index, envelopeSize);

	return static_cast<float> (0.5 * (1 - alpha) - 0.5 * cos2 + 0.5 * alpha * cos4);
}

float Grain::blackmanHarrisEnvelope(const int index)
{
	float cos2 = ncos(2, index, envelopeSize);
	float cos4 = ncos(4, index, envelopeSize);
	float cos6 = ncos(6, index, envelopeSize);

	return static_cast<float> (0.35875 - 0.48829 * cos2 + 0.14128 * cos4 - 0.01168 * cos6);
}

float Grain::flatTopEnvelope(const int index)
{

	float cos2 = ncos(2, index, envelopeSize);
	float cos4 = ncos(4, index, envelopeSize);
	float cos6 = ncos(6, index, envelopeSize);
	float cos8 = ncos(8, index, envelopeSize);

	return static_cast<float> (1.0 - 1.93 * cos2 + 1.29 * cos4 - 0.388 * cos6 + 0.028 * cos8);

}

GrainPoint* Grain::getGrainPoint()
{
	return &grainPoint;
}

float Grain::curve(float x, float coefficient)
{
	return 1.0f - std::pow(1.0f - x, coefficient);
}


//float Grain::logInterpolation(float x)
//{
//	return std::log10(1 + x) / std::log10(2);
//}