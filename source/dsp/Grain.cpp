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
      selectionWindow{0}, fadeIn{0}, fadeOut{0}, elapsedSamples{0}, offset{0}, readPosition{0.f}, inputNumChannels{0},
      inputNumSamples{0} //, linearGain{1.f}
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
    inputNumSamples = 0;
    //linearGain = 1.f;
    speed = 1.f;
    readPosition = 0.f;
}

void Grain::config(const ParameterSnapshot& snapshot, int delaySample, float posMod)
{
    elapsedSamples = 0;
    durationSamples = snapshot.durationSamples;
    inputNumSamples = snapshot.inputNumSamples;
    inputNumChannels = snapshot.inputNumChannels;
    const int positionModulationSamples = static_cast<int>(posMod * snapshot.selectionSamples);
    startPositionSamples = snapshot.startPositionSamples + positionModulationSamples;
    readPosition = static_cast<float>(startPositionSamples);
    selectionWindow = snapshot.selectionSamples;

    speed = snapshot.speed;
    offset = delaySample;

    sustainWidthSamples = static_cast<int>(snapshot.sustainRatio * durationSamples);
    fadeIn = static_cast<int>((durationSamples - sustainWidthSamples) / 2.0);
    fadeOut = fadeIn + sustainWidthSamples;
    envelopeSizeSamples = durationSamples - sustainWidthSamples;

    envelopeType = static_cast<int>(snapshot.envType);

    envelopeTable.resize((size_t)durationSamples + offset);
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

    // TODO mettre ca dans le VM idealement
    const int sourceChannel = inputbuffer->getNumChannels();
    const int inputNumSamples = inputbuffer->getNumSamples();
    const float* sample = inputbuffer->getReadPointer(channel % sourceChannel);

    //grainPoint.setSamplePos(readPosition);
    int index = static_cast<int>(readPosition);
    float frac = readPosition - (float)index;
    const float s0 = sample[index];
    const float s1 = sample[(index + 1) % inputNumSamples];

    float sampleValue = lerp(s0, s1, frac) * envelopeTable[elapsedSamples];

    float x = std::clamp((sampleValue * 0.5f + 0.5f), 0.f, 1.f);
    grainPoint.setOpacity(curve(x, 5.f));

    return sampleValue;
}

void Grain::update()
{
    elapsedSamples++;
    readPosition += speed;
    while(readPosition >= inputNumSamples)
        readPosition -= inputNumSamples;
}

void Grain::computeEnvelope(std::vector<float>& table)
{
    if(table.size() == 0)
        return;

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
    return std::cos(2.0f * pi * (float)order * (float)i / (float)(size - 1.f));
}

void Grain::hannEnvelope(std::vector<float>& table)
{
    const float phase = 2.f * pi / ((float)envelopeSizeSamples - 1.f);
    for(size_t i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            table[i] = 0.5f - 0.5f * std::cos(phase * (float)i);
        }
        else if(i >= fadeOut)
        {
            const float out = i - (float)sustainWidthSamples;
            table[i] = 0.5f - 0.5f * std::cos(phase * out);
        }
        else
        {
            table[i] = 1.f;
        }
    }

        static bool firstTime = true;
    if(firstTime)
    {
        firstTime = false;
        DBG("Hann envelope");
        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
    }
}

void Grain::triangularEnvelope(std::vector<float>& table)
{
    //const float halfSlots = 0.5f * (envelopeSizeSamples - 1.f);
    //const float fadeOutLen = static_cast<float>(durationSamples - fadeOut);
    //for(size_t i = 0; i < table.size(); i++)
    //{
    //    if(i < fadeIn)
    //    {
    //        table[i] = 1.f - std::abs((static_cast<float>(i - halfSlots) / halfSlots));
    //    }
    //    else if(i >= fadeOut)
    //    {
    //        const float out = i - (float)sustainWidthSamples;
    //        table[i] = 1.f - std::abs((out - halfSlots) / halfSlots);
    //    }
    //    else
    //    {
    //        table[i] = 1.f;
    //    }
    //}

    const float invSlopeUp = 1.f / std::max(1, fadeIn);
    const float invSlopeDown = 1.f / std::max(1, envelopeSizeSamples - fadeOut);

    for(size_t i = 0; i < envelopeSizeSamples; ++i)
    {
        if(i < fadeIn)
        {
            table[i] = (float)i * invSlopeUp;
        }
        else if(i >= fadeOut)
        {
            float remaining = (float)(envelopeSizeSamples - 1 - i);
            table[i] = remaining * invSlopeDown;
        }
        else
            table[i] = 1.f;
    }
    static bool firstTime = true;
    if(firstTime)
    {
        firstTime = false;
        DBG("triangular envelope");
        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
    }
}

void Grain::rectangularEnvelope(std::vector<float>& table)
{
    for(size_t i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
            table[i] = 0.f;

        if(i >= fadeOut)
            table[i] = 0.f;
        else

            table[i] = 1.f;
    }
    static bool firstTime = true;
    if(firstTime)
    {
        firstTime = false;
        DBG("rectangularEnvelope");
        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
    }
}

void Grain::hammingEnvelope(std::vector<float>& table)
{
    const float order = 2.f;
    const float phase = 2.f * pi * order / ((float)envelopeSizeSamples - 1.f);
    for(size_t i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            table[i] = static_cast<float>(0.54f - 0.46f * std::cos(phase * (float)i));
        }
        else if(i >= fadeOut)
        {
            const float out = i - (float)sustainWidthSamples;
            table[i] = static_cast<float>(0.54f - 0.46f * std::cos(phase * out));
        }
        else
        {
            table[i] = 1.f;
        }
    }
    static bool firstTime = true;
    if(firstTime)
    {
        firstTime = false;
        DBG("hammingEnvelope");
        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
    }
}

void Grain::blackmanEnvelope(std::vector<float>& table)
{
    const float alpha = 0.16f;
    const float order2 = 2.f;
    const float order4 = 4.f;
    const float phase2 = 2.f * pi * order2 / ((float)envelopeSizeSamples - 1.f);
    const float phase4 = 2.f * pi * order4 / ((float)envelopeSizeSamples - 1.f);

    for(size_t i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            table[i] = 0.5f * (1 - alpha) - 0.5 * std::cos(phase2 * i) + 0.5 * alpha * std::cos(phase4 * i);
        }
        else if(i >= fadeOut)
        {
            const float out = i - (float)sustainWidthSamples;
            table[i] = 0.5f * (1 - alpha) - 0.5 * std::cos(phase2 * out) + 0.5 * alpha * std::cos(phase4 * out);
        }
        else
        {
            table[i] = 1.f;
        }
    }
    static bool firstTime = true;
    if(firstTime)
    {
        firstTime = false;
        DBG("blackman env");
        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
    }
}

void Grain::blackmanHarrisEnvelope(std::vector<float>& table)
{
    const float order2 = 2.f;
    const float order4 = 4.f;
    const float order6 = 6.f;
    const float phase2 = 2.f * pi * order2 / ((float)envelopeSizeSamples - 1.f);
    const float phase4 = 2.f * pi * order4 / ((float)envelopeSizeSamples - 1.f);
    const float phase6 = 2.f * pi * order6 / ((float)envelopeSizeSamples - 1.f);

    for(size_t i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            table[i] =
                0.35875f - 0.48829f * std::cos(phase2 * i) + 0.14128f * std::cos(phase4 * i) - 0.01168f * std::cos(phase6 * i);
        }
        else if(i >= fadeOut)
        {
            const float out = (i - (float)sustainWidthSamples);
            table[i] = 0.35875f - 0.48829f * std::cos(phase2 * out) + 0.14128f * std::cos(phase4 * out)
                       - 0.01168f * std::cos(phase6 * out);
        }
        else
        {
            table[i] = 1.f;
        }
    }
    static bool firstTime = true;
    if(firstTime)
    {
        firstTime = false;
        DBG("blackman harris");
        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
    }
}

void Grain::flatTopEnvelope(std::vector<float>& table)
{
    const float order2 = 2.f;
    const float order4 = 4.f;
    const float order6 = 6.f;
    const float order8 = 6.f;
    const float phase2 = 2.f * pi * order2 / ((float)envelopeSizeSamples - 1.f);
    const float phase4 = 2.f * pi * order4 / ((float)envelopeSizeSamples - 1.f);
    const float phase6 = 2.f * pi * order6 / ((float)envelopeSizeSamples - 1.f);
    const float phase8 = 2.f * pi * order8 / ((float)envelopeSizeSamples - 1.f);
    for(size_t i = 0; i < table.size(); i++)
    {
        if(i < fadeIn)
        {
            table[i] = 1.f - 1.93f * std::cos(phase2 * i) + 1.29f * std::cos(phase4 * i) - 0.388f * std::cos(phase6 * i)
                       + 0.028f * std::cos(phase8 * i);
        }
        else if(i >= fadeOut)
        {
            const float out = (i - (float)sustainWidthSamples);
            table[i] = 1.f - 1.93f * std::cos(phase2 * out) + 1.29f * std::cos(phase4 * out) - 0.388f * std::cos(phase6 * out)
                       + 0.028f * std::cos(phase8 * out);
        }
        else
        {
            table[i] = 1.f;
        }
    }
    static bool firstTime = true;
    if(firstTime)
    {
        firstTime = false;
        DBG("flat top");
        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
    }
}

GrainPoint* Grain::getGrainPoint() { return &grainPoint; }

float Grain::curve(float x, float coefficient) { return 1.0f - std::pow(1.0f - x, coefficient); }

//float Grain::logInterpolation(float x)
//{
//	return std::log10(1 + x) / std::log10(2);
//}