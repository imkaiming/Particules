#include "TitlePanel.h"

#include <juce_graphics/juce_graphics.h>


#include "../../PluginProcessor.h"
#include "../../utils/PluginParams.h"
#include "../../utils/struct/UIContext.h"
#include "../lookandfeel/MyColours.h"
#include "BinaryData.h"

namespace particules
{

    TitlePanel::TitlePanel(UIContext& uic)
        : playBtn((const str) "playBtn", juce::DrawableButton::ButtonStyle::ImageFitted),
          pauseBtn((const str) "pauseBtn", juce::DrawableButton::ButtonStyle::ImageFitted), uic{uic},
          loadFileCallback{uic.facade.loadFilePath}
    {
        setPauseButtonImage();
        setPlayButtonImage();

        pauseBtn.onClick = [this]() { pauseButtonClicked(); };
        playBtn.onClick = [this]() { playButtonClicked(); };
        loadBtn.onClick = [this]() { loadSampleButtonClicked(); };

        titleLabel.setText("Particules", juce::dontSendNotification);
        titleLabel.setJustificationType(juce::Justification::centred);
        titleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
        titleLabel.setFont(juce::Font(18.0f));
        titleLabel.setSize(100, static_cast<int>(titleLabel.getFont().getHeight()));

        loadBtn.setButtonText("Load Sample");
        loadBtn.setEnabled(true);
        loadBtn.setColour(juce::Label::textColourId, juce::Colours::white);

        fileNameBox.setJustificationType(juce::Justification::centred);
        fileNameBox.setColour(juce::Label::textColourId, juce::Colours::white);
        fileNameBox.setMinimumHorizontalScale(1.0f);
        fileNameBox.setText(fileNameBoxPlaceHolder, juce::dontSendNotification);
        fileNameBox.setLookAndFeel(&lookAndFeel);

        addAndMakeVisible(leftArea);

        addAndMakeVisible(loadArea);
        addAndMakeVisible(fileArea);

        addAndMakeVisible(rightArea);
        addAndMakeVisible(btnArea);

        leftArea.addAndMakeVisible(titleLabel);

        loadArea.addAndMakeVisible(loadBtn);
        fileArea.addAndMakeVisible(fileNameBox);

        rightArea.addAndMakeVisible(btnArea);
        btnArea.addAndMakeVisible(playBtn);
        btnArea.addAndMakeVisible(pauseBtn);
    }

    TitlePanel::~TitlePanel() { fileNameBox.setLookAndFeel(nullptr); }

    void TitlePanel::pauseButtonClicked()
    {
        juce::RangedAudioParameter* playParameter = uic.apvts.getParameter(global::play::id);
        playParameter->setValueNotifyingHost(0.f);
        // TODO : simulate the end of a midi noteOn() C3 and proceed to the release
        // also reset the grain pool state and the grain visual buffer at the complete stop
    }

    void TitlePanel::playButtonClicked()
    {
        juce::RangedAudioParameter* playParameter = uic.apvts.getParameter(global::play::id);
        playParameter->setValueNotifyingHost(1.f);
        // TODO : simulate the start of a midi noteOn() C3
        // uic.facade.play();
    }

    void TitlePanel::setPlayButtonImage()
    {
        playBtn.setImages(juce::Drawable::createFromImageData(BinaryData::Play_svg, BinaryData::Play_svgSize).get(),
            juce::Drawable::createFromImageData(BinaryData::Play_Fill_svg, BinaryData::Play_Fill_svgSize).get(), nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr);
    }

    void TitlePanel::setPauseButtonImage()
    {
        pauseBtn.setImages(juce::Drawable::createFromImageData(BinaryData::Pause_svg, BinaryData::Pause_svgSize).get(),
            juce::Drawable::createFromImageData(BinaryData::Pause_Fill_svg, BinaryData::Pause_Fill_svgSize).get(), nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr);
    }

    void TitlePanel::loadSampleButtonClicked()
    {
        // trigger the midi onNoteOff()
        //paramsView.setIsPlaying(false);

        juce::RangedAudioParameter* playParameter = uic.apvts.getParameter(global::play::id);
        playParameter->setValueNotifyingHost(0.f);
        uic.facade.loadFile();
    }

    void TitlePanel::paint(juce::Graphics& g)
    {
        const juce::Rectangle<float> inner = getLocalBounds().reduced(2).toFloat();
        g.setColour(colours::panel::audioFilePanel);
        g.fillRoundedRectangle(inner, 12.0f);

        const float lineThickness = 2.0f;

        g.setColour(colours::panel::contourPanel);
        g.drawRoundedRectangle(inner, 12.0f, lineThickness);
    }

    void TitlePanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.alignItems = juce::FlexBox::AlignItems::stretch;

        fb.items.add(juce::FlexItem(leftArea).withFlex(1.0f));
        fb.items.add(juce::FlexItem(loadArea).withFlex(1.0f));
        fb.items.add(juce::FlexItem(fileArea).withFlex(1.0f));
        fb.items.add(juce::FlexItem(rightArea).withFlex(1.0f));

        fb.performLayout(area);

        layoutLeft();
        layoutLoad();
        layoutFile();
        layoutRight();

        fileNameBox.repaint();
    }

    void TitlePanel::layoutLeft()
    {
        juce::Rectangle<int> area = leftArea.getLocalBounds().reduced(4, 0);

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.justifyContent = juce::FlexBox::JustifyContent::center;
        fb.alignItems = juce::FlexBox::AlignItems::center;

        fb.items.add(juce::FlexItem(titleLabel).withFlex(0.0f).withHeight(20.f).withWidth(100.f));
        fb.performLayout(area);
    }

    void TitlePanel::layoutLoad()
    {
        juce::Rectangle<int> area = loadArea.getLocalBounds().reduced(4, 0);

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
        fb.alignItems = juce::FlexBox::AlignItems::center;

        fb.items.add(juce::FlexItem(loadBtn).withWidth(90.f).withHeight(25.f));
        fb.performLayout(area);
    }

    void TitlePanel::layoutFile()
    {
        juce::Rectangle<int> area = fileArea.getLocalBounds().reduced(4, 0);

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
        fb.alignItems = juce::FlexBox::AlignItems::center;

        fb.items.add(juce::FlexItem(fileNameBox).withFlex(1.0f).withHeight(25.f));
        fb.performLayout(area);

        fileNameBox.repaint();
    }

    void TitlePanel::layoutRight()
    {
        juce::Rectangle<int> area = rightArea.getLocalBounds();

        juce::FlexBox fb;
        fb.flexDirection = juce::FlexBox::Direction::row;
        fb.alignItems = juce::FlexBox::AlignItems::stretch;

        fb.items.add(juce::FlexItem().withFlex(1.0f));
        fb.items.add(juce::FlexItem(btnArea).withFlex(1.0f));

        fb.performLayout(area);

        // Centre les boutons dans btnArea
        juce::FlexBox btnFb;
        btnFb.flexDirection = juce::FlexBox::Direction::row;
        btnFb.justifyContent = juce::FlexBox::JustifyContent::center;
        btnFb.alignItems = juce::FlexBox::AlignItems::center;

        const float btnSize = 25.0f;
        const float gap = 8.0f;

        btnFb.items.add(juce::FlexItem(playBtn).withWidth(btnSize).withHeight(btnSize).withMargin({0, gap * 0.5f, 0, 0}));
        btnFb.items.add(juce::FlexItem(pauseBtn).withWidth(btnSize).withHeight(btnSize).withMargin({0, 0, 0, gap * 0.5f}));

        btnFb.performLayout(btnArea.getLocalBounds());
    }
}