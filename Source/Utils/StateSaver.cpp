#include "StateSaver.h"

StateSaver::StateSaver() : sampleRate(44100.0f), position(0.0f), density(0.0f), grainSize(1.0f),
speed(1.0f), isAudioLoaded(false)
{

};

StateSaver::~StateSaver()
{

};

void StateSaver::setAudioLoaded(bool b) {
	juce::Logger::outputDebugString("StateSaver.isAudioLoaded = " + b);
	isAudioLoaded = b;
}
bool StateSaver::getAudioLoaded() {
	return isAudioLoaded;
}

void StateSaver::setSampleRate(double sampleRate) {
	this->sampleRate = sampleRate;
	juce::Logger::outputDebugString("StateSaver::setSampleRate => " + (juce::String)this->sampleRate);
}
double StateSaver::getSampleRate() {
	return sampleRate;
}