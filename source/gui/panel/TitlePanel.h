
#pragma once

#include "../../framework/Core.h"
#include "../../framework/Types.h"
#include "../../utils/lookandfeel/LabelBoxLookAndFeel.h"

namespace particules
{
    class ParameterView;
    class ParticulesAudioProcessor;
    class UIContext;
    class LabelBoxLookAndFeel;
    class TitlePanel : public juce::Component
    {
    public:
        TitlePanel(UIContext& uic);
        ~TitlePanel();

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        static constexpr const char* fileNameBoxPlaceHolder = "audio_file_03.wav";

        ParameterView& paramsView;
        ParticulesAudioProcessor& audioProcessor;

        juce::Label titleLabel;
        juce::TextButton loadButton;
        juce::Label fileNameBox;

        juce::DrawableButton pauseBtn;
        juce::DrawableButton playBtn;

        LabelBoxLookAndFeel lookAndFeel;
        juce::Component leftArea, loadArea, fileArea, rightArea, btnArea;

        void setPlayButtonImage();
        void setPauseButtonImage();

        void pauseButtonClicked();
        void playButtonClicked();

        void layoutLeft();
        void layoutLoad();
        void layoutFile();
        void layoutRight();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TitlePanel)
    };
}