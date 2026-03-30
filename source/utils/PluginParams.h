#pragma once
#include "../framework/Core.h"

// init parameters for the plugin

namespace particules
{
    namespace adsr
    {
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

    }
    namespace global
    {
        namespace mix
        {
            inline constexpr const char* id = "MIX";
            inline constexpr const char* name = "Mix";
            inline constexpr float min = 0.0f;
            inline constexpr float max = 100.f;
            inline constexpr float init = 100.f;
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
            inline constexpr float max = 0.0f;
            inline constexpr float init = -6.0f;
        }
    }
    namespace grains
    {
        namespace emission
        {
            inline constexpr const char* id = "EMISSION"; // grain emissions per sec
            inline constexpr const char* name = "Emission";
            inline constexpr float min = 0.1f; // 0.1 grain per seconds (1 grain / 10 sec)
            inline constexpr float max = 100.f; // 500 grains per seconds (1 grain / 0.002 s)
            inline constexpr float init = 1.0f;
            inline constexpr float skewFactor = 1.0f;
        }
        namespace duration
        {
            inline constexpr const char* id = "DURATION"; // length of emitted grains in sec
            inline constexpr const char* name = "Duration";
            inline constexpr float min = 0.02f; // 0.002s for every emitted grains (1/ 0.002 = 500)
            inline constexpr float max = 10.0f;  // 10 sec for every emitted grains (1/ 10 = 0.1)
            inline constexpr float init = 1.0f;
            inline constexpr float skewFactor = 1.0f;
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

        namespace envelopeMode
        {
            inline constexpr const char* id = "ENVMODE";
            inline constexpr const char* name = "Envelope Mode";
            inline constexpr const int init = 0;
            inline constexpr std::array<const char*, 7> envModeNames = {
                "Hann", "Triangular", "Blackman", "BlackmanHarris", "Gaussian", "Exponential", "SmoothedStep"};

        }

        namespace sustainRatio
        {
            inline constexpr const char* id = "SUSTAINRATIO";
            inline constexpr const char* name = "SustainRatio";
            inline constexpr float min = 0.f;
            inline constexpr float max = 0.9f;
            inline constexpr float init = 0.5f;
            inline constexpr float skewFactor = 0.5f;
        }

        namespace traversalMode
        {
            inline constexpr const char* id = "TRAVERSALMODE";
            inline constexpr const char* name = "Traversal Mode";
            inline constexpr int init = 0;
            inline constexpr std::array<const char*, 5> traversalModeNames = {"Sine", "Triangle", "Square", "Random", "None"};
            //#define TRAVERSALMODE_6 "Saw Tooth"
            //#define TRAVERSALMODE_7 "Reverse Saw Tooth"
        }

        namespace traversalFreq
        {
            inline constexpr const char* id = "TRAVERSALFREQ";
            inline constexpr const char* name = "Traversal Frequency";
            inline constexpr float min = 0.01f; // low
            inline constexpr float max = 50.f; // high
            inline constexpr float init = 1.0f;
            inline constexpr float skewFactor = 1.0f;
        }
    }

}
