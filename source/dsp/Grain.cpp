/*
  ==============================================================================

	Grain.cpp
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#include "Grain.h"
#include "../framework/ParameterSnapshot.h"

Grain::Grain()
    : durationSamples{0}, startPositionSamples{0}, speed{1.f}, sustainWidthSamples{0}, selectionWindow{0},
      fadeInSamples{0}, fadeOutSamples{0}, elapsedSamples{0}, delaySamples{0}, readPosition{0.f}, inputNumChannels{0},
      inputNumSamples{0}, isInitialized{false} //, linearGain{1.f}
{
    reset();
}

void Grain::reset()
{
    durationSamples = 0;
    startPositionSamples = 0;
    sustainWidthSamples = 0;
    elapsedSamples = 0;
    fadeInSamples = 0;
    fadeOutSamples = 0;
    delaySamples = 0;
    generation = 0;
    inputNumSamples = 0;
    //linearGain = 1.f;
    speed = 1.f;
    readPosition = 0.f;
}

void Grain::config(const ParameterSnapshot& snapshot, int delay, float normalizedPosMod)
{
    elapsedSamples = 0;

    durationSamples = snapshot.durationSamples;
    inputNumSamples = snapshot.inputNumSamples;
    inputNumChannels = snapshot.inputNumChannels;

    // position data
    const int positionModulationSamples = static_cast<int>(normalizedPosMod * snapshot.selectionSamples);
    startPositionSamples = (snapshot.startPositionSamples + positionModulationSamples) % inputNumSamples;
    readPosition = static_cast<float>(startPositionSamples);
    selectionWindow = snapshot.selectionSamples;

    speed = snapshot.speed;
    delaySamples = delay;

    // envelope data
    sustainWidthSamples = static_cast<int>(snapshot.sustainRatio * durationSamples);
    fadeInSamples = static_cast<int>(0.5f * (durationSamples - sustainWidthSamples));
    fadeOutSamples = fadeInSamples + sustainWidthSamples;
    fadeInSamples = std::max<int>(fadeInSamples, 1);
    fadeOutSamples = std::max<int>(fadeOutSamples, 1);
    invFadeInSamples = 1.f / static_cast<float>(fadeInSamples);
    //envelopeSizeSamples = durationSamples - sustainWidthSamples;

    //DBG("########### config ###########");
    //DBG("input num samples = " + (str)inputNumSamples);
    //DBG("durationSamples = " + (str)durationSamples);
    //DBG("selectionSamples = " + (str)snapshot.selectionSamples);
    //DBG("normalizedPosMod = " + (str)normalizedPosMod);
    //DBG("positionModulationSamples = " + (str)positionModulationSamples);
    //DBG("startPositionSamples = " + (str)startPositionSamples);
    //DBG("readPosition = " + (str)readPosition);
    //DBG("delaySamples = " + (str)delaySamples);
    //DBG("sustainWidthSamples = " + (str)sustainWidthSamples);
    //DBG("envelopeSizeSamples = " + (str)envelopeSizeSamples);
    //DBG("fadeIn = " + (str)fadeIn);
    //DBG("fadeOut = " + (str)fadeOut);

    //linearGain = snapshot.linearGain;
}

const float Grain::getCurrentSample(const AudioBuffer* inputbuffer, const int outChannel, const int outNumChannels) noexcept
{
    if(delaySamples > 0)
        return 0.f;

    //if(isExhausted())
    //    return 0.f;

    // TODO mettre ca dans le VM idealement ou creer un ChannelManager
    const int intputNumsChannels = inputbuffer->getNumChannels();
    const float* sample = inputbuffer->getReadPointer(outChannel % intputNumsChannels);

    //grainPoint.setSamplePos(readPosition);
    int index = static_cast<int>(readPosition);
    float frac = readPosition - (float)index;
    const float s0 = sample[index];
    const float s1 = sample[(index + 1) % inputNumSamples];

    const float sampleValue = lerp(s0, s1, frac);

    float x = std::clamp((sampleValue * 0.5f + 0.5f), 0.f, 1.f);
    grainPoint.setOpacity(curve(x, 5.f));

    return sampleValue;
}

void Grain::update()
{
    if(delaySamples-- > 0)
        return;

    elapsedSamples++;
    readPosition += speed;
    while(readPosition >= inputNumSamples)
        readPosition -= inputNumSamples;
}

// EnvelopeLookUpTable
// fadeIn  : 0 - 0.5f
// sustain : 0.5f
// fadeOut : 0.5f - 1.f
const float Grain::getPhase() const noexcept
{
    if(durationSamples <= 0)
        return 0.f;
    //if(elapsedSamples < fadeInSamples)
    //    return 0.5f * static_cast<float>(elapsedSamples) * invFadeInSamples;
    //else if(elapsedSamples >= fadeOutSamples)
    //    return 0.5f * static_cast<float>(elapsedSamples - sustainWidthSamples) * invFadeInSamples;
    //else
    //    return 0.5f;
    const float elapsed = static_cast<float>(elapsedSamples);

    if(elapsed < fadeInSamples)
        return 0.5f * (elapsed * invFadeInSamples);
    else if(elapsed < fadeOutSamples)
        return 0.5f;
    else
    {
        const float stepsIntoRelease = elapsed - static_cast<float>(fadeOutSamples);
        const float releaseProgress = stepsIntoRelease * invFadeInSamples;
        return 0.5f + 0.5f * releaseProgress;
    }
}

GrainPoint* Grain::getGrainPoint() { return &grainPoint; }

float Grain::curve(float x, float coefficient) { return 1.0f - std::pow(1.0f - x, coefficient); }

// https://www.mathworks.com/help/signal/ug/windows.html
// https://en.wikipedia.org/wiki/Window_function
/*
void Grain::computeEnvelope(std::vector<float>& table)
{
    if(table.size() == 0)
        return;

    switch(envelopeType)
    {
        case 0:
            hannEnvelope(table); // ok
            break;
        case 1:
            triangularEnvelope(table); // ok
            break;
        case 2:
            hammingEnvelope(table); // a0 = 0.08 might be acceptable for granular
            break;
        case 3:
            blackmanEnvelope(table); // float is not enough precise a0 = -0.0000000149
            break;
        case 4:
            blackmanHarrisEnvelope(table);
            break;
        case 5:
            flatTopEnvelope(table); // can be negative a0 = -0.000421048 (use for fft)
            break;
        case 6:
            gaussianEnvelope(table); // ok
            break;
        case 7:
            exponentialEnvelope(table); // working well
            break;
        case 8:
            smoothedStepEnvelope(table); // ok
            break;
        case 9:
            tukeyEnvelope(table);
            break;
        default:
            jassertfalse;
    }
}
*/

//float Grain::ncos(const int order, const int i, const int size) const noexcept
//{
//return std::cos(2.0f * pi * (float)order * (float)i / (float)(size - 1.f));
//}

//void Grain::hannEnvelope(std::vector<float>& table)
//{
//    const float phase = envelopeSizeSamples > 1 ? 2.f * pi / ((float)envelopeSizeSamples - 1.f) : 0.f;
//    for(size_t i = 0; i < durationSamples; i++)
//    {
//        if(i < fadeIn)
//        {
//            table[i] = 0.5f - 0.5f * std::cos(phase * (float)i);
//        }
//        else if(i >= fadeOut)
//        {
//            const float out = i - (float)sustainWidthSamples;
//            table[i] = 0.5f - 0.5f * std::cos(phase * out);
//        }
//        else
//        {
//            table[i] = 1.f;
//        }
//    }
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("HANN");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

//void Grain::triangularEnvelope(std::vector<float>& table)
//{
//    const float invIn = fadeIn > 0 ? 1.f / (float)fadeIn : 0.f;
//    const float invOut = durationSamples > fadeOut ? 1.f / (float)(durationSamples - fadeOut) : 0.f;
//
//    for(size_t i = 0; i < durationSamples; ++i)
//    {
//        if(i < fadeIn)
//            table[i] = (float)i * invIn;
//        else if(i >= fadeOut)
//            table[i] = (float)(durationSamples - 1 - i) * invOut;
//        else
//            table[i] = 1.f;
//    }
//
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("TRIANGULAR");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

// w(n)=0.54−0.46cos(2π n/N), 0≤n≤N .
//void Grain::hammingEnvelope(std::vector<float>& table)
//{
//    const float phase = envelopeSizeSamples > 1 ? 2.f * pi / ((float)envelopeSizeSamples - 1.f) : 0.f;
//    for(size_t i = 0; i < durationSamples; i++)
//    {
//        if(i < fadeIn)
//        {
//            table[i] = static_cast<float>(0.54f - 0.46f * std::cos(phase * (float)i) - 0.08f);
//        }
//        else if(i >= fadeOut)
//        {
//            const float out = i - (float)sustainWidthSamples;
//            table[i] = static_cast<float>(0.54f - 0.46f * std::cos(phase * out) - 0.08f); // normaliser a 0.92 en divisant ?
//        }
//        else
//        {
//            table[i] = 1.f;
//        }
//    }
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("HAMMING");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

//0.42−0.5cos(2πn/L−1)+0.08cos(4πn/L−1)
//void Grain::blackmanEnvelope(std::vector<float>& table)
//{
//    const float phase2 = envelopeSizeSamples > 1 ? twoPi / ((float)envelopeSizeSamples - 1.f) : 0.f;
//    const float phase4 = envelopeSizeSamples > 1 ? 2.f * twoPi / ((float)envelopeSizeSamples - 1.f) : 0.f;
//
//    for(size_t i = 0; i < durationSamples; i++)
//    {
//        if(i < fadeIn)
//        {
//            const float val = 0.42f - 0.5f * std::cos(phase2 * i) + 0.08f * std::cos(phase4 * i);
//            table[i] = std::clamp(val, 0.f, 1.f);
//        }
//        else if(i >= fadeOut)
//        {
//            const float out = i - (float)sustainWidthSamples;
//            const float val = 0.42f - 0.5f * std::cos(phase2 * out) + 0.08f * std::cos(phase4 * out);
//            table[i] = std::clamp(val, 0.f, 1.f);
//        }
//        else
//            table[i] = 1.f;
//    }
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("BLACKMAN");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

//void Grain::blackmanHarrisEnvelope(std::vector<float>& table)
//{
//    const float phase = envelopeSizeSamples > 1 ? twoPi / ((float)envelopeSizeSamples - 1.f) : 0.f;
//
//    for(size_t i = 0; i < durationSamples; i++)
//    {
//        if(i < fadeIn)
//        {
//            const float val = 0.35875f - 0.48829f * std::cos(phase * i) + 0.14128f * std::cos(2.f * phase * i)
//                              - 0.01168f * std::cos(3.f * phase * i);
//            table[i] = std::clamp(val, 0.f, 1.f);
//        }
//        else if(i >= fadeOut)
//        {
//            const float out = (i - (float)sustainWidthSamples);
//            const float val = 0.35875f - 0.48829f * std::cos(phase * out) + 0.14128f * std::cos(2.f * phase * out)
//                              - 0.01168f * std::cos(3.f * phase * out);
//            table[i] = std::clamp(val, 0.f, 1.f);
//        }
//        else
//        {
//            table[i] = 1.f;
//        }
//    }
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("BLACKMAN HARRIS");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

// a0 −a 1 cos(2πn L−1) + a 2 cos(4πn L−1)−a 3 cos(6πn L−1) + a 4 cos(8πn L−1)
// a0=0.21557895 a1=0.41663158 a20.277263158 a3=0.083578947 a4=0.006947368
//void Grain::flatTopEnvelope(std::vector<float>& table)
//{
//    const float a0 = 0.21557895f;
//    const float a1 = 0.41663158f;
//    const float a2 = 0.277263158f;
//    const float a3 = 0.083578947f;
//    const float a4 = 0.006947368f;
//    const float phase = envelopeSizeSamples > 1 ? pi / ((float)envelopeSizeSamples) : 0.f;
//
//    for(size_t i = 0; i < durationSamples; i++)
//    {
//        if(i < fadeIn)
//        {
//            const float val = a0 - a1 * std::cos(2.f * phase * i) + a2 * std::cos(4.f * phase * i)
//                              - a3 * std::cos(6.f * phase * i) + a4 * std::cos(8.f * phase * i);
//            table[i] = std::clamp(val, 0.f, 1.f);
//        }
//        else if(i >= fadeOut)
//        {
//            const float out = (i - (float)sustainWidthSamples);
//            const float val = a0 - a1 * std::cos(2.f * phase * out) + a2 * std::cos(4.f * phase * out)
//                              - a3 * std::cos(6.f * phase * out) + a4 * std::cos(8.f * phase * out);
//            table[i] = std::clamp(val, 0.f, 1.f);
//        }
//        else
//            table[i] = 1.f;
//    }
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("FLAT TOP");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

//void Grain::gaussianEnvelope(std::vector<float>& table)
//{
//    const float sigma = 0.4f;
//    const float invFadeIn = fadeIn > 0 ? 1.f / fadeIn : 0.f;
//    const float invFadeOut = (durationSamples - fadeOut) > 0 ? 1.f / (durationSamples - fadeOut) : 0.f;
//    const float offset = -0.0183279f;
//
//    for(int i = 0; i < durationSamples; ++i)
//    {
//        if(i < fadeIn)
//        {
//            const float x = 1.f - (i * invFadeIn);
//            table[i] = std::exp(-4.f * x * x) - offset;
//        }
//        else if(i >= fadeOut)
//        {
//            const float p = (i - fadeOut) * invFadeOut;
//            table[i] = std::exp(-4.f * p * p) - offset;
//        }
//        else
//            table[i] = 1.f - offset;
//    }
//
//    table[0] = 0.f;
//    table[durationSamples - 1] = 0.f;
//
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("GAUSSIAN");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

//void Grain::exponentialEnvelope(std::vector<float>& table)
//{
//    const float kIn = fadeIn > 0 ? 5.f / fadeIn : 0.f;
//    const float kOut = durationSamples > fadeOut ? 5.f / (durationSamples - fadeOut) : 0.f;
//
//    for(int i = 0; i < durationSamples; ++i)
//    {
//        if(i < fadeIn)
//            table[i] = 1.f - std::exp(-kIn * i);
//        else if(i >= fadeOut)
//        {
//            const float d = durationSamples - 1.f - (float)i;
//            table[i] = 1.f - std::exp(-kOut * d);
//        }
//        else
//
//            table[i] = 1.f;
//    }
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("EXP");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

//void Grain::smoothedStepEnvelope(std::vector<float>& table)
//{
//    const float invIn = fadeIn > 0 ? 1.f / (float)fadeIn : 0.f;
//    const float invOut = durationSamples > fadeOut ? 1.f / (float)(durationSamples - fadeOut) : 0.f;
//
//    for(int i = 0; i < durationSamples; ++i)
//    {
//        if(i < fadeIn)
//            table[i] = smoothedStep((float)i * invIn);
//        else if(i >= fadeOut)
//            table[i] = smoothedStep((float)(durationSamples - 1 - i) * invOut);
//        else
//            table[i] = 1.f;
//    }
//
//    static bool firstTime = true;
//    if(firstTime)
//    {
//        firstTime = false;
//        DBG("SMOOTHED STEP");
//        DBG("Envelope first 5: " << table[0] << ", " << table[1] << ", " << table[2] << ", " << table[3] << ", " << table[4]);
//        DBG("Envelope last 5: " << table[table.size() - 5] << ", " << table[table.size() - 4] << ", " << table[table.size() - 3]
//                                << ", " << table[table.size() - 2] << ", " << table[table.size() - 1]);
//    }
//}

//float Grain::logInterpolation(float x)
//{
//	return std::log10(1 + x) / std::log10(2);
//}