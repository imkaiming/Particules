/*
  ==============================================================================

	Grain.cpp
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#include "Grain.h"
#include "../framework/ParameterSnapshot.h"
#include "../framework/SampleSource.h"

WindowingMethod Grain::getWindowingMethod(int val)
{
    WindowingMethod window = WindowingMethod::hann;
    switch(val)
    {
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

Grain::Grain()
    : durationSamples{0}, startPosition{0}, speed{1.f}, sustainWidth{0}, envelopeType{0}, envelopeSize{0}, fadeIn{0}, fadeOut{0},
      elapsedSamples{0}, offset{0}, linearGain{1.f}
{
    reset();
}

void Grain::reset()
{
    durationSamples = 0;
    startPosition = 0;
    speed = 1.f;
    fadeIn = 0;
    fadeOut = 0;
    offset = 0;
    sustainWidth = 0;
    envelopeType = 0;
    envelopeSize = 0;
    elapsedSamples = 0;
    generation = 0;
    linearGain = 1.f;
}

void Grain::config(const ParameterSnapshot& snapshot, int sample)
{
    // TODO convert to int ? or keep float
    //durationSamples = static_cast<int>(snapshot.durationSamples);
    //startPosition = static_cast<int>(snapshot.startPosition);
    //sustainWidth = static_cast<int>(snapshot.sustainWidth * durationSamples);
    elapsedSamples = 0;

    durationSamples = snapshot.duration* snapshot.sampleRate;
    startPosition = snapshot.position * snapshot.sampleRate;
    selectionWindow = snapshot.selection * snapshot.sampleRate;
    linearGain = snapshot.linearGain;
    speed = snapshot.speed;
    offset = sample;

    //fadeIn = static_cast<int>((snapshot.durationSamples - envelopeWidth) / 2.0);
    sustainWidth = snapshot.sustainRatio * durationSamples;
    fadeIn = (durationSamples - sustainWidth) / 2.0;
    fadeOut = fadeIn + sustainWidth;
    envelopeSize = durationSamples - sustainWidth;

    envelopeType = getWindowingMethod(static_cast<int>(snapshot.envType));
    traversalMode = snapshot.traversalMode;
    traversalTime = snapshot.traversalTime;
    //gain = snapshot.gain;

    //TODO@ precalculer la table d'enveloppe et la storer en local
    //make it not possible to change the envelop type while grain is active
}

float Grain::getCurrentSample(const SampleSource* source, const int channel, const int outNumChannel) noexcept
{
    if(offset > 0)
    {
        offset--;
        return 0.f;
    }

    const AudioBuffer& inputbuffer = source->inputBuffer;
    const int sourceChannel = source->numChannels;
    const int inputNumSamples = inputbuffer.getNumSamples();
    const float* sample = inputbuffer.getReadPointer(channel % sourceChannel);

    int readPosition = startPosition + static_cast<int>(elapsedSamples * speed);
    if(readPosition >= inputNumSamples)
        readPosition -= inputNumSamples;

    //grainPoint.setSamplePos(readPosition);

    float sampleValue = 0.0f;

    if(elapsedSamples < fadeIn)
        sampleValue = sample[readPosition] * applyEnvelope(elapsedSamples);
    else if(fadeOut <= elapsedSamples)
        sampleValue = sample[readPosition] * applyEnvelope(durationSamples - elapsedSamples);
    else
        sampleValue = sample[readPosition];

    float x = std::clamp((sampleValue * 0.5f + 0.5f), 0.f, 1.f);
    grainPoint.setOpacity(curve(x, 5.f));

    return sampleValue * linearGain;
}

float Grain::applyEnvelope(const int index)
{
    switch(envelopeType)
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
            throw std::logic_error("wrong case for enveloppe calculation");
            return -1.0f;
            break;
    }
}

// envelope types computations
// https://en.wikipedia.org/wiki/Window_function#A_list_of_window_functions

float Grain::ncos(size_t order, size_t i, size_t size)
{
    return std::cos(static_cast<float>(order * i) * juce::MathConstants<float>::pi / static_cast<float>(size - 1));
}

float Grain::hannEnvelope(const int index)
{
    float cos2 = ncos(2, index, envelopeSize);
    return (0.5f - 0.5f * cos2);
}

float Grain::triangularEnvelope(const int index)
{
    float halfSlots = static_cast<float>(0.5) * static_cast<float>(envelopeSize - 1);
    return static_cast<float>(1.0) - std::abs((static_cast<float>(index) - halfSlots) / halfSlots);
}

float Grain::rectangularEnvelope(const int index) { return static_cast<float>(1); }

float Grain::hammingEnvelope(const int index)
{
    float cos2 = ncos(2, index, envelopeSize);
    return static_cast<float>(0.54 - 0.46 * cos2);
}

float Grain::blackmanEnvelope(const int index)
{
    constexpr float alpha = 0.16f;
    float cos2 = ncos(2, index, envelopeSize);
    float cos4 = ncos(4, index, envelopeSize);

    return static_cast<float>(0.5 * (1 - alpha) - 0.5 * cos2 + 0.5 * alpha * cos4);
}

float Grain::blackmanHarrisEnvelope(const int index)
{
    float cos2 = ncos(2, index, envelopeSize);
    float cos4 = ncos(4, index, envelopeSize);
    float cos6 = ncos(6, index, envelopeSize);

    return static_cast<float>(0.35875 - 0.48829 * cos2 + 0.14128 * cos4 - 0.01168 * cos6);
}

float Grain::flatTopEnvelope(const int index)
{
    float cos2 = ncos(2, index, envelopeSize);
    float cos4 = ncos(4, index, envelopeSize);
    float cos6 = ncos(6, index, envelopeSize);
    float cos8 = ncos(8, index, envelopeSize);

    return static_cast<float>(1.0 - 1.93 * cos2 + 1.29 * cos4 - 0.388 * cos6 + 0.028 * cos8);
}

GrainPoint* Grain::getGrainPoint() { return &grainPoint; }

float Grain::curve(float x, float coefficient) { return 1.0f - std::pow(1.0f - x, coefficient); }

//float Grain::logInterpolation(float x)
//{
//	return std::log10(1 + x) / std::log10(2);
//}