#include <array>

#include "GranularVoice.h"

#include "framework/core/PluginParams.h"

// act like a midi dispatch. 
// assign a midi note to a granular voice

namespace particules
{
    struct ParameterSnapshot;
    struct SmoothedParameters;
    struct AudioPayload;
    class VoiceManager
    {
    public:
        explicit VoiceManager(SpawnGrainCallback);
        ~VoiceManager() = default;

        void prepare(double sampleRate);
        void process(int currentSample, const ParameterSnapshot& ps, const SmoothedParameters& sp, AudioPayload* payload);

        void noteOn(int midiNoteNumber, float velocity);
        void noteOff(int midiNoteNumber);
        void allNotesOff();

        bool isAnyVoicesActive() const noexcept;
        int getNumActiveNotes() const noexcept;
        float getVoiceGain(int index) const noexcept;
    private:
        GranularVoice* findOldestVoice();
        GranularVoice* findFreeVoice();
        GranularVoice* findVoiceByNote(int noteNumber);

        SpawnGrainCallback spawnCallback;
        std::array<GranularVoice, params::maxMidiVoice> voices;

        uint32_t globalSampleCounter = 0;

        static constexpr int numPitchRatio = 128;
        std::array<float, numPitchRatio> pitchRatioLUT;
        float baseNote = 60.f; // C3
    };
}