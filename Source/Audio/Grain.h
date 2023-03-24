/*
  ==============================================================================

	Grain.h
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <juce_dsp/juce_dsp.h>

enum WindowingMethod
{
	triangular,
	hann,
	hamming
};

class Grain {

public:
	Grain(int duration, int numChannel, int envelopeType, float speed, int envelopeWidth, int position, int selection, juce::AudioBuffer<float>* buffer);
	~Grain();
	float getCurrentSample(int);
	bool isActive();
	void update();
	void applyCrossFade(int, bool);
	//int remainingLife();
	void updateBuffer(juce::AudioBuffer<float>*);

private:


	//float parabolicEnvelope(int, int, float);
	//float trapezoidalEnvelope(int, int, float);
	//float raisedCosineBellEnvelope(int, int, float);

	float ncos(size_t, size_t, size_t);
	float hammingEnvelope(int index, int length);
	float triangularEnvelope(int index, int length);
	float hannEnvelope(int index, int length);
	void applyEnvelope();

	WindowingMethod getWindowingMethod(int);

	const int envelopeType;	// on associe un grain a une envelope

	int currentTime;		// le compteur interne du grain
	const int numChannels;	// le grain est le même pour chaque channel
	float speed;
	const int duration;		// définie la durée en nombre de sample
	const int position;		// définie la position en sample dans le buffer
	const int selection;	// définie la position maximale qu'un grain peut atteindre dans le buffer
	const int envelopeWidth;		// définie la taille des rampes d'amplitude en entré et en sortie du grain
	juce::AudioBuffer<float> grainBuffer;
	//double startTime;
	//double frequency;
	//float amplitude;

};

