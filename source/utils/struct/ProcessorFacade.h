#pragma once

#include "../../framework/Core.h"

// expose some audio processor functions witouth coupling
// the audio processor directly with the GUI

namespace particules
{
    struct ProcessorFacade
    {
        std::function<void()> loadFile;
        std::function<void(const str& path)> loadFilePath;
    };
}