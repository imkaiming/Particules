#pragma once

#include <juce_core/juce_core.h>
// expose some audio processor functions witouth coupling
// the audio processor directly with the GUI

namespace particules
{
    struct ProcessorFacade
    {
        std::function<void()> loadFile;
        std::function<void(const juce::String& path)> loadFilePath;
        std::function<float()> isPlaying;
        std::function<void(bool)> setPlaying;
    };
}