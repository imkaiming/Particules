/*
  ==============================================================================

	Grain.h
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include "../framework/Core.h"

// Grain is the unit that read samples the buffer
// It have 2 main structure time and amplitude
// a grain have a fade in then a sustain then a fade out
struct ParameterSnapshot;
struct SmoothedParameters;
class Grain
{
public:
    Grain();
    ~Grain() = default;

    void reset();
    void config(const ParameterSnapshot& snapshot, float normalizedPosMod);

    //const float getCurrentSample(const AudioBuffer*, const int) noexcept;

    //const int getReadPosition() const noexcept { return static_cast<int>(readPosition); } 
    const float getReadPosition() const noexcept; 
    void nextReadPosition() noexcept;
    void updateParams(const SmoothedParameters&) noexcept;

    bool getActive() const noexcept { return active; }
    void setActive(bool b) noexcept { active = b; }
    bool isExhausted() const noexcept { return elapsedSamples >= durationSamples; }

    uint16_t getGeneration() const noexcept { return generation; }
    void incrementGen() noexcept { generation++; }

    const float getPhase() const noexcept;

private:
    //void setEnvelopeData(const float sustainRatio) noexcept;

    // Time related parameters
    int elapsedSamples; // le compteur interne du grain
    int durationSamples; // définie la durée en nombre de sample
    int startPositionSamples; // définie la position en sample dans le buffer
    int selectionWindow; // définie la position maximale qu'un grain peut atteindre dans le buffer
    //int delaySamples; // act like a little delay to sync scheduler.process() and voiceManager.process()
    int sustainWidthSamples; // définie la taille des rampes d'amplitude en entré et en sortie du grain
    int fadeInSamples; // 0 to fadeIn
    int fadeOutSamples; // fadeOut to durationSamples
    int inputNumSamples;
    int inputNumChannels;

    float speed; // change the pitch and accelerate the lecture
    float readPosition;
    float invFadeInSamples;
    //float linearGain;

    // lifecycle
    uint16_t generation = 0;
    bool active = false;
    //bool isInitialized = false;
};
