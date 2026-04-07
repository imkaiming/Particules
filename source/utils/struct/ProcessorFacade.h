#pragma once

//#include "../../framework/GuiTypes.h"

#include <juce_core/juce_core.h>
// expose some audio processor functions witouth coupling
// the audio processor directly with the GUI

namespace particules
{
    struct ProcessorFacade
    {
        std::function<void()> loadFile;
        std::function<void(const juce::String& path)> loadFilePath;
        std::function<void()> play;
        std::function<void()> stop;
    };
}