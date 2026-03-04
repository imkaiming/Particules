/*
  ==============================================================================

	Grain.h
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include "../framework/Core.h"
#include "../utils/GrainPoint.h"

enum WindowingMethod { triangular, hann, hamming, rectangular, blackman, blackmanHarris, flatTop };

// Grain is the unit that read samples the buffer
// It have 2 main structure time and amplitude
// a grain have a fade in then a sustain then a fade out
struct ParameterSnapshot;
struct SampleSource;
class Grain
{
public:
    Grain();
    ~Grain() = default;

    void reset();
    void config(const ParameterSnapshot&, int, float);

    // Runtime functions
    float getCurrentSample(const AudioBuffer*, const int, const int) noexcept;
    bool isExhausted() { return elapsedSamples >= durationSamples; };
    void update() { elapsedSamples++; };

    GrainPoint* getGrainPoint();

    bool getActive() const noexcept { return active; };
    void setActive(bool b) noexcept { active = b; };

    uint16_t getGeneration() const noexcept { return generation; };
    void incrementGen() noexcept { generation++; };

private:
    float curve(float, float);

    // envelope type function
    float ncos(size_t, size_t, size_t);
    float hammingEnvelope(const int index);
    float triangularEnvelope(const int index);
    float hannEnvelope(int index);
    float rectangularEnvelope(const int index);
    float blackmanEnvelope(const int index);
    float blackmanHarrisEnvelope(const int index);
    float flatTopEnvelope(const int index);
    float applyEnvelope(const int index);

    WindowingMethod getWindowingMethod(int);

    int envelopeType; // on associe un grain a une envelope

    // Time related parameters
    float elapsedSamples; // le compteur interne du grain
    float speed = 0.0f; // change the pitch and accelerate the lecture
    float durationSamples = 0; // définie la durée en nombre de sample
    float startPosition = 0; // définie la position en sample dans le buffer
    float selectionWindow; // définie la position maximale qu'un grain peut atteindre dans le buffer
    float sustainWidth = 0; // définie la taille des rampes d'amplitude en entré et en sortie du grain
    float offset = 0; // act like a little delay to sync scheduler.process() and voiceManager.process()
    float fadeIn = 0; // 0 to fadeIn
    float fadeOut = 0; // fadeOut to numSamples
    float envelopeSize = 0; // utile pour calculer les fade d'entrés et de sorties des envelopes selon les functions données
    float linearGain = 1.f;

    //int traversalMode; // change position with phase mode
    //int traversalTime; // speed of the traversal LFO

    // Amplitude related data
    std::vector<float> envelopeTable; // precompute the entire grain envelope once

    GrainPoint grainPoint;

    // lifecycle
    uint16_t generation = 0;
    bool active = false;
};
