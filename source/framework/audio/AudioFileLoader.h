#pragma once

#include <juce_audio_basics/juce_audio_basics.h> // audio loaded callback
#include <juce_audio_formats/juce_audio_formats.h> // audio format manager
#include <juce_core/juce_core.h> // file

#include "ChannelMixer.h"
#include "framework/core/Core.h"
#include "framework/core/PluginParams.h"
#include "framework/core/PluginTypes.h"

namespace juce
{
    class FileChooser; // gui basics
}

namespace particules
{
    class AudioFileLoader
    {
    public:
        AudioFileLoader();
        ~AudioFileLoader() = default;

        void loadFile(AudioLoadedCallback, const juce::File&);

        void loadFile(const str&, AudioLoadedCallback, const juce::File&);

        void processLoadingFile(juce::File&, AudioLoadedCallback);

        std::unique_ptr<AudioBuffer> loadAudioFromFile(juce::File&);

        void init(double, int) noexcept;
        void setSampleRate(double) noexcept;
        void setNumTargetChannels(int) noexcept;
        juce::AudioFormatManager& getFormatManager();

    private:
        void showErrorWindow(const str&);

        double sampleRate;

        int targetChannels;
        ChannelMixer channelMixer;
        std::unique_ptr<juce::FileChooser> chooser;
        juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3

        //JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileLoader)
    };
}
