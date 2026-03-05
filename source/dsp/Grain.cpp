/*
  ==============================================================================

	Grain.cpp
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#include "Grain.h"
#include "../framework/ParameterSnapshot.h"

//WindowingMethod Grain::getWindowingMethod(int val)
//{
//    WindowingMethod window = WindowingMethod::hann;
//    switch(val)
//    {
//        case 1:
//            window = WindowingMethod::hann;
//            break;
//        case 2:
//            window = WindowingMethod::triangular;
//            break;
//        case 3:
//            window = WindowingMethod::hamming;
//            break;
//        case 4:
//            window = WindowingMethod::rectangular;
//            break;
//        case 5:
//            window = WindowingMethod::blackman;
//            break;
//        case 6:
//            window = WindowingMethod::blackmanHarris;
//            break;
//        case 7:
//            window = WindowingMethod::flatTop;
//            break;
//        default:
//            break;
//    }
//
//    return window;
//}

Grain::Grain()
    : durationSamples{0}, startPositionSamples{0}, speed{1.f}, sustainWidthSamples{0}, envelopeType{0}, envelopeSizeSamples{0},
      selectionWindow{0}, fadeIn{0}, fadeOut{0}, elapsedSamples{0}, offset{0}//, linearGain{1.f}
{
    reset();
}

void Grain::reset()
{
    durationSamples = 0;
    startPositionSamples = 0;
    sustainWidthSamples = 0;
    envelopeSizeSamples = 0;
    elapsedSamples = 0;
    fadeIn = 0;
    fadeOut = 0;
    offset = 0;
    envelopeType = 0;
    generation = 0;

    //linearGain = 1.f;
    speed = 1.f;
}

void Grain::config(const ParameterSnapshot& snapshot, int delaySample, float posMod)
{
    elapsedSamples = 0;
    durationSamples = snapshot.durationSamples;

    const int positionModulationSamples = static_cast<int>(posMod * snapshot.selectionSamples);
    startPositionSamples = snapshot.startPositionSamples + positionModulationSamples;
    selectionWindow = snapshot.selectionSamples;

    speed = snapshot.speed;
    offset = delaySample;

    sustainWidthSamples = static_cast<int>(snapshot.sustainRatio * durationSamples);
    fadeIn = static_cast<int>((durationSamples - sustainWidthSamples) / 2.0);
    fadeOut = fadeIn + sustainWidthSamples;
    envelopeSizeSamples = durationSamples - sustainWidthSamples;

    envelopeType = static_cast<int>(snapshot.envType);

    envelopeTable.resize((size_t)durationSamples);
    computeEnvelope(envelopeTable);

    //linearGain = snapshot.linearGain;

    //TODO@ precalculer la table d'enveloppe et la storer en local
    //make it not possible to change the envelop type while grain is active
}

float Grain::getCurrentSample(const AudioBuffer* inputbuffer, const int channel, const int outNumChannel) noexcept
{
    if(offset > 0)
    {
        offset--;
        return 0.f;
    }

    if(isExhausted())
        return 0.f;

    const int sourceChannel = inputbuffer->getNumChannels();
    const int inputNumSamples = inputbuffer->getNumSamples();
    const float* sample = inputbuffer->getReadPointer(channel % sourceChannel);

    float readPosition = startPositionSamples + elapsedSamples * speed;
    while(readPosition >= inputNumSamples)
        readPosition -= inputNumSamples;

    //grainPoint.setSamplePos(readPosition);
    int index = static_cast<int>(readPosition);
    float frac = readPosition - index;
    const float s0 = sample[index];
    const float s1 = sample[(index + 1) % inputNumSamples];

    float sampleValue = lerp(s0, s1, frac) * envelopeTable[elapsedSamples]; // need to provide

    float x = std::clamp((sampleValue * 0.5f + 0.5f), 0.f, 1.f);
    grainPoint.setOpacity(curve(x, 5.f));

    return sampleValue;
}

void Grain::computeEnvelope(std::vector<float>& table)
{
    switch(envelopeType)
    {
        case 0:
            hannEnvelope(table);
            break;
        case 1:
            triangularEnvelope(table);
            break;
        case 2:
            hammingEnvelope(table);
            break;
        case 3:
            rectangularEnvelope(table);
            break;
        case 4:
            blackmanEnvelope(table);
            break;
        case 5:
            blackmanHarrisEnvelope(table);
            break;
        case 6:
            flatTopEnvelope(table);
            break;
        default:
            jassertfalse;
    }
}

float Grain::ncos(const int order, const int i, const int size) const noexcept
{
    return std::cos((float)order * pi * (float)i / (float)(size - 1));
}

void Grain::hannEnvelope(std::vector<float>& table)
{
    for(int i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            const float cos2 = ncos(2, i, envelopeSizeSamples);
            table[i] = (0.5f - 0.5f * cos2);
        }
        else if(i >= fadeOut)
        {
            const float cos2 = ncos(2, i - sustainWidthSamples, envelopeSizeSamples);
            table[i] = (0.5f - 0.5f * cos2);
        }
        else
        {
            table[i] = 1.f;
        }
    }
}

void Grain::triangularEnvelope(std::vector<float>& table)
{
    float halfSlots = 0.5f * (envelopeSizeSamples - 1);
    for(int i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            table[i] = 1.f - std::abs((static_cast<float>(i - halfSlots) / halfSlots));
        }
        else if(i >= fadeOut)
        {
            table[i] = 1.f - std::abs((static_cast<float>(i - sustainWidthSamples - halfSlots) / halfSlots));
        }
        else
        {
            table[i] = 1.f;
        }
    }
}

void Grain::rectangularEnvelope(std::vector<float>& table)
{
    for(int i = 0; i < table.size(); i++)
    {
        table[i] = 1.f;
    }
}

void Grain::hammingEnvelope(std::vector<float>& table)
{
    for(int i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            const float cos2 = ncos(2, i, envelopeSizeSamples);
            table[i] = static_cast<float>(0.54f - 0.46f * cos2);
        }
        else if(i >= fadeOut)
        {
            const float cos2 = ncos(2, i - sustainWidthSamples, envelopeSizeSamples);
            table[i] = static_cast<float>(0.54f - 0.46f * cos2);
        }
        else
        {
            table[i] = 1.f;
        }
    }
}

void Grain::blackmanEnvelope(std::vector<float>& table)
{
    const float alpha = 0.16f;
    for(int i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            const float cos2 = ncos(2, i, envelopeSizeSamples);
            const float cos4 = ncos(4, i, envelopeSizeSamples);
            table[i] = 0.5f * (1 - alpha) - 0.5 * cos2 + 0.5 * alpha * cos4;
        }
        else if(i >= fadeOut)
        {
            const float cos2 = ncos(2, i - sustainWidthSamples, envelopeSizeSamples);
            const float cos4 = ncos(4, i - sustainWidthSamples, envelopeSizeSamples);
            table[i] = 0.5f * (1 - alpha) - 0.5 * cos2 + 0.5 * alpha * cos4;
        }
        else
        {
            table[i] = 1.f;
        }
    }
}

void Grain::blackmanHarrisEnvelope(std::vector<float>& table)
{
    for(int i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            const float cos2 = ncos(2, i, envelopeSizeSamples);
            const float cos4 = ncos(4, i, envelopeSizeSamples);
            const float cos6 = ncos(6, i, envelopeSizeSamples);
            table[i] = 0.35875f - 0.48829f * cos2 + 0.14128f * cos4 - 0.01168f * cos6;
        }
        else if(i >= fadeOut)
        {
            const float cos2 = ncos(2, i - sustainWidthSamples, envelopeSizeSamples);
            const float cos4 = ncos(4, i - sustainWidthSamples, envelopeSizeSamples);
            const float cos6 = ncos(6, i - sustainWidthSamples, envelopeSizeSamples);
            table[i] = 0.35875f - 0.48829f * cos2 + 0.14128f * cos4 - 0.01168f * cos6;
        }
        else
        {
            table[i] = 1.f;
        }
    }
}

void Grain::flatTopEnvelope(std::vector<float>& table)
{
    for(int i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            const float cos2 = ncos(2, i, envelopeSizeSamples);
            const float cos4 = ncos(4, i, envelopeSizeSamples);
            const float cos6 = ncos(6, i, envelopeSizeSamples);
            const float cos8 = ncos(8, i, envelopeSizeSamples);
            table[i] = 1.f - 1.93f * cos2 + 1.29f * cos4 - 0.388f * cos6 + 0.028f * cos8;
        }
        else if(i >= fadeOut)
        {
            const float cos2 = ncos(2, i - sustainWidthSamples, envelopeSizeSamples);
            const float cos4 = ncos(4, i - sustainWidthSamples, envelopeSizeSamples);
            const float cos6 = ncos(6, i - sustainWidthSamples, envelopeSizeSamples);
            const float cos8 = ncos(8, i - sustainWidthSamples, envelopeSizeSamples);
            table[i] = 1.f - 1.93f * cos2 + 1.29f * cos4 - 0.388f * cos6 + 0.028f * cos8;
        }
        else
        {
            table[i] = 1.f;
        }
    }
}

GrainPoint* Grain::getGrainPoint() { return &grainPoint; }

float Grain::curve(float x, float coefficient) { return 1.0f - std::pow(1.0f - x, coefficient); }

//float Grain::logInterpolation(float x)
//{
//	return std::log10(1 + x) / std::log10(2);
//}