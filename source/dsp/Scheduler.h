/*
  ==============================================================================

	Scheduler.h
	Created: 15 Feb 2023 1:44:18pm
	Author:  user

  ==============================================================================
*/

// Maintains state necessary for activating grains according to Grain onset timesand durations
// Exposes a method for synthesizing samples of sound by mixing together the output from its active Grains.
// Manages grain allocation, for efficiency it may maintain a pool of reusable Grains

// Scheduler is responsible of computing the events

#pragma once

#include "../framework/Constants.h"
#include "../framework/Core.h"

#include <juce_core/juce_core.h>

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
        //void tick(int, std::function<void(int, const ParameterSnapshot&)>, const ParameterSnapshot&);

    private:
        static constexpr int SIZE = maxSpawnsPerBlock;

        void reset();
        //const double getInterOnSet(float, double) const noexcept;
        //double getNextOnSet() const noexcept { return nextOnSet; }
        //void setNextOnSet(double n) noexcept { nextOnSet = n; }

        double nextOnSet; // Tells us when the next grain should play
        double interOnSet; // time period between every spawn
        double phase;

        double sampleRate;
        float emission;

        juce::Random random; // parameters to set the interOnset

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Scheduler)
    };
}