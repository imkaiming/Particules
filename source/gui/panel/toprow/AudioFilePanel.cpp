#include "AudioFilePanel.h"

#include "framework/state/AudioState.h"
#include "framework/state/UIState.h"
#include "framework/bridge/FromUI.h"
#include "framework/core/GuiTypes.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/math/TimeUtils.h"
#include "utils/struct/UIContext.h"

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
            grainVisualComponent.setNumSamples(this->uic.audioState.getNumSamples());
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
                numSamples = uic.audioState.getNumSamples();
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
        const int numGrains = uic.audioState.getNumActiveGrains();
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
                    "POS: " + utils::formatSamplesToTime(posVal, uic.audioState.getSampleRate()), juce::dontSendNotification);
                lastPos = posVal;
            }
        }

        if(spanParam != nullptr)
        {
            const float spanVal = spanParam->load(std::memory_order_relaxed);
            if(spanVal != lastSpan)
            {
                spanLabel.setText(
                    "SPAN: " + utils::formatSamplesToTime(spanVal, uic.audioState.getSampleRate()), juce::dontSendNotification);
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
        uic.fui.setPlaying(false);

        for(str file : files)
        {
            if(isInterestedInFileDrag(file))
            {
                uic.fui.loadFilePath(file);
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