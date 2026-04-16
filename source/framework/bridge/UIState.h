#pragma once

#include <juce_audio_formats/juce_audio_formats.h> // format manager
#include <juce_audio_utils/juce_audio_utils.h> // audio thumbnail
#include <juce_core/juce_core.h> // file
#include <juce_events/juce_events.h> // change broadcaster

#include "../Core.h"

// pushed 
namespace particules
{
    struct VisualSnapshot;
    template<typename T>
    class LockFreeDoubleBuffer;
    class UIState : public juce::ChangeBroadcaster
    {
    public:
        UIState();
        ~UIState() = default;

        void setSource(const juce::File& f) noexcept;
        void init(const LockFreeDoubleBuffer<VisualSnapshot>* vb) noexcept;

        const VisualSnapshot& getSnapshot() const noexcept;

        bool isFileLoaded() const noexcept { return fileLoaded.load(); }
        //void setNumSamples(int s) noexcept { numSamples = s; }
        //int getNumSamples() const noexcept { return numSamples; }

        juce::AudioThumbnail& getAudioThumbnail() noexcept { return audioThumbnail; }
        const juce::File& getCurrentFile() const noexcept { return currentFile; }

    private:
        void setFileLoaded(bool b);

        //int numSamples;
        std::atomic<bool> fileLoaded;
        juce::File currentFile;

        const int samplesPerThumbnail = 64;
        juce::AudioFormatManager formatManager; // classe qui traite les formats de fichier tq wav, aiff, ogg, vorbis ou mp3
        juce::AudioThumbnailCache cache;
        juce::AudioThumbnail audioThumbnail;

        const LockFreeDoubleBuffer<VisualSnapshot>* visualBuffer;
        //const GrainVisualBuffer& visualBuffer;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UIState)
    };
}
