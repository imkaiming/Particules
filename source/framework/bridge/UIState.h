#pragma once

//#include "../../utils/struct/VisualSnapshot.h"
#include "../Core.h"
#include "../bridge/GrainVisualBuffer.h"

namespace particules
{
    class UIState : public juce::ChangeBroadcaster
    {
    public:
        UIState();
        ~UIState() = default;

        void setSource(const juce::File& f) noexcept;
        void init(const GrainVisualBuffer* vb) { visualBuffer = vb; }

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
