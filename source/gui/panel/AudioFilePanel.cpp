#include "AudioFilePanel.h"
#include "../../PluginProcessor.h"
#include "../../framework/ParameterView.h"
#include "../../utils/MyColours.h"
#include "../../utils/struct/UIContext.h"
#include "BinaryData.h"

namespace particules
{
    AudioFilePanel::AudioFilePanel(UIContext& uic)
        : paramsView(uic.paramsView), apvts{uic.apvts}, audioProcessor(uic.audioProcessor), thumbnailComponent(uic)
    {
        audioProcessor.addChangeListener(this); // audio processor can now send message

        positionSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, globalPositionId, positionSlider);
        spanSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, globalSpanId, spanSlider);

        positionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
        positionSlider.setRange(globalPositionMin, globalPositionMax);

        spanSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        spanSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
        spanSlider.setTextBoxIsEditable(true);
        spanSlider.setRange(globalSpanMin, globalSpanMax);

        addAndMakeVisible(&positionSlider);
        addAndMakeVisible(&spanSlider);
        addAndMakeVisible(&thumbnailComponent);
    }

    AudioFilePanel::~AudioFilePanel() { audioProcessor.removeChangeListener(this); }

    void AudioFilePanel::filesDropped(const juce::StringArray& files, int x, int y)
    {
        //paramsView.setIsPlaying(false);
        //audioProcessor.release();
        for(juce::String file : files)
        {
            if(isInterestedInFileDrag(file))
            {
                audioProcessor.loadFile(file);
            }
        }
    }

    bool AudioFilePanel::isInterestedInFileDrag(const juce::StringArray& files)
    {
        for(juce::String file : files)
        {
            if(file.endsWithIgnoreCase(".wav") || file.endsWithIgnoreCase(".aif") || file.endsWithIgnoreCase(".mp3"))
            {
                return true;
            }
        }
        return false;
    }

    void AudioFilePanel::paint(juce::Graphics& g)
    {
        const juce::Rectangle<float> inner = getLocalBounds().reduced(2).toFloat();
        g.setColour(colours::brightBlue);
        g.fillRoundedRectangle(inner, 12.0f);
    }

    void AudioFilePanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds().reduced(10, 10);

        const int sliderH = 16;
        const int gap = 4;

        spanSlider.setBounds(area.removeFromTop(sliderH));
        area.removeFromTop(gap);

        const juce::Rectangle<int> thumbArea = area.withTrimmedBottom(sliderH + gap);
        thumbnailComponent.setBounds(thumbArea);

        area.removeFromTop(thumbArea.getHeight() + gap);
        positionSlider.setBounds(area.removeFromTop(sliderH));
    }

    // TODO : Remove the change listener callback after MIDI implementation
    void AudioFilePanel::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if(source == &audioProcessor)
        {
            const juce::File& f = audioProcessor.getCurrentFile();
            bool valid = f.existsAsFile();
            //play_pause_btn.setEnabled(valid);
        }
    }

}

/*
 
     AudioFilePanel::AudioFilePanel(UIContext& uic)
        : paramsView(uic.paramsView), audioProcessor(uic.audioProcessor), thumbnailComponent(uic)
    {
        audioProcessor.addChangeListener(this); // audio processor can now send message


        std::function<void()> onThumbnailReady = [this]() {
            //juce::Logger::outputDebugString("10) callback after thumbnail painted");
            //play_btn.setEnabled(true);
        };

        thumbnailComponent.setCallbackOnThumbnailReady(onThumbnailReady);

        // audio file loader is a variable of audio file frame so it cannot outlive his parent.


        std::function<void(bool)> onFileLoaded = [this](bool ok) {
            // UI operation on the message thread
            juce::MessageManager::callAsync([this, ok]() {
                if(juce::MessageManager* m = juce::MessageManager::getInstance())
                {
                    if(m->currentThreadHasLockedMessageManager())
                    {
                        //std::shared_ptr<const SampleSource> src = paramsView.getSampleSource();
                        //play_btn.setEnabled(ok); //&& src);
                    }
                }
            });
        };
}

//audioProcessor.getAudioFileLoader().setOnFileLoadedCallBack(onFileLoaded);

    void AudioFilePanel::stopAudioButtonClicked()
    {
        //DBG("stop audio button clicked");
        //DBG("param is Playing = " + (str)(paramsView.getIsPlaying() ? "true" : "false"));
        paramsView.setIsPlaying(false);
        setPlayButtonImage();
    }

    void AudioFilePanel::playAudioButtonClicked()
    {
        if(paramsView.getIsPlaying())
        {
            paramsView.setIsPlaying(false);
            setPlayButtonImage();
        }
        else
        {
            paramsView.setIsPlaying(true);
            setPauseButtonImage();
        }
    }

    void AudioFilePanel::setPlayButtonImage()
    {
        play_pause_btn.setImages(juce::Drawable::createFromImageData(BinaryData::Play_svg, BinaryData::Play_svgSize).get(),
            juce::Drawable::createFromImageData(BinaryData::Play_Fill_svg, BinaryData::Play_Fill_svgSize).get(), nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr);
    }

    void AudioFilePanel::setPauseButtonImage()
    {
        play_pause_btn.setImages(juce::Drawable::createFromImageData(BinaryData::Pause_svg, BinaryData::Pause_svgSize).get(),
            juce::Drawable::createFromImageData(BinaryData::Pause_Fill_svg, BinaryData::Pause_Fill_svgSize).get(), nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr);
    }

    void AudioFilePanel::setStopButtonImage()
    {
        stop_btn.setImages(juce::Drawable::createFromImageData(BinaryData::Stop_svg, BinaryData::Stop_svgSize).get(),
            juce::Drawable::createFromImageData(BinaryData::Stop_Fill_svg, BinaryData::Stop_Fill_svgSize).get(), nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr);
    }

    void AudioFilePanel::setOpenButtonImage()
    {
        open_btn.setImages(juce::Drawable::createFromImageData(BinaryData::AddFolder_svg, BinaryData::AddFolder_svgSize).get(),
            juce::Drawable::createFromImageData(BinaryData::AddFolder_Fill_svg, BinaryData::AddFolder_Fill_svgSize).get(),
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    }


    */