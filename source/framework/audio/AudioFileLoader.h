#pragma once

#include "../Core.h"
#include "../PluginTypes.h"
#include "framework/bridge/LockFreePointerQueue.h"

#include <juce_audio_basics/juce_audio_basics.h> // audio loaded callback
#include <juce_audio_formats/juce_audio_formats.h> // audio format manager
#include <juce_core/juce_core.h> // file

#include "../PluginParams.h"
#include "ChannelMixer.h"

namespace juce
{
    class FileChooser; // gui basics
}

namespace particules
{
    //template <typename T>
    //class LockFreePointerQueue;
    class AudioFileLoader
    {
    public:
        AudioFileLoader(LockFreePointerQueue<AudioBuffer>&);
        ~AudioFileLoader() = default;

        void loadFile(AudioLoadedCallback, const juce::File&);

        void loadFile(const str&, AudioLoadedCallback, const juce::File&);

        void processLoadingFile(juce::File&, AudioLoadedCallback);

        bool loadAudioFromFile(juce::File&, AudioBuffer&);

        void init(double, int) noexcept;
        void setSampleRate(double) noexcept;
        void setNumTargetChannels(int) noexcept;
        juce::AudioFormatManager& getFormatManager();

    private:
        static constexpr int MAX_FILE_SIZE = params::maxFileSize;
        static constexpr double MAX_DURATION = params::maxFileDuration;
        void showErrorWindow(const str&);

        double sampleRate;

        ChannelMixer channelMixer;
        LockFreePointerQueue<AudioBuffer>& incomingBuffer;
        std::unique_ptr<juce::FileChooser> chooser;
        juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileLoader)
    };
}
//void setOnFileLoadedCallBack(std::function<void(bool)> foo);
//std::function<void(bool)> onFileLoaded;