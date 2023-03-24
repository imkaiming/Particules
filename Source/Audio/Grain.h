/*
  ==============================================================================

	Grain.h
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include <juce_dsp/juce_dsp.h>

enum WindowingMethod
{
	triangular,
	hann,
	hamming
};

class Grain {

public:
	Grain(int duration, int numChannel, int envelopeType, float speed, int width, int position, int selection, juce::AudioBuffer<float>* buffer);
	~Grain();
	float getCurrentSample(int);
	bool isActive();
	void update();
	void applyCrossFade(int, bool);
	int remainingLife();
	void updateBuffer(juce::AudioBuffer<float>*);

private:

	float ncos(size_t, size_t, size_t);

	float parabolicEnvelope(int, int, float);
	float trapezoidalEnvelope(int, int, float);
	float raisedCosineBellEnvelope(int, int, float);

	float hammingEnvelope(int,  int) ;
	float triangularEnvelope(int,  int);
	float hannEnvelope(int, int);
	float applyEnvelope(float);

	WindowingMethod getWindowingMethod(int);
	WindowingMethod window;
	//GrainEnvelope grainEnv;
	//const int envelopeType;	// on associe un grain a une envelope

	//juce::dsp::WindowingFunction<float>::

	int currentTime;		// le compteur interne du grain
	const int numChannel;	// le grain est le même pour chaque channel
	float speed;
	const int duration;		// définie la durée en nombre de sample
	const int position;		// définie la position en sample dans le buffer
	const int selection;	// définie la position maximale qu'un grain peut atteindre dans le buffer
	const int width;		// définie la taille des rampes d'amplitude en entré et en sortie du grain

	juce::AudioBuffer<float>* fileBuffer;
	//double startTime;
	//double frequency;
	//float amplitude;

};

