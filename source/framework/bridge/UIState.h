#pragma once

#include <juce_audio_utils/juce_audio_utils.h> // audio thumbnail
#include <juce_audio_formats/juce_audio_formats.h> // format manager
#include <juce_events/juce_events.h> // change broadcaster
#include <juce_core/juce_core.h> // file

#include "../Core.h"

namespace particules
{
    struct VisualSnapshot;
    class GrainVisualBuffer;
    class UIState : public juce::ChangeBroadcaster
    {
    public:
        UIState();
        ~UIState() = default;

        void setSource(const juce::File& f) noexcept;
        void init(const GrainVisualBuffer* vb) noexcept;

        const VisualSnapshot& getSnapshot() const noexcept;

        bool isFileLoaded() const noexcept { return fileLoaded.load(); }
        void setNumSamples(int s) noexcept { numSamples = s; }
        int getNumSamples() const noexcept { return numSamples; }

        juce::AudioThumbnail& getAudioThumbnail() noexcept { return audioThumbnail; }

    private:
        void setFileLoaded(bool b);

        int numSamples;
        std::atomic<bool> fileLoaded{false};

        const int samplesPerThumbnail = 64;
        juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
        juce::AudioThumbnailCache cache;
        juce::AudioThumbnail audioThumbnail;

        const GrainVisualBuffer* visualBuffer = nullptr;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIState)
    };
}
