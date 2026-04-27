#include "StateSynchronizer.h"

#ifdef TRACY_ENABLE
#include <tracy/Tracy.hpp>
#endif

#include "framework/state/AudioState.h"
#include "framework/state/UIState.h"
#include "utils/struct/AudioPayload.h"

namespace particules
{
    StateSynchronizer::StateSynchronizer(
        std::atomic<AudioPayload*>& payload, RingBuffer<AudioPayload*>& gc, AudioState& as, UIState& us)
        : currentPayload{payload}, releaseQueue{gc}, audioState{as}, uiState{us}, lastSeenPayload{nullptr}
    {
    }

    StateSynchronizer::~StateSynchronizer() { stop(); }

    void StateSynchronizer::start(int hz) { startTimerHz(hz); }

    void StateSynchronizer::stop() { stopTimer(); }

    void StateSynchronizer::timerCallback()
    {
#ifdef TRACY_ENABLE
        ZoneScopedN("Garbage Queue");
#endif

        // 1. emptying the garabage collector
        while(AudioPayload* oldPayload = releaseQueue.pop())
            pendingDeletions.push_back(oldPayload);
        //delete oldPayload;

        // 2. verifying no grains are currently reading the payload before deleting it
        pendingDeletions.erase(std::remove_if(pendingDeletions.begin(), pendingDeletions.end(),
                                   [](AudioPayload* p) {
                                       if(p->activeReaders.load(std::memory_order_acquire) == 0)
                                       {
                                           delete p;
                                           return true;
                                       }
                                       return false; // some grains are still actives. The deletion is delayed to the next tick.
                                   }),
            pendingDeletions.end());

        // 3. state syncing
        AudioPayload* playingNow = currentPayload.load(std::memory_order_acquire);
        if(playingNow != nullptr && playingNow != lastSeenPayload)
        {
            audioState.setNumSamples(playingNow->numSamples);
            audioState.setNumChannels(playingNow->numChannels);
            uiState.setSource(playingNow->file); // retrigger the new audio thumbnail repaint
            lastSeenPayload = playingNow;
        }
    }
}