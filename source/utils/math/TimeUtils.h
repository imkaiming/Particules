#pragma once
#include <cmath>
#include <juce_core/juce_core.h>

namespace particules
{
    namespace utils
    {
        // Formate des secondes brutes en hh:mm:ss.sss, mm:ss.sss, ou ss.sss
        [[nodiscard]] inline juce::String formatTime(double totalSeconds)
        {
            if(totalSeconds <= 0.0)
                return "0.000 s";

            const int hours = static_cast<int>(totalSeconds / 3600.0);
            const int minutes = static_cast<int>(std::fmod(totalSeconds / 60.0, 60.0));
            const double seconds = std::fmod(totalSeconds, 60.0);

            if(hours > 0)
                return juce::String::formatted("%02d:%02d:%06.3f", hours, minutes, seconds);
            if(minutes > 0)
                return juce::String::formatted("%02d:%06.3f", minutes, seconds);

            return juce::String::formatted("%.3f s", seconds);
        }

        [[nodiscard]] inline juce::String formatSamplesToTime(double samples, double sampleRate)
        {
            if(sampleRate <= 0.0)
                return "0.000 s";

            return formatTime(samples / sampleRate);
        }
    }
}