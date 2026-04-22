#pragma once
#include "framework/core/Core.h"
#include "utils/enum/EnvelopeMode.h"
#include "utils/enum/TraversalMode.h"

// init parameters for the plugin

namespace particules
{
    namespace gui
    {
        inline constexpr const float grainVisualSize = 8.f;
        inline constexpr const float refreshRate = 30.f;

        inline constexpr const int windowWidthInit = 900; 
        inline constexpr const int windowWidthMin = 900;
        inline constexpr const int windowWidthMax = 1200; 

        inline constexpr const int windowHeightInit = 600;
        inline constexpr const int windowHeightMin = 600;
        inline constexpr const int windowHeightMax = 800;
    }

    namespace params
    {

        inline constexpr int maxMidiVoice = 8;
        inline constexpr int maxSpawnsPerBlock = 16;
        inline constexpr double maxFileDuration = 600.0; // 10 min
        inline constexpr uint64_t maxFileSize = 4LL * 512LL * 1024LL * 1024LL; // 512MB limits ULL -> Unsigned Long Long : 2GB
        inline constexpr int maxLutSize = 2048;
        inline constexpr int maxActiveGrains = 500;
        //inline constexpr float maxDuration = 10.0f; // seconds

        namespace attack
        {
            inline constexpr const char* id = "ATTACK";
            inline constexpr const char* name = "Attack";
            inline constexpr const float min = 0.0f;
            inline constexpr const float max = 10.f;
            inline constexpr const float init = 1.f;
        }

        namespace decay
        {
            inline constexpr const char* id = "DECAY";
            inline constexpr const char* name = "Decay";
            inline constexpr const float min = 0.0f;
            inline constexpr const float max = 10.f;
            inline constexpr const float init = 1.f;
        }
        namespace sustain
        {
            inline constexpr const char* id = "SUSTAIN";
            inline constexpr const char* name = "Sustain";
            inline constexpr const float min = 0.0f;
            inline constexpr const float max = 1.f;
            inline constexpr const float init = 0.5f;
        }
        namespace release
        {
            inline constexpr const char* id = "RELEASE";
            inline constexpr const char* name = "Release";
            inline constexpr const float min = 0.0f;
            inline constexpr const float max = 10.f;
            inline constexpr const float init = 1.f;
        }

        namespace play
        {
            inline constexpr const char* id = "PLAY";
            inline constexpr const char* name = "Play";
            inline constexpr float min = 0.f;
            inline constexpr float max = 1.0f;
            inline constexpr float skewFactor = 0.5f;
            inline constexpr float init = 0.0f;
            //inline constexpr bool init = false;
        }
        namespace position
        {
            inline constexpr const char* id = "POSITION";
            inline constexpr const char* name = "Position";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 1.f;
            inline constexpr float init = .0f;
        }

        namespace span
        {
            inline constexpr const char* id = "SPAN";
            inline constexpr const char* name = "Span";
            inline constexpr float min = 0.01f;
            inline constexpr float max = 1.f;
            inline constexpr float init = 0.0f;
        }
        namespace output
        {
            inline constexpr const char* id = "OUTPUT";
            inline constexpr const char* name = "Output";
            inline constexpr float min = -96.0f;
            inline constexpr float max = 12.0f;
            inline constexpr float init = -3.0f;
            inline constexpr float skewFactor = 0.0f;
        }

        namespace emission
        {
            inline constexpr const char* id = "EMISSION"; // grain emissions per sec
            inline constexpr const char* name = "Emission";
            inline constexpr float min = 0.1f; // 0.1 grain per seconds (1 grain / 10 sec)
            inline constexpr float max = 50.f; // 50 grains per seconds (1 grain / 0.002 s)
            inline constexpr float init = 5.0f;
            inline constexpr float skewFactor = 5.0f;
        }
        namespace emission_jitter
        {
            inline constexpr const char* id = "EMISSIONJITTER";
            inline constexpr const char* name = "Emission Jitter";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 1.0f;
            inline constexpr float init = 0.0f;
            inline constexpr float skewFactor = 0.5f;
        }
        namespace duration
        {
            inline constexpr const char* id = "DURATION"; // length of emitted grains in sec
            inline constexpr const char* name = "Duration";
            inline constexpr float min = 0.02f; // 0.002s for every emitted grains (1/ 0.002 = 500)
            inline constexpr float max = 10.0f; // 10 sec for every emitted grains (1/ 10 = 0.1)
            inline constexpr float init = 0.2f;
            inline constexpr float skewFactor = 0.2f;
        }
        namespace duration_jitter
        {
            inline constexpr const char* id = "DURATIONJITTER";
            inline constexpr const char* name = "Duration Jitter";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 1.0f;
            inline constexpr float init = 0.0f;
            inline constexpr float skewFactor = 0.5f;
        }
        namespace pitch
        {
            inline constexpr const char* id = "PITCH";
            inline constexpr const char* name = "Pitch";
            inline constexpr float min = -12.f;
            inline constexpr float max = 12.f;
            inline constexpr float init = 0.0f;
        }
        namespace speed
        {
            inline constexpr const char* id = "SPEED";
            inline constexpr const char* name = "Speed";
            inline constexpr float min = 0.1f;
            inline constexpr float max = 2.f;
            inline constexpr float init = 1.f;
            inline constexpr float skewFactor = 1.0f;
        }
        namespace speed_jitter {
            inline constexpr const char* id = "SPEEDJITTER";
            inline constexpr const char* name = "Speed Jitter";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 1.0f;
            inline constexpr float init = 0.0f;
            inline constexpr float skewFactor = 0.5f;
        }
        namespace pan
        {
            inline constexpr const char* id = "PAN";
            inline constexpr const char* name = "Pan";
            inline constexpr float min = 0.f;
            inline constexpr float max = 1.f;
            inline constexpr float init = 0.5f;
            inline constexpr float skewFactor = 0.5f;
        }
        namespace pan_jitter
        {
            inline constexpr const char* id = "PANJITTER";
            inline constexpr const char* name = "Pan Jitter";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 1.0f;
            inline constexpr float init = 0.0f;
            inline constexpr float skewFactor = 0.5f;
        }

        namespace envelopeMode
        {
            inline constexpr const char* id = "ENVMODE";
            inline constexpr const char* name = "Envelope Mode";
            inline constexpr const int init = 0;
            inline constexpr std::array<const char*, static_cast<int>(EnvelopeMode::Count)> envModeNames = {
                "Hann", "Linear", "Sqrt", "Gaussian", "Exponential"};

        }
        namespace envelopeRatio
        {
            inline constexpr const char* id = "ENVELOPERATIO";
            inline constexpr const char* name = "Envelope Ratio";
            inline constexpr float min = 0.f;
            inline constexpr float max = 0.95f;
            inline constexpr float init = 0.0f;
            inline constexpr float skewFactor = 0.25f;
        }
        namespace envelopeRatio_jitter
        {
            inline constexpr const char* id = "ENVRATIOJITTER";
            inline constexpr const char* name = "EnvelopeRatio Jitter";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 1.0f;
            inline constexpr float init = 0.0f;
            inline constexpr float skewFactor = 0.5f;
        }

        namespace traversalMode
        {
            inline constexpr const char* id = "TRAVERSALMODE";
            inline constexpr const char* name = "Traversal Mode";
            inline constexpr int init = 0;
            inline constexpr std::array<const char*, static_cast<int>(TraversalMode::Count)> traversalModeNames = {
                "Sine", "Triangle", "Square", "Random", "None"};
            //#define TRAVERSALMODE_6 "Saw Tooth"
            //#define TRAVERSALMODE_7 "Reverse Saw Tooth"
        }

        namespace traversalFreq
        {
            inline constexpr const char* id = "TRAVERSALFREQ";
            inline constexpr const char* name = "Traversal Frequency";
            inline constexpr float min = 0.01f; // low
            inline constexpr float max = 10.f; // high
            inline constexpr float init = 1.0f;
            inline constexpr float skewFactor = 1.0f;
        }
        namespace traversalFreq_jitter
        {
            inline constexpr const char* id = "TRAVERSALFREQJITTER";
            inline constexpr const char* name = "TraversalFreq Jitter";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 1.0f;
            inline constexpr float init = 0.0f;
            inline constexpr float skewFactor = 0.5f;
        }
        namespace playback
        {
            inline constexpr const char* id = "PLAYBACK";
            inline constexpr const char* name = "Playback";
            inline constexpr float min = -1.0f;
            inline constexpr float max = 1.0f;
            inline constexpr float init = 1.0f;
            inline constexpr float skewFactor = 0.0f;
        }
    }

}
