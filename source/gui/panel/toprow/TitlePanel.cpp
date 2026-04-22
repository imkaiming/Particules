#include "TitlePanel.h"

#include <juce_graphics/juce_graphics.h>

#include "framework/bridge/FromUI.h"
#include "framework/core/PluginParams.h"
#include "framework/state/AudioState.h"
#include "framework/state/UIState.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/UIHelpers.h"
#include "utils/struct/UIContext.h"

#include "BinaryData.h"

#ifndef PARTICULES_BUILD_TYPE
#define PARTICULES_BUILD_TYPE ""
#endif

#ifndef PARTICULES_VERSION
#define PARTICULES_VERSION ""
#endif

namespace particules
{

    TitlePanel::TitlePanel(UIContext& uic)
        : playBtn{(const str) "playBtn"}, loadBtn{(const str) "loadBtn"}, uiState{uic.uiState}, fui{uic.fui},
          lastPlayState{false}, audioState{uic.audioState}, nextBtn{(const str) "nextBtn"}, previousBtn{(const str) "previousBtn"}
    {
        uiState.addChangeListener(this);

        playIcon = UIHelpers::loadSVG(BinaryData::play_svg, BinaryData::play_svgSize, juce::Colours::white);
        pauseIcon = UIHelpers::loadSVG(BinaryData::pause_svg, BinaryData::pause_svgSize, juce::Colours::white);
        loadIcon = UIHelpers::loadSVG(BinaryData::add_folder_svg, BinaryData::add_folder_svgSize, juce::Colours::white);
        previousIcon = UIHelpers::loadSVG(
            BinaryData::arrow_turn_backward_svg, BinaryData::arrow_turn_backward_svgSize, juce::Colours::white);
        nextIcon =
            UIHelpers::loadSVG(BinaryData::arrow_turn_forward_svg, BinaryData::arrow_turn_forward_svgSize, juce::Colours::white);

        lastPlayState = fui.isPlaying();
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

        juce::String versionText = str("v") + PARTICULES_VERSION;
        versionLabel.setText(versionText, juce::dontSendNotification);
        versionLabel.getProperties().set("isVersion", true);
        versionLabel.setJustificationType(juce::Justification::centredLeft);
        versionLabel.setColour(juce::Label::textColourId, coloursv2::deepBlack.withAlpha(0.5f));

        addAndMakeVisible(&playBtn);
        addAndMakeVisible(&loadBtn);
        addAndMakeVisible(&previousBtn);
        addAndMakeVisible(&nextBtn);
        addAndMakeVisible(&titleLabel);
        addAndMakeVisible(&versionLabel);
    }

    TitlePanel::~TitlePanel() {}

    void TitlePanel::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if(source == &uiState)
        {
            // Active ou désactive le bouton play selon l'état du fichier en mémoire
            playBtn.setEnabled(uiState.isFileLoaded());
        }
    }

    void TitlePanel::playButtonClicked()
    {
        // On inverse l'état actuel mémorisé
        //const bool newState = !lastPlayState;
        //fui.setPlaying(newState);
        const bool currentState = fui.isPlaying() > 0.5f;
        fui.setPlaying(!currentState);
        playBtn.setIcon(currentState ? playIcon.get() : pauseIcon.get());
    }

    void TitlePanel::loadSampleButtonClicked()
    {
        //fui.stop();
        //fui.setPlaying(false);
        //if(fui.onLoadFile)
        fui.loadFile();
    }

    void TitlePanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::perleBlanc.darker(0.1f)); }

    void TitlePanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();
        int margin = 5;
        area.reduce(margin, 0);

        int btnSize = juce::jmin(getHeight() - margin, 35);
        int titleWidth = 150;

        juce::Rectangle<int> titleArea = area.removeFromLeft(titleWidth);

        titleLabel.setBounds(titleArea);
        const int textBottomY = titleArea.getCentreY() + 6;
        versionLabel.setBounds(titleArea.withTop(textBottomY).withHeight(14));

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
}