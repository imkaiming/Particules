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

// Grain is the unit that read samples the buffer
// TODO rename it GrainPOD

enum WindowingMethod {
    triangular,
    hann,
    hamming,
    rectangular,
    blackman,
    blackmanHarris,
    flatTop
};

struct ParameterSnapshot;
struct SampleSource;
class Grain
{
public:
    //Grain(int duration, int numChannel, int envelopeType, float speed, int envelopeWidth, int position);
    Grain();
    ~Grain() = default;

    void reset();
    void config(const ParameterSnapshot& snapshot, int sample);

    //float getCurrentSample(const int channel);
    float getCurrentSample(const SampleSource* source, const int channel, const int outChannel) noexcept;
    bool isExhausted() { return currentTime >= duration; };
    void update() { currentTime++; };

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

    int currentTime; // le compteur interne du grain
    float speed = 0.0f;
    //int numChannels = 0;	// le grain est le même pour chaque channel
    int duration = 0; // définie la durée en nombre de sample
    int position = 0; // définie la position en sample dans le buffer
    // const int selection;	// définie la position maximale qu'un grain peut atteindre dans le buffer
    int envelopeWidth = 0; // définie la taille des rampes d'amplitude en entré et en sortie du grain
    int offset = 0;

    int fadeIn = 0; // 0 to fadeIn
    int fadeOut = 0; // fadeOut to numSamples
    int envelopeSize = 0; // utile pour calculer les fade d'entrés et de sorties des envelopes selon les functions données

    //juce::AudioBuffer<float>* buffer; // replace with sample source

    GrainPoint grainPoint;

    // lifecycle
    uint16_t generation = 0;
    bool active = false;
};
