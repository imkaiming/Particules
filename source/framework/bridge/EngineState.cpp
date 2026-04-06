#include "EngineState.h"

namespace particules
{
    EngineState::EngineState()
        : isGrainsEmpty{true}, sampleRate{0.0}, numChannels{0}, numSamples{0}, numActiveGrains{0}, isLinked{false}
    {
    }

    const EngineSnapshot EngineState::getSnapshot() const noexcept
    {
        EngineSnapshot snapshot;

        snapshot.sampleRate = getSampleRate();
        snapshot.inputNumSamples = getNumSamples();
        snapshot.inputNumChannels = getNumChannels();

        return snapshot;
    }
}