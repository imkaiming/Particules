#pragma once

#include "../Core.h"
#include "../PluginTypes.h"

#include <juce_audio_basics/juce_audio_basics.h> // audio loaded callback
#include <juce_audio_formats/juce_audio_formats.h> // audio format manager
#include <juce_core/juce_core.h> // file

#include "../Constants.h"
#include "ChannelMixer.h"

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

        void loadFile(AudioLoadedCallback);

        void loadFile(const str&, AudioLoadedCallback);

        void processLoadingFile(juce::File&, AudioLoadedCallback);

        bool loadAudioFromFile(juce::File&, AudioBuffer&);

        void init(double, int) noexcept;
        void setSampleRate(double) noexcept;
        juce::AudioFormatManager& getFormatManager();
        const juce::File& getCurrentFile() const noexcept { return currentFile; };
        void setCurrentFile(juce::File& f) noexcept { currentFile = f; };

    private:
        static constexpr int MAX_FILE_SIZE = maxFileSize;
        static constexpr double MAX_DURATION = maxFileDuration;
        void showErrorWindow(const juce::String&);

        double sampleRate;

        ChannelMixer channelMixer;
        juce::File currentFile;
        std::unique_ptr<juce::FileChooser> chooser;
        juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileLoader)
    };
}
//void setOnFileLoadedCallBack(std::function<void(bool)> foo);
//std::function<void(bool)> onFileLoaded;