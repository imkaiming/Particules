#pragma once
enum class EnvelopeMode;
enum class TraversalMode;
struct ParameterSnapshot
{
    int durationSamples;
    int startPositionSamples;
    int selectionSamples;
    TraversalMode traversalMode;
    EnvelopeMode envMode;

    int inputNumSamples;
    int inputNumChannels;

    float emission;
    float linearGain;
    float speed;
    float mix;
    float sustainRatio;
    float traversalFreq;

    double sampleRate;
    bool isValid() const { return durationSamples > 0 && speed > 0 && emission >= 0; }
};