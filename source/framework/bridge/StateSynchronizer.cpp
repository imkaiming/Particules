#include "StateSynchronizer.h"

#include "framework/state/AudioState.h"
#include "framework/state/UIState.h"
#include "utils/struct/AudioPayload.h"

namespace particules
{
    StateSynchronizer::StateSynchronizer(
        std::atomic<AudioPayload*>& payload, RingBuffer<AudioPayload>& gc, AudioState& as, UIState& us)
        : currentPayload{payload}, garbageCollector{gc}, audioState{as}, uiState{us}
    {
    }

    StateSynchronizer::~StateSynchronizer() { stop(); }

    void StateSynchronizer::start(int hz) { startTimerHz(hz); }

    void StateSynchronizer::stop() { stopTimer(); }

    void StateSynchronizer::timerCallback()
    {
        // 1. freeing ram in the ui thread
        while(AudioPayload* oldPayload = garbageCollector.pop())
            delete oldPayload;

        // 2. state syncing
        AudioPayload* playingNow = currentPayload.load(std::memory_order_acquire);

        if(playingNow != nullptr && playingNow != lastSeenPayload)
        {
            //3.  new source of truth need to be updated to the states
            audioState.setNumSamples(playingNow->numSamples);
            audioState.setNumChannels(playingNow->numChannels);
            uiState.setSource(playingNow->file);
            lastSeenPayload = playingNow;
        }
    }
}