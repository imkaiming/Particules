#pragma once

#include <juce_core/juce_core.h>

#include "framework/core/Core.h"
#include "framework/core/PluginParams.h"

// Scheduler is responsible of computing the events
// Maintains state necessary for activating grains according to Grain onset timesand durations
// Exposes a method for synthesizing samples of sound by mixing together the output from its active Grains.
// Manages grain allocation, for efficiency it may maintain a pool of reusable Grains
// Scheduler compute onsets
namespace particules
{
    struct ParameterSnapshot;
    class Scheduler
    {
    public:
        Scheduler();
        ~Scheduler() = default;

        void init(double) noexcept;
        void setEmission(float) noexcept;
        void tick(std::function<void(const ParameterSnapshot&)>, const ParameterSnapshot&);

        void reset();

    private:
        static constexpr int SIZE = params::maxSpawnsPerBlock;

        double nextOnSet; // Tells us when the next grain should play
        double interOnSet; // time period between every spawn
        double phase;

        double sampleRate;
        float emission;

        //juce::Random random; // parameters to set the interOnset
    };
}