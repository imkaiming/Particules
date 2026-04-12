#include "TitlePanel.h"

#include <juce_graphics/juce_graphics.h>

#include "../../../framework/PluginParams.h"
#include "../../../framework/bridge/EngineState.h"
#include "../../../utils/UIHelpers.h"
#include "../../../utils/struct/ProcessorFacade.h"
#include "../../../utils/struct/UIContext.h"
#include "../../lookandfeelv2/Colours.h"

#include "BinaryData.h"

namespace particules
{

    TitlePanel::TitlePanel(UIContext& uic)
        : playBtn{(const str) "playBtn"}, loadBtn{(const str) "loadBtn"}, uic{uic}, facade{uic.facade},
          engineState{uic.engineState}
    {
        playIcon = UIHelpers::loadSVG(BinaryData::play_svg, BinaryData::play_svgSize, juce::Colours::white);
        pauseIcon = UIHelpers::loadSVG(BinaryData::pause_svg, BinaryData::pause_svgSize, juce::Colours::white);
        loadIcon = UIHelpers::loadSVG(BinaryData::add_folder_svg, BinaryData::add_folder_svgSize, juce::Colours::white);

        playBtn.setIcon(playIcon.get());
        loadBtn.setIcon(loadIcon.get());

        playBtn.onClick = [this]() { playButtonClicked(); };
        loadBtn.onClick = [this]() { loadSampleButtonClicked(); };

        titleLabel.setText("PARTICULES", juce::dontSendNotification);
        titleLabel.setJustificationType(juce::Justification::centred);
        titleLabel.setColour(juce::Label::textColourId, coloursv2::perleBlanc);
        titleLabel.setFont(juce::Font(18.0f));
        titleLabel.setSize(100, static_cast<int>(titleLabel.getFont().getHeight()));

        addAndMakeVisible(playBtn);
        addAndMakeVisible(loadBtn);
        addAndMakeVisible(titleLabel);

        //loadBtn.setButtonText("Load Sample");
        //loadBtn.setEnabled(true);
        //loadBtn.setColour(juce::Label::textColourId, juce::Colours::white);

        //fileNameBox.setJustificationType(juce::Justification::centred);
        //fileNameBox.setColour(juce::Label::textColourId, juce::Colours::white);
        //fileNameBox.setMinimumHorizontalScale(1.0f);
        //fileNameBox.setText(fileNameBoxPlaceHolder, juce::dontSendNotification);
        //fileNameBox.setLookAndFeel(&lookAndFeel);

    }

    TitlePanel::~TitlePanel() {}

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

    void TitlePanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::deepBlack.brighter(0.1f)); }

    void TitlePanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();
        const int quarter = getWidth() / 8.f;


        juce::Rectangle<int> titleArea= area.removeFromLeft(quarter);
        area.removeFromLeft(quarter * 5.f);
        juce::Rectangle<int> loadArea = area.removeFromLeft(quarter);
        juce::Rectangle<int> playArea = area.removeFromLeft(quarter);

        const int btnHeight = getHeight() * 0.666f;
        titleLabel.setBounds(titleArea);
        loadBtn.setBounds(loadArea.withSizeKeepingCentre(btnHeight, btnHeight));
        playBtn.setBounds(playArea.withSizeKeepingCentre(btnHeight, btnHeight));


        //juce::FlexBox fb;
        //fb.flexDirection = juce::FlexBox::Direction::row;
        //fb.alignItems = juce::FlexBox::AlignItems::stretch;

        //fb.items.add(juce::FlexItem(titleLabel).withFlex(1.0f));
        //fb.items.add(juce::FlexItem(loadBtn).withFlex(1.0f));
        //fb.items.add(juce::FlexItem(playBtn).withFlex(1.0f));

        //fb.performLayout(area);

        //fileNameBox.repaint();
    }

    //void TitlePanel::layoutLeft()
    //{
    //    juce::Rectangle<int> area = leftArea.getLocalBounds().reduced(4, 0);

    //    juce::FlexBox fb;
    //    fb.flexDirection = juce::FlexBox::Direction::row;
    //    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    //    fb.alignItems = juce::FlexBox::AlignItems::center;

    //    fb.items.add(juce::FlexItem(titleLabel).withFlex(1.0f).withHeight(20.f));
    //    fb.performLayout(area);
    //}

    //void TitlePanel::layoutLoad()
    //{
    //    juce::Rectangle<int> area = loadArea.getLocalBounds().reduced(4, 0);

    //    juce::FlexBox fb;
    //    fb.flexDirection = juce::FlexBox::Direction::row;
    //    fb.justifyContent = juce::FlexBox::JustifyContent::flexEnd;
    //    fb.alignItems = juce::FlexBox::AlignItems::center;

    //    fb.items.add(juce::FlexItem(loadBtn).withWidth(90.f).withHeight(25.f));
    //    fb.performLayout(area);
    //}

    //void TitlePanel::layoutFile()
    //{
    //    juce::Rectangle<int> area = fileArea.getLocalBounds().reduced(4, 0);

    //    juce::FlexBox fb;
    //    fb.flexDirection = juce::FlexBox::Direction::row;
    //    fb.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    //    fb.alignItems = juce::FlexBox::AlignItems::center;

    //    //fb.items.add(juce::FlexItem(fileNameBox).withFlex(1.0f).withHeight(25.f));
    //    fb.performLayout(area);

    //    //fileNameBox.repaint();
    //}

    //void TitlePanel::layoutRight()
    //{
    //    juce::Rectangle<int> area = rightArea.getLocalBounds();

    //    juce::FlexBox fb;
    //    fb.flexDirection = juce::FlexBox::Direction::row;
    //    fb.alignItems = juce::FlexBox::AlignItems::stretch;

    //    fb.items.add(juce::FlexItem().withFlex(1.0f));
    //    fb.items.add(juce::FlexItem(btnArea).withFlex(1.0f));

    //    fb.performLayout(area);

    //    // Centre les boutons dans btnArea
    //    juce::FlexBox btnFb;
    //    btnFb.flexDirection = juce::FlexBox::Direction::row;
    //    btnFb.justifyContent = juce::FlexBox::JustifyContent::center;
    //    btnFb.alignItems = juce::FlexBox::AlignItems::center;

    //    const float btnSize = 25.0f;
    //    const float gap = 8.0f;

    //    btnFb.items.add(juce::FlexItem(playBtn).withWidth(btnSize).withHeight(btnSize).withMargin({0, gap * 0.5f, 0, 0}));
    //    //btnFb.items.add(juce::FlexItem(pauseBtn).withWidth(btnSize).withHeight(btnSize).withMargin({0, 0, 0, gap * 0.5f}));

    //    btnFb.performLayout(btnArea.getLocalBounds());
    //}
}