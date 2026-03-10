/*
  ==============================================================================

	Grain.h
	Created: 15 Feb 2023 1:44:13pm
	Author:  user

  ==============================================================================
*/

#pragma once

#include "../framework/Core.h"
#include "../framework/EnvelopeMode.h"
#include "../utils/GrainPoint.h"

//enum WindowingMethod { triangular, hann, hamming, rectangular, blackman, blackmanHarris, flatTop };

// Grain is the unit that read samples the buffer
// It have 2 main structure time and amplitude
// a grain have a fade in then a sustain then a fade out
struct ParameterSnapshot;
class Grain
{
public:
    explicit Grain();
    ~Grain() = default;

    void reset();
    void config(const ParameterSnapshot&, int, float);

    // Runtime functions
    const float getCurrentSample(const AudioBuffer*, const int, const int) noexcept;
    bool isExhausted() const noexcept { return elapsedSamples >= durationSamples; };
    void update();

    GrainPoint* getGrainPoint();

    bool getActive() const noexcept { return active; };
    void setActive(bool b) noexcept { active = b; };

    uint16_t getGeneration() const noexcept { return generation; };
    void incrementGen() noexcept { generation++; };

    const float getPhase() const noexcept;
    //const EnvelopeMode getEnvelopeMode() const noexcept { return envMode; };

private:
    float curve(float, float);

    //EnvelopeMode envMode;
    //const float readEnvelope() const noexcept;
    // envelope type function
    //void hannEnvelope(std::vector<float>& table);
    //void hammingEnvelope(std::vector<float>& table);
    //void triangularEnvelope(std::vector<float>& table);
    //void blackmanEnvelope(std::vector<float>& table);
    //void blackmanHarrisEnvelope(std::vector<float>& table);
    //void flatTopEnvelope(std::vector<float>& table);
    //void gaussianEnvelope(std::vector<float>& table);
    //void exponentialEnvelope(std::vector<float>& table);
    //void smoothedStepEnvelope(std::vector<float>& table);
    //void tukeyEnvelope(std::vector<float>& table);

    //void applyEnvelope(std::vector<float>& table);
    //const float smoothedStep(float x) const noexcept { return x * x * (3.f - 2.f * x); };
    //void computeEnvelope(std::vector<float>&);

    //int envelopeType; // on associe un grain a une envelope

    // Time related parameters
    int elapsedSamples; // le compteur interne du grain
    int durationSamples; // définie la durée en nombre de sample
    int startPositionSamples; // définie la position en sample dans le buffer
    int selectionWindow; // définie la position maximale qu'un grain peut atteindre dans le buffer
    int delaySamples; // act like a little delay to sync scheduler.process() and voiceManager.process()
    int sustainWidthSamples; // définie la taille des rampes d'amplitude en entré et en sortie du grain
    //int envelopeSizeSamples; // utile pour calculer les fade d'entrés et de sorties des envelopes selon les functions données
    int fadeInSamples; // 0 to fadeIn
    int fadeOutSamples; // fadeOut to durationSamples
    int inputNumSamples;
    int inputNumChannels;

    float speed; // change the pitch and accelerate the lecture
    float readPosition;
    float invFadeInSamples;
    //float linearGain;

    //int traversalMode; // change position with phase mode
    //int traversalTime; // speed of the traversal LFO

    // Amplitude related data
    //std::vector<float> envelopeTable; // precompute the entire grain envelope once

    GrainPoint grainPoint;

    // lifecycle
    uint16_t generation = 0;
    bool active = false;
    bool isInitialized = false;
};
