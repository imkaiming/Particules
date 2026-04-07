#include "TitlePanel.h"

#include <juce_graphics/juce_graphics.h>

#include "../../framework/PluginParams.h"
#include "../../framework/bridge/EngineState.h"
#include "../../utils/UIHelpers.h"
#include "../../utils/struct/ProcessorFacade.h"
#include "../../utils/struct/UIContext.h"
#include "../lookandfeelv2/Colours.h"

#include "BinaryData.h"

namespace particules
{

    TitlePanel::TitlePanel(UIContext& uic)
        : playBtn{(const str) "playBtn"}, uic{uic}, facade{uic.facade}, engineState{uic.engineState}
    {
        playIcon = UIHelpers::loadSVG(BinaryData::play_svg, BinaryData::play_svgSize, juce::Colours::white);
        pauseIcon = UIHelpers::loadSVG(BinaryData::pause_svg, BinaryData::pause_svgSize, juce::Colours::white);

        playBtn.setIcon(playIcon.get());
        addAndMakeVisible(playBtn);

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
    }

    TitlePanel::~TitlePanel() { fileNameBox.setLookAndFeel(nullptr); }

    void TitlePanel::playButtonClicked()
    {
        if(engineState.getIsPlaying())
        {
            //facade.stop();
            playBtn.setIcon(playIcon.get());
            engineState.setIsPlaing(false);
        }
        else
        {
            //facade.play();
            playBtn.setIcon(pauseIcon.get());
            engineState.setIsPlaing(true);
        }
    }

    void TitlePanel::loadSampleButtonClicked()
    {
        facade.stop();
        facade.loadFile();
    }

    void TitlePanel::paint(juce::Graphics& g)
    {
        /*
        const juce::Rectangle<float> inner = getLocalBounds().reduced(2).toFloat();
        g.setColour(colours::panel::audioFilePanel);
        g.fillRoundedRectangle(inner, 12.0f);

        const float lineThickness = 2.0f;

        g.setColour(colours::panel::contourPanel);
        g.drawRoundedRectangle(inner, 12.0f, lineThickness);
        */
        g.fillAll(coloursv2::deepBlack.brighter(0.04f));
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
        //btnFb.items.add(juce::FlexItem(pauseBtn).withWidth(btnSize).withHeight(btnSize).withMargin({0, 0, 0, gap * 0.5f}));

        btnFb.performLayout(btnArea.getLocalBounds());
    }
}