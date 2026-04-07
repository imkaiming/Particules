#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    namespace UIHelpers
    {
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