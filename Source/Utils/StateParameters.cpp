/*
  ==============================================================================

	Parameters.cpp
	Created: 26 Feb 2023 12:38:46am
	Author:  user

  ==============================================================================
*/

#include "StateParameters.h"

StateParameters::StateParameters()
{
	sampleRate = 0;
	numChannels = 0;
	isAudioLoaded = false;
	numSamples = 0;
	isGrainsEmpty = true;
	//isAudioLoaded.addListener(this);


}

StateParameters::~StateParameters()
{
	audioFileBuffer = nullptr;
	grains = nullptr;
}

void StateParameters::init(ValueTreeState* apvts, int numChannels)
{
	this->numChannels = numChannels;
	setMix(apvts->getRawParameterValue(MIX_ID)->load());
	setGain(apvts->getRawParameterValue(GAIN_ID)->load());
	setDensity(apvts->getRawParameterValue(DENSITY_ID)->load());
	setDuration(apvts->getRawParameterValue(DURATION_ID)->load());
	setSpeed(apvts->getRawParameterValue(SPEED_ID)->load());
	setFilePosition(apvts->getRawParameterValue(POSITION_ID)->load());
	setWindowSelection(apvts->getRawParameterValue(SELECTION_ID)->load());
	setEnvWidth(apvts->getRawParameterValue(ENVWIDTH_ID)->load());
	setTraversalModeValue(apvts->getRawParameterValue(TRAVERSALMODE_ID)->load());
	setTraversalTimeValue(apvts->getRawParameterValue(TRAVERSALTIME_ID)->load());
}


// Le callback qui envoit newValue ne provient pas du apvts mais du slider associé au Listener
/*void StateParameters::parameterChanged(const juce::String& parameterID, float newValue)
{
	if (parameterID == MIX_ID) {
		mix = static_cast<int>(newValue / 100.f);
		juce::Logger::outputDebugString("StateParameters mix : " + (const juce::String)mix);
	}
	if (parameterID == GAIN_ID) {
		gain = pow(10, newValue / 20);
		juce::Logger::outputDebugString("StateParameters gain : " + (const juce::String)gain);
	}
	else if (parameterID == DENSITY_ID) {
		density = newValue;
		juce::Logger::outputDebugString("StateParameters density : " + (const juce::String)density);
		setInterOnSet();
	}
	else if (parameterID == DURATION_ID) {
		duration = newValue;
		juce::Logger::outputDebugString("StateParameters duration : " + (const juce::String)duration);
	}
	else if (parameterID == PITCH_ID) {
		pitch = static_cast<int>(newValue);
		juce::Logger::outputDebugString("StateParameters pitch : " + (const juce::String)pitch);
	}
}*/


// setters 

void StateParameters::setGain(float newValue)
{
	gain = juce::Decibels::decibelsToGain(newValue);
}

void StateParameters::setDensity(float newValue)
{
	density = newValue;
	setInterOnSet();
}

void StateParameters::setDuration(float newValue)
{
	duration = newValue;
}

void StateParameters::setMix(float newValue)
{
	mix = newValue / 100.f;

}

void StateParameters::setSpeed(float newValue)
{
	speed = newValue;
}

void StateParameters::setInterOnSet()
{
	interOnset = static_cast<int>(round(getSampleRate() / getDensity()));
}

void StateParameters::setEnvelopeType(int newValue)
{
	envelopeType = newValue;
}

void StateParameters::setSampleRate(double sampleRate) {
	this->sampleRate = sampleRate;
	setInterOnSet();
}

void StateParameters::setAudioLoaded(bool newValue)
{
	if (newValue == false)
	{
		audioFileBuffer = nullptr;
	}

	isAudioLoaded.setValue(newValue);
}

void StateParameters::setAudioBuffer(juce::AudioBuffer<float>* newValue)
{
	audioFileBuffer = newValue;
	if (newValue != nullptr)
		setNumSamples(audioFileBuffer->getNumSamples());
}

void StateParameters::setIsPlaying(bool newValue)
{
	isPlaying = newValue;
}

void StateParameters::setFilePosition(float newValue)
{
	filePosition = newValue;
}

void StateParameters::setWindowSelection(float newValue)
{
	windowSelection = newValue;
}

void StateParameters::setEnvWidth(float newValue)
{
	envWidth = newValue;
}

void StateParameters::setNumSamples(float newValue)
{
	numSamples = newValue;
}

void StateParameters::setIsGrainsEmpty(bool newValue)
{
	isGrainsEmpty = newValue;
}

void StateParameters::setTraversalModeValue(int newValue)
{
	traversalMode = newValue;
}

void StateParameters::setTraversalTimeValue(float newValue)
{
	traversalTime = newValue;
}

void StateParameters::setGrains(juce::Array<Grain*>* grains)
{
	this->grains = grains;
}

// getters

float StateParameters::getGain()
{
	return gain;
}

float StateParameters::getDensity()
{
	return density;
}

float StateParameters::getDuration()
{
	return duration;
}

float StateParameters::getSpeed()
{
	return speed;
}

double StateParameters::getSampleRate()
{
	return sampleRate;
}

float StateParameters::getMix()
{
	return mix;
}

int StateParameters::getInterOnset()
{
	return interOnset;
}

int StateParameters::getNumChannels()
{
	return numChannels;
}

int StateParameters::getEnvelopeType()
{
	return envelopeType;
}

juce::Value* StateParameters::getAudioLoaded()
{
	//return isAudioLoaded.getValue();
	return &isAudioLoaded;
}

bool StateParameters::getIsPlaying()
{
	return isPlaying;
}

juce::AudioBuffer<float>* StateParameters::getAudioBuffer()
{
	return audioFileBuffer;
}

float StateParameters::getWindowSelection()
{
	return windowSelection;
}

float StateParameters::getFilePosition()
{
	return filePosition;
}

float StateParameters::getEnvWidth()
{
	return envWidth;
}

float StateParameters::getNumSamples()
{
	return numSamples;
}

bool StateParameters::getIsGrainsEmpty()
{
	return isGrainsEmpty;
}

int StateParameters::getTraversalModeValue()
{
	return traversalMode;
}

float StateParameters::getTraversalTimeValue()
{
	return traversalTime;
}

juce::Array<Grain*>* StateParameters::getGrains()
{
	return grains;
}