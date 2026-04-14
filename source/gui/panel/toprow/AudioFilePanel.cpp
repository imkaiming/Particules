#include "AudioFilePanel.h"

#include "../../../framework/GuiTypes.h"
#include "../../../framework/bridge/UIState.h"
#include "../../../utils/struct/ProcessorFacade.h"
#include "../../../utils/struct/UIContext.h"
#include "../../lookandfeelv2/Colours.h"

namespace particules
{
    AudioFilePanel::AudioFilePanel(UIContext& uic) : apvts{uic.apvts}, thumbnailComponent{uic}, uiState{uic.uiState}, uic{uic}
    {
        //uiState.addChangeListener(this); // can now send message

        waveformOverlay = std::make_unique<WaveformOverlay>(apvts, params::position::id, params::span::id);

        std::function<void()> onThumbnailReady = [this]() {
            waveformOverlay->setAudioLoaded(true);
        };

        thumbnailComponent.setCallbackOnThumbnailReady(onThumbnailReady);

        addAndMakeVisible(&thumbnailComponent);
        addAndMakeVisible(*waveformOverlay);

        //updatePosition(uic.apvts.getRawParameterValue(globalPositionId)->load());
        //updateSpan(uic.apvts.getRawParameterValue(globalSpanId)->load());
        waveformOverlay->setAudioLoaded(true);
    }

    void AudioFilePanel::filesDropped(const juce::StringArray& files, int x, int y)
    {
        //paramsView.setIsPlaying(false);
        //audioProcessor.release();
        juce::RangedAudioParameter* playParameter = apvts.getParameter(params::play::id);
        playParameter->setValueNotifyingHost(0.f);

        for(str file : files)
        {
            if(isInterestedInFileDrag(file))
            {
                uic.facade.loadFilePath(file);
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

    void AudioFilePanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::perleBlanc.darker(0.1f)); }

    void AudioFilePanel::resized()
    {
        juce::Rectangle<int> bounds = getLocalBounds();

        const int padY = 2;
        const int padX = 8;

        const int totalH = bounds.getHeight();

        const int sliderH = (int)(totalH * 0.10f);
        const int thumbH = (int)(totalH * 0.80f);

        juce::Rectangle<int> topSliderArea = bounds.removeFromTop(sliderH);
        juce::Rectangle<int> thumbArea = bounds.removeFromTop(thumbH);
        juce::Rectangle<int> bottomSliderArea = bounds;

        juce::Rectangle<int> thumbReduced = thumbArea.reduced(padX, 0);
        thumbnailComponent.setBounds(thumbReduced);
        waveformOverlay->setBounds(thumbReduced);
    }
    //void AudioFilePanel::updatePosition(float position)
    //{
        //const float width = (float)positionOverlay.getWidth();

        //const float startPx = position * width;
        //const float spanPx = spanSlider.getValue() * width;
        //const float endPx = startPx + spanPx;

        //positionOverlay.setPosition(startPx);
        //spanOverlay.setPosition(startPx);

        //const float overflowPx = juce::jmax(0.0f, endPx - width);
        //updateOverflow(overflowPx);

        //positionOverlay.repaint();
        //spanOverlay.repaint();
        //overflowOverlay.repaint();
    }

    //void AudioFilePanel::updateSpan(float span)
    //{
        //const float width = (float)positionOverlay.getWidth();

        //const float startPx = positionSlider.getValue() * width;
        //const float spanPx = span * width;
        //const float endPx = startPx + spanPx;

        //spanOverlay.setSpan(spanPx);

        //const float overflowPx = juce::jmax(0.0f, endPx - width);
        //updateOverflow(overflowPx);

        //spanOverlay.repaint();
        //overflowOverlay.repaint();
    //}

    //void AudioFilePanel::updateOverflow(float value) { /*overflowOverlay.setSpan(value);*/ }

    /*
    void AudioFilePanel::resized()
    {
        juce::Rectangle<int> sliderArea = getBounds().reduced(0,2);
        juce::Rectangle<int> area = getLocalBounds().reduced(10, 2);

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::column;
        fb.alignItems = juce::FlexBox::AlignItems::stretch;

        fb.items.add(juce::FlexItem(spanSlider).withFlex(0.10f));
        fb.items.add(juce::FlexItem(thumbnailComponent).withFlex(0.80f));
        fb.items.add(juce::FlexItem(positionSlider).withFlex(0.10f));

        fb.performLayout(area);

        juce::Rectangle<int> thumbBounds = thumbnailComponent.getBounds();

        spanOverlay.setBounds(thumbBounds);
        positionOverlay.setBounds(thumbBounds);
        overflowOverlay.setBounds(thumbBounds);

        updatePosition(positionSlider.getValue());
        updateSpan(spanSlider.getValue());
    }
    */

    // TODO : Remove the change listener callback after MIDI implementation
    //void AudioFilePanel::changeListenerCallback(juce::ChangeBroadcaster* source)
    //{
    //    if(source == &audioProcessor)
    //    {
    //        const juce::File& f = audioProcessor.getCurrentFile();
    //        bool valid = f.existsAsFile();
    //        //play_pause_btn.setEnabled(valid);
    //    }
    //}

//}

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