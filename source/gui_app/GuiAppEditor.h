/**
 * This class is the graphical user interface for the plugin and is responsible
 * for displaying the controls and parameters.
*/

#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_core/juce_core.h>

#include "../gui/lookandfeelv2/MainLNF.h"
#include "../gui/panel/MainPanel.h"

namespace juce
{
    class Graphics;
}

namespace particules
{
    class GuiAppProcessor;
    class GuiAppEditor : public juce::AudioProcessorEditor
    {
    public:
        GuiAppEditor(GuiAppProcessor&);
        ~GuiAppEditor() override;

        void resized() override;

        const MainPanel& getMainPanel() const noexcept { return mainPanel; };

    private:
        MainLNF lnf;
        MainPanel mainPanel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GuiAppEditor)
    };
}