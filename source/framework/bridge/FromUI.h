#pragma once

#include <juce_core/juce_core.h>

/**
 * @class FromUI
 * @brief Unidirectional communication bridge (Message Thread -> Audio Engine).
 *
 * This class is exclusively responsible for routing commands and events triggered 
 * by the User Interface to the audio engine or the application state. It serves 
 * as the primary entry point for control instructions (e.g., loading audio files, 
 * transport commands).
 * 
 * By design, it must not expose any methods for reading the audio state, ensuring 
 * a strict separation of concerns where the UI acts as the commander and the 
 * engine acts as the executor.
 */
namespace particules
{
    class FromUI
    {
    public:
        FromUI() = default;
        ~FromUI() = default;

        void loadFile() const
        {
            if(onLoadFile)
                onLoadFile();
        }

        void loadFilePath(const juce::String& path) const
        {
            if(onLoadFilePath)
                onLoadFilePath(path);
        }

        float isPlaying() const
        {
            if(onIsPlaying)
                return onIsPlaying();
            return 0.0f;
        }

        void setPlaying(bool b) const
        {
            if(onSetPlaying)
                onSetPlaying(b);
        }

        // non copyable : contain function object
        FromUI(const FromUI&) = delete;
        FromUI& operator=(const FromUI&) = delete;
        FromUI(FromUI&&) = delete;
        FromUI& operator=(FromUI&&) = delete;

        std::function<void()> onLoadFile;
        std::function<void(const juce::String& path)> onLoadFilePath;
        std::function<float()> onIsPlaying;
        std::function<void(bool)> onSetPlaying;
    };
}