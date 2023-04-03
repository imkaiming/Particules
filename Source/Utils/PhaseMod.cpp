/*
  ==============================================================================

	PhaseMod.cpp
	Created: 3 Apr 2023 12:02:48am
	Author:  user

  ==============================================================================
*/

#include "PhaseMod.h"
#include "ParamsID.h"

PhaseMod::PhaseMod(double sampleRate) : sampleRate(sampleRate)
{
	setFrequency(TRAVERSALTIME_DEFAULT);
	setMod(TRAVERSALMODE_DEFAULT);
	reset();
}

PhaseMod::PhaseMod(double sampleRate, float frequency, int mod) : sampleRate(sampleRate), frequency(frequency), mod(mod)
{
	updateDelta();
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

void PhaseMod::setMod(int newValue)
{
	mod = newValue;
	reset();
}

void PhaseMod::updateDelta()
{
	delta = frequency / static_cast<float>(sampleRate);
	DBG("delta : " << delta);
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
	phase.reset();
	value = 0.f;
}

void PhaseMod::advance()
{
	phase.advance(delta);
	switch (mod) {
	case 1:
		nextSine();
		break;
	case 2:
		nextTriangular();
		break;
	case 3:
		nextSquare();
		break;
	case 4:
		nextRandom();
		break;
	case 5:
		value = 0;
		break;
	default:
		break;
	}
}

void PhaseMod::nextSine()
{
	// (sin(2pix) + 1) / 2
	value = (std::sin(juce::MathConstants<float>::twoPi * phase.phase) + 1) / 2;
}

void PhaseMod::nextTriangular()
{
	// 2|x - floor(x + 0.5)| 
	value = 2 * std::abs(phase.phase - std::floorf(phase.phase + 0.5f));
}

void PhaseMod::nextSquare()
{
	value = phase.phase < 0.5f ? 0.f : 1.f;
}

void PhaseMod::nextRandom()
{
	value = r.nextFloat();
}

float PhaseMod::getValue()
{
	return value;
}
//void PhaseMod::nextPhase()
//{
//	phase += juce::MathConstants<float>::twoPi * delta;
//	if (phase > juce::MathConstants<float>::twoPi)
//		phase -= juce::MathConstants<float>::twoPi;
//}