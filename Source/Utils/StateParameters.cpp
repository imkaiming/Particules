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
	audioFileNumSamples = 0;
	//isAudioLoaded.addListener(this);


}

StateParameters::~StateParameters()
{

}

void StateParameters::init(ValueTreeState* apvts, int numChannels)
{
	setMix(apvts->getRawParameterValue(MIX_ID)->load());
	setGain(apvts->getRawParameterValue(GAIN_ID)->load());
	//smoothedGain.setValue(apvts->getRawParameterValue(GAIN_ID)->load());
	setDensity(apvts->getRawParameterValue(DENSITY_ID)->load());
	setDuration(apvts->getRawParameterValue(DURATION_ID)->load());
	setSpeed(apvts->getRawParameterValue(SPEED_ID)->load());
	setFilePosition(apvts->getRawParameterValue(POSITION_ID)->load());
	setWindowSelection(apvts->getRawParameterValue(POSWIDTH_ID)->load());
	this->numChannels = numChannels;
}

void StateParameters::print() {
	juce::Logger::outputDebugString("stateparams mix : " + (juce::String)mix);
	juce::Logger::outputDebugString("stateparams gain : " + (juce::String)gain);
	juce::Logger::outputDebugString("stateparams dens : " + (juce::String)density);
	juce::Logger::outputDebugString("stateparams dur : " + (juce::String)duration);
	juce::Logger::outputDebugString("stateparams speed : " + (juce::String)speed);
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
	//smoothedGain.setTargetValue(newValue);
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
	//juce::Logger::outputDebugString((juce::String)mix);

}

void StateParameters::setSpeed(float newValue)
{
	//juce::Logger::outputDebugString("speed is " + (juce::String)speed);
	speed = newValue;
}

void StateParameters::setInterOnSet()
{
	interOnset = static_cast<int>(round(getSampleRate() / getDensity()));
}

void StateParameters::setEnvelopeType(int newValue)
{
	envelopeType = newValue;
	//juce::Logger::outputDebugString((juce::String)envelopeType);
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

// getters

float StateParameters::getGain()
{
	return gain;
	//return smoothedGain.getNextValue();
}

float StateParameters::getDensity() {
	return density;
}

float StateParameters::getDuration() {
	return duration;
}

float StateParameters::getSpeed()
{
	return speed;
}

double StateParameters::getSampleRate() {
	return sampleRate;
}

float StateParameters::getMix() {
	return mix;
}

int StateParameters::getInterOnset()
{
	return interOnset;
}

int StateParameters::getNumChannels() {
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