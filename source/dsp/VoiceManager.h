#include <array>

#include "GranularVoice.h"

#include "framework/core/PluginParams.h"

// act like a midi dispatch.
// assign a midi note to a granular voice

namespace particules
{
    struct ParameterSnapshot;
    //struct SmoothedParameters;
    struct AudioPayload;
    class VoiceManager
    {
    public:
        explicit VoiceManager(SpawnGrainCallback);
        ~VoiceManager() = default;

        void setSampleRate(double sampleRate);
        void process(int currentSample, const ParameterSnapshot& ps, AudioPayload* payload /*, const SmoothedParameters& sp*/);

        void noteOn(int midiNoteNumber, float velocity);
        void noteOff(int midiNoteNumber);
        void allNotesOff();

        bool isAnyVoicesActive() const noexcept;
        int getNumActiveNotes() const noexcept;
        bool isVoiceDead(int index) const noexcept;
        float getVoiceGain(int index) const noexcept;
        void setParameters(float a, float d, float s, float r, float e) noexcept;

    private:
        static constexpr int numPitchRatio = 128;

        GranularVoice* findOldestVoice();
        GranularVoice* findFreeVoice();
        GranularVoice* findVoiceByNote(int noteNumber);

        SpawnGrainCallback spawnCallback;
        
        std::array<GranularVoice, params::maxMidiVoice> voices;

        uint32_t globalSampleCounter;
        std::array<float, numPitchRatio> pitchRatioLUT;
        float baseNote; // C3
    };
}