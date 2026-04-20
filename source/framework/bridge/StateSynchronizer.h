#pragma once

#include <atomic>
#include <vector>

#include <juce_events/juce_events.h>

#include "framework/bridge/RingBuffer.h"


// state synchronizer is responsible of emptying the garabage collector
// and to update the state with the new payloads
namespace particules
{
    class AudioState;
    struct AudioPayload;
    class UIState;
    class StateSynchronizer : public juce::Timer
    {
    public:
        StateSynchronizer(std::atomic<AudioPayload*>& payload, RingBuffer<AudioPayload*>& gc, AudioState& as, UIState& us);
        ~StateSynchronizer() override;

        void start(int framerateHz = 30);
        void stop();

        void timerCallback() override;

    private:
        std::atomic<AudioPayload*>& currentPayload;
        RingBuffer<AudioPayload*>& garbageCollector;

        AudioState& audioState;
        UIState& uiState;

        AudioPayload* lastSeenPayload;

        std::vector<AudioPayload*> zombies;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateSynchronizer)
    };
}