#include "AudioFilePanel.h"

#include "framework/bridge/FromUI.h"
#include "framework/core/GuiTypes.h"
#include "framework/state/AudioState.h"
#include "framework/state/UIState.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/math/TimeUtils.h"
#include "utils/struct/UIContext.h"

namespace particules
{
    AudioFilePanel::AudioFilePanel(UIContext& uic)
        : uic{uic}, apvts{uic.apvts}, thumbnailComponent{uic.uiState.getAudioThumbnail()}, grainVisualComponent{uic},
          posParam{nullptr}, spanParam{nullptr},
          lastNumGrains{-1}, lastPos{-1.0f}, lastSpan{-1.0f}, numSamples{0}, uiState{uic.uiState}, audioState{uic.audioState}, fui{uic.fui}
    {
        uiState.addChangeListener(this); // uistate send a message at setSource(file), audio file panel is listening

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
        grainsLabel.setInterceptsMouseClicks(false, false);
        numGrainsLabel.setInterceptsMouseClicks(false, false);
        posTitleLabel.setInterceptsMouseClicks(false, false);
        posLabel.setInterceptsMouseClicks(false, false);
        spanTitleLabel.setInterceptsMouseClicks(false, false);
        spanLabel.setInterceptsMouseClicks(false, false);

        color labelColor = coloursv2::perleBlanc.withAlpha(0.5f);
        fileNameLabel.setColour(juce::Label::textColourId, labelColor);
        grainsLabel.setColour(juce::Label::textColourId, labelColor);
        numGrainsLabel.setColour(juce::Label::textColourId, labelColor);
        posTitleLabel.setColour(juce::Label::textColourId, labelColor);
        posLabel.setColour(juce::Label::textColourId, labelColor);
        spanTitleLabel.setColour(juce::Label::textColourId, labelColor);
        spanLabel.setColour(juce::Label::textColourId, labelColor);

        fileNameLabel.getProperties().set("isText", true);
        grainsLabel.getProperties().set("isText", true);
        numGrainsLabel.getProperties().set("isText", true);
        posTitleLabel.getProperties().set("isText", true);
        posLabel.getProperties().set("isText", true);
        spanTitleLabel.getProperties().set("isText", true);
        spanLabel.getProperties().set("isText", true);

        fileNameLabel.setJustificationType(juce::Justification::topLeft);
        grainsLabel.setJustificationType(juce::Justification::topRight);
        numGrainsLabel.setJustificationType(juce::Justification::topLeft);
        posTitleLabel.setJustificationType(juce::Justification::centredRight);
        posLabel.setJustificationType(juce::Justification::centredLeft);
        spanTitleLabel.setJustificationType(juce::Justification::centredRight);
        spanLabel.setJustificationType(juce::Justification::centredLeft);

        fileNameLabel.setText("", juce::dontSendNotification);
        grainsLabel.setText("GRAINS: ", juce::dontSendNotification);
        numGrainsLabel.setText("", juce::dontSendNotification);
        posLabel.setText("POS: -", juce::dontSendNotification);
        spanLabel.setText("SPAN: -", juce::dontSendNotification);

        addAndMakeVisible(&thumbnailComponent);
        addAndMakeVisible(&grainVisualComponent);
        addAndMakeVisible(*sliderOnWaveform);

        addAndMakeVisible(&fileNameLabel);
        addAndMakeVisible(&numGrainsLabel);
        addAndMakeVisible(&grainsLabel);
        addAndMakeVisible(&posTitleLabel);
        addAndMakeVisible(&posLabel);
        addAndMakeVisible(&spanLabel);
        addAndMakeVisible(&spanTitleLabel);

        sliderOnWaveform->setAudioLoaded(false); // change to true to debug UI
        sliderOnWaveform->setEnabled(false);
        fileNameLabel.setVisible(false);
        grainsLabel.setVisible(false);
        numGrainsLabel.setVisible(false);
        posTitleLabel.setVisible(false);
        posLabel.setVisible(false);
        spanTitleLabel.setVisible(false);
        spanLabel.setVisible(false);

        changeListenerCallback(&uiState);
    }

    AudioFilePanel::~AudioFilePanel()
    {
        stopTimer();
        uiState.removeChangeListener(this);
    }

    void AudioFilePanel::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if(source == &uiState)
        {
            if(uiState.isFileLoaded())
            {
                numSamples = audioState.getNumSamples();
                const juce::File& currentFile = uiState.getCurrentFile();

                sliderOnWaveform->setAudioLoaded(true);
                sliderOnWaveform->setEnabled(true);
                sliderOnWaveform->setAlpha(1.0f);

                grainVisualComponent.setNumSamples(numSamples);
                //thumbnailComponent.paint();

                fileNameLabel.setText(currentFile.getFileName(), juce::dontSendNotification);
                fileNameLabel.setVisible(true);
                fileNameLabel.setVisible(true);
                grainsLabel.setVisible(true);
                numGrainsLabel.setVisible(true);
                posTitleLabel.setVisible(true);
                posLabel.setVisible(true);
                spanTitleLabel.setVisible(true);
                spanLabel.setVisible(true);
            }
            else
            {
                sliderOnWaveform->setAudioLoaded(false);
                sliderOnWaveform->setEnabled(false);
                sliderOnWaveform->setAlpha(0.2f);

                fileNameLabel.setVisible(false);
                grainsLabel.setVisible(false);
                numGrainsLabel.setVisible(false);
                posTitleLabel.setVisible(false);
                posLabel.setVisible(false);
                spanTitleLabel.setVisible(false);
                spanLabel.setVisible(false);
            }
        }
    }

    // we use timercallback to update the value instead of onValueChange to dont rely on send notification host
    // or to avoid to trigger onValueChange 3000time with automations.
    void AudioFilePanel::timerCallback()
    {
        const int numGrains = audioState.getNumActiveGrains();
        if(numGrains != lastNumGrains)
        {
            numGrainsLabel.setText(str::formatted("%d", numGrains), juce::dontSendNotification);
            lastNumGrains = numGrains;
        }

        const double totalSamples = static_cast<double>(audioState.getNumSamples());
        const double sampleRate = audioState.getSampleRate();

        if(posParam != nullptr)
        {
            const float posVal = posParam->load(std::memory_order_relaxed);
            if(posVal != lastPos)
            {
                const double absolutePosSamples = static_cast<double>(posVal) * totalSamples;
                posLabel.setText(
                    "POS: " + utils::formatSamplesToTime(absolutePosSamples, sampleRate), juce::dontSendNotification);
                lastPos = posVal;
            }
        }

        if(spanParam != nullptr)
        {
            const float spanVal = spanParam->load(std::memory_order_relaxed);
            if(spanVal != lastSpan)
            {
                const double absoluteSpanSamples = static_cast<double>(spanVal) * totalSamples;
                spanLabel.setText(
                    "SPAN: " + utils::formatSamplesToTime(absoluteSpanSamples, sampleRate), juce::dontSendNotification);
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
        //fui.stop();
        //juce::RangedAudioParameter* playParameter = apvts.getParameter(params::play::id);
        //playParameter->setValueNotifyingHost(0.f);
        //fui.setPlaying(false);

        for(str file : files)
        {
            if(isInterestedInFileDrag(file))
            {
                fui.loadFilePath(file);
            }
        }
    }

    void AudioFilePanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::perleBlanc.darker(0.1f)); }

void AudioFilePanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();

        thumbnailComponent.setBounds(area);
        grainVisualComponent.setBounds(area);
        sliderOnWaveform->setBounds(area);

        const int pad = 8;
        const int labelHeight = 18;

        fileNameLabel.setBounds(area.getX() + pad, area.getY() + pad, 200, labelHeight);

        juce::Rectangle<int> grainsArea(area.getRight() - 130 - pad, area.getY() + pad, 130, labelHeight);
        grainsLabel.setBounds(grainsArea.removeFromLeft(80));
        numGrainsLabel.setBounds(grainsArea);

        const int totalBottomWidth = juce::jmin(area.getWidth() - (pad * 2), 360);

        juce::Rectangle<int> posSpanArea = area.withHeight(labelHeight).withY(area.getBottom() - labelHeight - pad);
        posSpanArea = posSpanArea.withSizeKeepingCentre(totalBottomWidth, labelHeight);

        juce::Rectangle<int> posBlock = posSpanArea.removeFromLeft(totalBottomWidth / 2).withTrimmedRight(10);
        juce::Rectangle<int> spanBlock = posSpanArea.withTrimmedLeft(10);

        posTitleLabel.setBounds(posBlock.removeFromLeft(40));
        posLabel.setBounds(posBlock);

        spanTitleLabel.setBounds(spanBlock.removeFromLeft(45));
        spanLabel.setBounds(spanBlock);
    }
}