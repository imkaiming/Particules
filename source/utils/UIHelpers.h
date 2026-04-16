#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    namespace UIHelpers
    {
        inline float getStandardVisualRadius(float cellWidth, float cellHeight)
        {
            const float minDim = juce::jmin(cellWidth, cellHeight);
            const float maxExtentFromCenter = minDim * 0.62f;
            const float auxSize = juce::jlimit(18.0f, 32.0f, minDim * 0.18f);

            const float jitterOuterMultiplier = 1.10f;
            const float auxGap = 4.0f;
            const float safetyPadding = 3.0f;
            const float jitterLineWidthHalf = 1.0f;

            const float maxAuxCenterDist = maxExtentFromCenter - safetyPadding - (auxSize * 0.5f);
            const float availableForJitter = maxAuxCenterDist - auxGap - (auxSize * 0.5f);

            return juce::jmax(1.0f, (availableForJitter - jitterLineWidthHalf) / jitterOuterMultiplier);
        }

        inline std::unique_ptr<juce::Drawable> loadSVG(const char* data, int size, juce::Colour c)
        {
            auto d = juce::Drawable::createFromImageData(data, size);
            if(d)
                d->replaceColour(juce::Colours::black, c);
            return d;
        }

        // combo box
        inline std::unique_ptr<juce::Drawable> loadSVGWithMap(
            const void* data, size_t size, const std::map<juce::Colour, juce::Colour>& colourMap)
        {
            auto d = juce::Drawable::createFromImageData(data, size);
            if(d)
            {
                for(auto const& [oldCol, newCol] : colourMap)
                    d->replaceColour(oldCol, newCol);
            }
            return d;
        }
    }
}