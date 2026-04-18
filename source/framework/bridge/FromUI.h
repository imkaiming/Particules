#pragma once

#include <juce_core/juce_core.h>
// expose some audio processor functions witouth coupling
// the audio processor directly with the GUI
// Populated by the plugin processor
// consumed by the gui components
// UI to DSP bus
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

        //private:
        std::function<void()> onLoadFile;
        std::function<void(const juce::String& path)> onLoadFilePath;
        std::function<float()> onIsPlaying;
        std::function<void(bool)> onSetPlaying;
    };
}