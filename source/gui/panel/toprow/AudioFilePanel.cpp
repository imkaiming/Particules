#include "AudioFilePanel.h"

#include "../../../framework/GuiTypes.h"
#include "../../../framework/bridge/EngineState.h"
#include "../../../framework/bridge/UIState.h"
#include "../../../utils/math/TimeUtils.h"
#include "../../../utils/struct/ProcessorFacade.h"
#include "../../../utils/struct/UIContext.h"
#include "../../lookandfeelv2/Colours.h"

namespace particules
{
    AudioFilePanel::AudioFilePanel(UIContext& uic)
        : uic{uic}, apvts{uic.apvts}, thumbnailComponent{uic}, grainVisualComponent{uic}, posParam{nullptr}, spanParam{nullptr},
          lastNumGrains{-1}, lastPos{-1.0f}, lastSpan{-1.0f}, numSamples{0} /*, lastPlayState{false}*/
    {
        uic.uiState.addChangeListener(this); // uistate send a message at setSource(file), audio file panel is listening

        startTimerHz(static_cast<int>(gui::refreshRate));

        sliderOnWaveform = std::make_unique<SliderOnWaveform>(apvts, params::position::id, params::span::id);

        std::function<void()> onThumbnailReady = [this]() {
            sliderOnWaveform->setAudioLoaded(true);
            grainVisualComponent.setNumSamples(this->uic.engineState.getNumSamples());
        };

        thumbnailComponent.setCallbackOnThumbnailReady(onThumbnailReady);

        posParam = apvts.getRawParameterValue(params::position::id);
        spanParam = apvts.getRawParameterValue(params::span::id);

        fileNameLabel.setInterceptsMouseClicks(false, false);
        numGrainsLabel.setInterceptsMouseClicks(false, false);
        posLabel.setInterceptsMouseClicks(false, false);
        spanLabel.setInterceptsMouseClicks(false, false);

        color labelColor = coloursv2::perleBlanc.withAlpha(0.5f);
        fileNameLabel.setColour(juce::Label::textColourId, labelColor);
        numGrainsLabel.setColour(juce::Label::textColourId, labelColor);
        posLabel.setColour(juce::Label::textColourId, labelColor);
        spanLabel.setColour(juce::Label::textColourId, labelColor);

        fileNameLabel.getProperties().set("isText", true);
        numGrainsLabel.getProperties().set("isText", true);
        posLabel.getProperties().set("isText", true);
        spanLabel.getProperties().set("isText", true);

        fileNameLabel.setJustificationType(juce::Justification::topLeft);
        numGrainsLabel.setJustificationType(juce::Justification::topRight);
        posLabel.setJustificationType(juce::Justification::centredRight);
        spanLabel.setJustificationType(juce::Justification::centredLeft);

        fileNameLabel.setText("", juce::dontSendNotification);
        numGrainsLabel.setText("GRAINS: 0", juce::dontSendNotification);
        posLabel.setText("POS: -", juce::dontSendNotification);
        spanLabel.setText("SPAN: -", juce::dontSendNotification);

        addAndMakeVisible(&thumbnailComponent);
        addAndMakeVisible(&grainVisualComponent);
        addAndMakeVisible(*sliderOnWaveform);

        addAndMakeVisible(&fileNameLabel);
        addAndMakeVisible(&numGrainsLabel);
        addAndMakeVisible(&posLabel);
        addAndMakeVisible(&spanLabel);

        sliderOnWaveform->setAudioLoaded(false); // change to true to debug UI
        sliderOnWaveform->setEnabled(false);
        posLabel.setVisible(false);
        spanLabel.setVisible(false);
        fileNameLabel.setVisible(false);
        numGrainsLabel.setVisible(false);

        changeListenerCallback(&uic.uiState);
    }

    AudioFilePanel::~AudioFilePanel()
    {
        stopTimer();
        uic.uiState.removeChangeListener(this);
    }

    void AudioFilePanel::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if(source == &uic.uiState)
        {
            if(uic.uiState.isFileLoaded())
            {
                numSamples = uic.engineState.getNumSamples();
                const juce::File& currentFile = uic.uiState.getCurrentFile();

                sliderOnWaveform->setAudioLoaded(true);
                sliderOnWaveform->setEnabled(true);
                sliderOnWaveform->setAlpha(1.0f);

                grainVisualComponent.setNumSamples(numSamples);

                fileNameLabel.setText(currentFile.getFileName(), juce::dontSendNotification);
                fileNameLabel.setVisible(true);
                posLabel.setVisible(true);
                spanLabel.setVisible(true);
                numGrainsLabel.setVisible(true);
            }
            else
            {
                sliderOnWaveform->setAudioLoaded(false);
                sliderOnWaveform->setEnabled(false);
                sliderOnWaveform->setAlpha(0.2f);

                fileNameLabel.setVisible(false);
                posLabel.setVisible(false);
                spanLabel.setVisible(false);
                numGrainsLabel.setVisible(false);
            }
        }
    }

    // we use timercallback to update the value instead of onValueChange to dont rely on send notification host
    // or to avoid to trigger onValueChange 3000time with automations.
    void AudioFilePanel::timerCallback()
    {
        const int numGrains = uic.engineState.getNumActiveGrains();
        if(numGrains != lastNumGrains)
        {
            numGrainsLabel.setText(str::formatted("GRAINS: %d", numGrains), juce::dontSendNotification);
            lastNumGrains = numGrains;
        }

        if(posParam != nullptr)
        {
            const float posVal = posParam->load(std::memory_order_relaxed);
            if(posVal != lastPos)
            {
                posLabel.setText(
                    "POS: " + utils::formatSamplesToTime(posVal, uic.engineState.getSampleRate()), juce::dontSendNotification);
                lastPos = posVal;
            }
        }

        if(spanParam != nullptr)
        {
            const float spanVal = spanParam->load(std::memory_order_relaxed);
            if(spanVal != lastSpan)
            {
                spanLabel.setText(
                    "SPAN: " + utils::formatSamplesToTime(spanVal, uic.engineState.getSampleRate()), juce::dontSendNotification);
                lastSpan = spanVal;
            }
        }

        grainVisualComponent.repaint();
    }

    bool AudioFilePanel::isInterestedInFileDrag(const juce::StringArray& files)
    {
        for(str file : files)
        {
            if(file.endsWithIgnoreCase(".wav") || file.endsWithIgnoreCase(".aif") || file.endsWithIgnoreCase(".mp3"))
            {
                return true;
            }
        }
        return false;
    }

    void AudioFilePanel::filesDropped(const juce::StringArray& files, int x, int y)
    {
        //paramsView.setIsPlaying(false);
        //audioProcessor.release();
        //juce::RangedAudioParameter* playParameter = apvts.getParameter(params::play::id);
        //playParameter->setValueNotifyingHost(0.f);
        uic.facade.setPlaying(false);

        for(str file : files)
        {
            if(isInterestedInFileDrag(file))
            {
                uic.facade.loadFilePath(file);
            }
        }
    }

    void AudioFilePanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::perleBlanc.darker(0.1f)); }

    void AudioFilePanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();
        //juce::Rectangle<int> area = bounds.reduced(2);

        thumbnailComponent.setBounds(area);
        grainVisualComponent.setBounds(area);
        sliderOnWaveform->setBounds(area);

        const int pad = 8;
        const int labelHeight = 18;

        fileNameLabel.setBounds(area.getX() + pad, area.getY() + pad, 200, labelHeight);
        numGrainsLabel.setBounds(area.getRight() - 100 - pad, area.getY() + pad, 100, labelHeight);

        juce::Rectangle<int> posSpanArea = area.withHeight(labelHeight).withY(area.getBottom() - labelHeight - pad);
        posSpanArea = posSpanArea.withSizeKeepingCentre(240, labelHeight);

        posLabel.setBounds(posSpanArea.removeFromLeft(120).withTrimmedRight(4));
        spanLabel.setBounds(posSpanArea.withTrimmedLeft(4));
    }
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