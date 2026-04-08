#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace particules
{
    class LabelBoxLookAndFeel : public juce::LookAndFeel_V4
    {
    public:
        LabelBoxLookAndFeel()
        {
            setColour(juce::Label::backgroundColourId, juce::Colours::black.withAlpha(0.4f));
            setColour(juce::Label::outlineColourId, juce::Colours::black.withAlpha(0.8f));
        };

        ~LabelBoxLookAndFeel() = default;

        void drawLabel(juce::Graphics& g, juce::Label& label) override
        {
            const float availableWidth = static_cast<float>(label.getWidth()) - 12.0f;

            juce::Rectangle<float> area = label.getLocalBounds().toFloat();

            g.setColour(juce::Colours::black.withAlpha(0.35f));
            g.fillRoundedRectangle(area, 6.0f);

            g.setColour(juce::Colours::black.withAlpha(0.8f));
            g.drawRoundedRectangle(area, 6.0f, 1.0f);

            g.setColour(label.findColour(juce::Label::textColourId));
            g.setFont(label.getFont());

            g.drawFittedText(smartEllipsizeFilename(label.getText(), label.getFont(), availableWidth), label.getLocalBounds(),
                label.getJustificationType(), 1);
        }

        static str smartEllipsizeFilename(const str& fullPath, const juce::Font& font, float maxWidth)
        {
            if(fullPath.isEmpty() || maxWidth <= 0.0f)
                return {};

            const bool looksLikePath = fullPath.containsChar('/') || fullPath.containsChar('\\');
            const str fileName = looksLikePath ? juce::File(fullPath).getFileName() : fullPath;

            if(font.getStringWidthFloat(fileName) <= maxWidth)
                return fileName;

            const int dot = fileName.lastIndexOfChar('.');
            const str base = dot > 0 ? fileName.substring(0, dot) : fileName;
            const str ext = dot > 0 ? fileName.substring(dot) : str{};

            const str ellipsis = "...";
            const float extWidth = font.getStringWidthFloat(ellipsis + ext);

            if(extWidth >= maxWidth)
                return ellipsis;

            const float budgetForBase = maxWidth - extWidth;

            for(int i = base.length(); i > 0; --i)
            {
                if(font.getStringWidthFloat(base.substring(0, i)) <= budgetForBase)
                    return base.substring(0, i) + ellipsis + ext;
            }

            return ellipsis + ext;
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabelBoxLookAndFeel)
    };
}