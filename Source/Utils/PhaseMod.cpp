/*
  ==============================================================================

	PhaseMod.cpp
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

#include "PhaseMod.h"
#include "ParamsID.h"


//PhaseMod::PhaseMod(double sampleRate) : sampleRate(sampleRate)
//{
//	setFrequency(TRAVERSALTIME_DEFAULT);
//	setMod(TRAVERSALMODE_DEFAULT);
//	reset();
//}
//
//PhaseMod::PhaseMod(double sampleRate, float frequency, int mod) : sampleRate(sampleRate), frequency(frequency), mod(mod)
//{
//	updateDelta();
//	reset();
//}

PhaseMod::PhaseMod()
{
	reset();
}

PhaseMod::~PhaseMod()
{
}

void PhaseMod::setFrequency(float newValue)
{
	frequency = newValue;
	updateDelta();
}

void PhaseMod::setSampleRate(double newValue) {
	jassert(newValue != 0);
	sampleRate = newValue;
}

void PhaseMod::setMod(int newValue)
{
	mod = newValue;
	//reset();
}

void PhaseMod::updateDelta()
{
	delta = frequency / static_cast<float>(sampleRate);
}

float PhaseMod::getFrequency()
{
	return frequency;
}

int PhaseMod::getMod()
{
	return mod;
}

void PhaseMod::reset()
{
	phase = 0.f;
	value = 0.f;
}

void PhaseMod::advance()
{
	switch (mod) {
	case 1:
		value = nextSine(phase);
		break;
	case 2:
		value = nextTriangular(phase);
		break;
	case 3:
		value = nextSquare(phase);
		break;
	case 4:
		value = nextRandom();
		break;
	case 5:
		value = 0;
		break;
	default:
		break;
	}

	phase += delta;
	while (phase >= twoPi)
		phase -= twoPi;
}

float PhaseMod::nextSine(float phase)
{
	float offset = pi / 2; // so it start à pos 0
	return (std::sin(juce::MathConstants<float>::twoPi * phase + offset) + 1) / 2;
}

float PhaseMod::nextTriangular(float phase)
{
	// 2|x - floor(x + 0.5)| 
	return 2 * std::abs(phase - std::floorf(phase + 0.5f));
}

float PhaseMod::nextSquare(float phase)
{
	return phase < (twoPi / 2) ? 0.f : 1.f;
}

float PhaseMod::nextRandom()
{
	return r.nextFloat();
}

float PhaseMod::getValue()
{
	return value;
}
