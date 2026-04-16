#include "AudioState.h"

namespace particules
{
    AudioState::AudioState()
        : isGrainsEmpty{true}, sampleRate{0.0}, numChannels{0}, numSamples{0}, numActiveGrains{0}, isLinked{false}, isAuditioning{false}
    {
    }

    AudioStateSnapshot AudioState::getSnapshot() const noexcept
    {
        AudioStateSnapshot snapshot;

        snapshot.sampleRate = getSampleRate();
        snapshot.inputNumSamples = getNumSamples();
        snapshot.inputNumChannels = getNumChannels();

        return snapshot;
    }
}