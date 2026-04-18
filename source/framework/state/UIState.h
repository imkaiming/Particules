#pragma once

#include <juce_audio_formats/juce_audio_formats.h> // format manager
#include <juce_audio_utils/juce_audio_utils.h> // audio thumbnail
#include <juce_core/juce_core.h> // file
#include <juce_events/juce_events.h> // change broadcaster

#include "framework/core/Core.h"

// pushed
namespace particules
{
    struct AudioPayload;
    struct VisualSnapshot;
    template <typename T>
    class PingPongBuffer;
    class UIState : public juce::ChangeBroadcaster
    {
    public:
        UIState();
        ~UIState() = default;

        void setSource(const juce::File& f) noexcept;
        void init(const PingPongBuffer<VisualSnapshot>* vb) noexcept;

        const VisualSnapshot& getSnapshot() const noexcept;

        bool isFileLoaded() const noexcept { return fileLoaded.load(); }

        juce::AudioThumbnail& getAudioThumbnail() noexcept { return audioThumbnail; }
        const juce::File& getCurrentFile() const noexcept { return currentFile; }
        void setPayload(AudioPayload*);

    private:
        void setFileLoaded(bool b);

        const int samplesPerThumbnail = 64;
        juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
        juce::AudioThumbnailCache cache;
        juce::AudioThumbnail audioThumbnail;

        const PingPongBuffer<VisualSnapshot>* visualBuffer;
        std::atomic<AudioPayload*> currentPayload;
        std::atomic<bool> fileLoaded;
        juce::File currentFile;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIState)
    };
}
