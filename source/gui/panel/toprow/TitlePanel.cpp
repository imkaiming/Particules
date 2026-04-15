#include "TitlePanel.h"

#include <juce_graphics/juce_graphics.h>

#include "../../../framework/PluginParams.h"
#include "../../../framework/bridge/EngineState.h"
#include "../../../framework/bridge/UIState.h"
#include "../../../utils/UIHelpers.h"
#include "../../../utils/struct/ProcessorFacade.h"
#include "../../../utils/struct/UIContext.h"
#include "../../lookandfeelv2/Colours.h"

#include "BinaryData.h"

namespace particules
{

    TitlePanel::TitlePanel(UIContext& uic)
        : playBtn{(const str) "playBtn"}, loadBtn{(const str) "loadBtn"}, uic{uic}, facade{uic.facade},
          lastPlayState{false}, engineState{uic.engineState}, nextBtn{(const str) "nextBtn"},
          previousBtn{(const str) "previousBtn"}
    {
        uic.uiState.addChangeListener(this);

        playIcon = UIHelpers::loadSVG(BinaryData::play_svg, BinaryData::play_svgSize, juce::Colours::white);
        pauseIcon = UIHelpers::loadSVG(BinaryData::pause_svg, BinaryData::pause_svgSize, juce::Colours::white);
        loadIcon = UIHelpers::loadSVG(BinaryData::add_folder_svg, BinaryData::add_folder_svgSize, juce::Colours::white);
        previousIcon = UIHelpers::loadSVG(
            BinaryData::arrow_turn_backward_svg, BinaryData::arrow_turn_backward_svgSize, juce::Colours::white);
        nextIcon =
            UIHelpers::loadSVG(BinaryData::arrow_turn_forward_svg, BinaryData::arrow_turn_forward_svgSize, juce::Colours::white);

        lastPlayState = uic.facade.isPlaying();
        playBtn.setIcon(lastPlayState ? pauseIcon.get() : playIcon.get());
        playBtn.setEnabled(uic.uiState.isFileLoaded());

        //playBtn.setIcon(playIcon.get());
        loadBtn.setIcon(loadIcon.get());
        previousBtn.setIcon(previousIcon.get());
        nextBtn.setIcon(nextIcon.get());

        playBtn.onClick = [this]() { playButtonClicked(); };
        loadBtn.onClick = [this]() { loadSampleButtonClicked(); };
        previousBtn.onClick = [this]() { /* undomanager . back */ };
        nextBtn.onClick = [this]() { /* undomanager . next */ };

        titleLabel.setText("PARTICULES", juce::dontSendNotification);
        titleLabel.getProperties().set("isTitle", true);
        titleLabel.setJustificationType(juce::Justification::centredLeft);
        titleLabel.setColour(juce::Label::textColourId, coloursv2::deepBlack);

        addAndMakeVisible(playBtn);
        addAndMakeVisible(loadBtn);
        addAndMakeVisible(previousBtn);
        addAndMakeVisible(nextBtn);
        addAndMakeVisible(titleLabel);

        //playBtn.setEnabled(false);
    }

    TitlePanel::~TitlePanel() {}

    void TitlePanel::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if(source == &uic.uiState)
        {
            // Active ou désactive le bouton play selon l'état du fichier en mémoire
            playBtn.setEnabled(uic.uiState.isFileLoaded());
        }
    }

    void TitlePanel::playButtonClicked()
    {
        if(!facade.setPlaying)
            return;

        // On inverse l'état actuel mémorisé
        //const bool newState = !lastPlayState;
        //facade.setPlaying(newState);
        const bool currentState = facade.isPlaying() > 0.5f;
        facade.setPlaying(!currentState);
        playBtn.setIcon(currentState ? playIcon.get() : pauseIcon.get());

        /*
        if(facade.isPlaying())
        {
            //facade.stop();
            playBtn.setIcon(playIcon.get());
            facade.setPlaying(false);

        }
        else
        {
            //facade.play();
            playBtn.setIcon(pauseIcon.get());
            facade.setPlaying(true);
            // TODO change to engineState.setAuditionning(true) after midi compatibility
        }
        */
    }

    void TitlePanel::loadSampleButtonClicked()
    {
        //facade.stop();
        facade.setPlaying(false);
        if(facade.loadFile)
            facade.loadFile();
    }

    void TitlePanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::perleBlanc.darker(0.1f)); }

    void TitlePanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();
        int margin = 15;
        area.reduce(margin, 0);

        int btnSize = juce::jmin(getHeight() - 16, 32);
        int titleWidth = 150;

        juce::Rectangle<int> titleArea = area.removeFromLeft(titleWidth);
        titleLabel.setBounds(titleArea);

        juce::Rectangle<int> loadArea = area.removeFromRight(btnSize);
        loadBtn.setBounds(loadArea.withSizeKeepingCentre(btnSize, btnSize));

        int transportSpacing = 15;
        int transportWidth = (btnSize * 2) + btnSize + (transportSpacing * 2);
        juce::Rectangle<int> transportArea = getLocalBounds().withSizeKeepingCentre(transportWidth, getHeight());

        juce::Rectangle<int> prevArea = transportArea.removeFromLeft(btnSize);
        transportArea.removeFromLeft(transportSpacing);

        juce::Rectangle<int> playArea = transportArea.removeFromLeft(btnSize);
        transportArea.removeFromLeft(transportSpacing);

        juce::Rectangle<int> nextArea = transportArea.removeFromLeft(btnSize);

        previousBtn.setBounds(prevArea.withSizeKeepingCentre(btnSize, btnSize));
        playBtn.setBounds(playArea.withSizeKeepingCentre(btnSize, btnSize));
        nextBtn.setBounds(nextArea.withSizeKeepingCentre(btnSize, btnSize));
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