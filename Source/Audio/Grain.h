/*
  ==============================================================================

	Grain.h
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Utils/GrainPoint.h"
#include <juce_dsp/juce_dsp.h>

enum WindowingMethod
{
	triangular,
	hann,
	hamming,
	rectangular,
	blackman,
	blackmanHarris,
	flatTop
};

class Grain
{

public:
	Grain(int duration, int numChannel, int envelopeType, float speed, int envelopeWidth, int position, juce::AudioBuffer<float>* buffer);
	~Grain();
	float getCurrentSample(const int channel);
	bool isActive();
	void update();

	GrainPoint* getGrainPoint();

private:

	// envelope type function
	float ncos(size_t, size_t, size_t);
	float hammingEnvelope(const int index);
	float triangularEnvelope(const int index);
	float hannEnvelope(int index);
	float rectangularEnvelope(const int index);
	float blackmanEnvelope(const int index);
	float blackmanHarrisEnvelope(const int index);
	float flatTopEnvelope(const int index);
	float applyEnvelope(const int index);

	WindowingMethod getWindowingMethod(int);

	const int envelopeType;	// on associe un grain a une envelope

	int currentTime;		// le compteur interne du grain
	const int numChannels;	// le grain est le même pour chaque channel
	float speed;
	const int duration;		// définie la durée en nombre de sample
	const int position;		// définie la position en sample dans le buffer
	// const int selection;	// définie la position maximale qu'un grain peut atteindre dans le buffer
	const int envelopeWidth;// définie la taille des rampes d'amplitude en entré et en sortie du grain


	const int fadeIn;		// 0 to fadeIn
	const int fadeOut;		// fadeOut to numSamples
	const int envelopeSize;	// utile pour calculer les fade d'entrés et de sorties des envelopes selon les functions données


	juce::AudioBuffer<float>* buffer;
	//double frequency;
	//float amplitude;

	GrainPoint grainPoint;
};

