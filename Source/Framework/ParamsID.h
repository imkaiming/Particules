#pragma once

namespace Param
{
    namespace Mix
    {
        inline constexpr const char* id = "MIX";
        inline constexpr const char* name = "Mix";
        inline constexpr float min = 0.0f;
        inline constexpr float max = 100.f;
        inline constexpr float init = 100.f;
    }
    namespace Gain
    {
        inline constexpr const char* id = "OUTPUT";
        inline constexpr const char* name = "Output";
        inline constexpr float min = -96.0f;
        inline constexpr float max = 0.0f;
        inline constexpr float init = -6.0f;
    }
    namespace Emission
    {
        inline constexpr const char* id = "EMISSION"; // grain emissions per sec
        inline constexpr const char* name = "Emission";
        inline constexpr float min = 0.1f; // 0.1 grain per seconds (1 grain / 10 sec)
        inline constexpr float max = 500.f; // 500 grains per seconds (1 grain / 0.002 s)
        inline constexpr float init = 1.0f;
    }
    namespace Duration
    {
        inline constexpr const char* id = "DURATION"; // length of emitted grains in sec
        inline constexpr const char* name = "Duration";
        inline constexpr float min = 0.002f; // 0.002s for every emitted grains (1/ 0.002 = 500)
        inline constexpr float max = 10.0f; // 10 sec for every emitted grains (1/ 10 = 0.1)
        inline constexpr float init = 1.0f;
    }
    namespace Pitch
    {
        inline constexpr const char* id = "PITCH";
        inline constexpr const char* name = "Pitch";
        inline constexpr float min = -12.f;
        inline constexpr float max = 12.f;
        inline constexpr float init = 0.0f;
    }
    namespace Speed
    {
        inline constexpr const char* id = "SPEED";
        inline constexpr const char* name = "Speed";
        inline constexpr float min = 0.1f;
        inline constexpr float max = 2.f;
        inline constexpr float init = 1.f;
    }

    namespace EnvelopeType
    {
        inline constexpr const char* id = "ENVTYPE";
        inline constexpr const char* name = "Envelope Type";
        inline constexpr const int init = 1;
        inline constexpr std::array<const char*, 10> envTypeNames = {"Hann", "Triangular", "Hamming", "Blackman", "BlackmanHarris",
            "FlatTop", "Gaussian", "Exponential", "SmoothedStep", "Tukey"};

        //inline constexpr const char* ENVTYPE_1 = "Hann";
        //inline constexpr const char* ENVTYPE_2 = "Triangular";
        //inline constexpr const char* ENVTYPE_3 = "Hamming";
        //inline constexpr const char* ENVTYPE_4 = "Rectangular";
        //inline constexpr const char* ENVTYPE_5 = "Blackman";
        //inline constexpr const char* ENVTYPE_6 = "BlackmanHarris";
        //inline constexpr const char* ENVTYPE_7 = "FlatTop";
    }

    namespace Position
    {
        inline constexpr const char* id = "POSITION";
        inline constexpr const char* name = "Position";
        inline constexpr float min = 0.0f;
        inline constexpr float max = 1.f;
        inline constexpr float init = .0f;
    }
    namespace Selection
    {
        inline constexpr const char* id = "SELECTION";
        inline constexpr const char* name = "Selection";
        inline constexpr float min = 0.01f;
        inline constexpr float max = 1.f;
        inline constexpr float init = 0.0f;
    }
    namespace SustainRatio // TODO to rename to FadeRatio or SustainRatio
    {
        inline constexpr const char* id = "SUSTAINRATIO";
        inline constexpr const char* name = "SustainRatio";
        inline constexpr float min = 0.f;
        inline constexpr float max = 0.9f;
        inline constexpr float init = 0.5f;
    }

    namespace TraversalMode
    {
        inline constexpr const char* id = "TRAVERSALMODE";
        inline constexpr const char* name = "Traversal Mode";
        inline constexpr int init = 1;
        inline constexpr std::array<const char*, 5> traversalModeNames = {"Sinus", "Square", "Triangular", "Random", "None"};
        inline constexpr const char* TraversalMode_1 = "Sinus";
        inline constexpr const char* TraversalMode_2 = "Square";
        inline constexpr const char* TraversalMode_3 = "Triangular";
        inline constexpr const char* TraversalMode_4 = "Random";
        inline constexpr const char* TraversalMode_5 = "None";

        //#define TRAVERSALMODE_6 "Saw Tooth"
        //#define TRAVERSALMODE_7 "Reverse Saw Tooth"
    }

    namespace TraversalTime
    {
        inline constexpr const char* id = "TRAVERSALTIME";
        inline constexpr const char* name = "Traversal Time";
        inline constexpr float min = 0.01f;
        inline constexpr float max = 10.f;
        inline constexpr float init = 1.0f;
    }

        namespace TraversalFreq
    {
        inline constexpr const char* id = "TRAVERSALFREQ";
        inline constexpr const char* name = "Traversal Frequency";
        inline constexpr float min = 0.01f; // low
        inline constexpr float max = 100.f; // high
        inline constexpr float init = 10.0f;
    }
    namespace Util
    {

        inline constexpr float grainPointSize = 10.f;
        inline constexpr float fftSizeOrder = 10.f;
        //inline constexpr float init = 1.0f;
    }
}
