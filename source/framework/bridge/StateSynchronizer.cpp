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
        : currentPayload{payload}, garbageCollector{gc}, audioState{as}, uiState{us}, lastSeenPayload{nullptr}
    {
    }

    StateSynchronizer::~StateSynchronizer() { stop(); }

    void StateSynchronizer::start(int hz) { startTimerHz(hz); }

    void StateSynchronizer::stop() { stopTimer(); }

    void StateSynchronizer::timerCallback()
    {
#ifdef TRACY_ENABLE 
        ZoneScopedN("Zombie Garbage Collection");
#endif

        // 1. emptying the garabage collector
        while(AudioPayload* oldPayload = garbageCollector.pop())
            zombies.push_back(oldPayload);
        //delete oldPayload;

        // 2. erasing only grains that are not active
        zombies.erase(std::remove_if(zombies.begin(), zombies.end(),
                          [](AudioPayload* p) {
                              if(p->activeReaders.load(std::memory_order_acquire) == 0)
                              {
                                  delete p;
                                  return true;
                              }
                              return false; // some grains are still actives. The deletion is delayed to the next tick.
                          }),
            zombies.end());

        // 3. state syncing
        AudioPayload* playingNow = currentPayload.load(std::memory_order_acquire);
        if(playingNow != nullptr && playingNow != lastSeenPayload)
        {
            audioState.setNumSamples(playingNow->numSamples);
            audioState.setNumChannels(playingNow->numChannels);
            uiState.setSource(playingNow->file);
            lastSeenPayload = playingNow;
        }
    }
}