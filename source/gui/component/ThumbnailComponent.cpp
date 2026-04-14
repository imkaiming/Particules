#include "ThumbnailComponent.h"

#include <juce_audio_utils/juce_audio_utils.h> // audiothumbnail
#include <juce_graphics/juce_graphics.h> // graphics

#include "../../framework/bridge/EngineState.h"
#include "../../framework/bridge/UIState.h"
#include "../../utils/struct/UIContext.h"
#include "../lookandfeel/MyColours.h"
#include "../lookandfeelv2/Colours.h"
#include "../lookandfeelv2/MainLNF.h"

namespace particules
{
    ThumbnailComponent::ThumbnailComponent(UIContext& uic)
        : audioThumbnail{uic.uiState.getAudioThumbnail()}/*, uiState{uic.uiState}*/
    {
        audioThumbnail.addChangeListener(this);

        setOpaque(true);
        //addAndMakeVisible(&grainVisualComponent);
        //grainVisualComponent.toBack();

        if(audioThumbnail.isFullyLoaded())
            repaint();


        noFileLabel.setText("No File Loaded", juce::dontSendNotification);
        noFileLabel.setJustificationType(juce::Justification::centred);
        noFileLabel.setColour(juce::Label::textColourId, coloursv2::perleBlanc);
        noFileLabel.getProperties().set("isValue", true);
        addAndMakeVisible(noFileLabel);

        // apvts listener to update the UI
        //apvts.addParameterListener(Param::Position::id, this);
        //apvts.addParameterListener(Param::Selection::id, this);
    }

    ThumbnailComponent::~ThumbnailComponent()
    {
        audioThumbnail.removeChangeListener(this);
        //apvts.removeParameterListener(Param::Position::id, this);
        //apvts.removeParameterListener(Param::Selection::id, this);
    }

    void ThumbnailComponent::paint(juce::Graphics& g)
    {
        if(audioThumbnail.getNumChannels() == 0)
            paintIfNoFileLoaded(g);
        else
            paintIfFileLoaded(g);
    }

    void ThumbnailComponent::paintIfNoFileLoaded(juce::Graphics& g)
    {
        g.fillAll(colours::black);
        paintGrid(g);
        noFileLabel.setVisible(true);
    }

    void ThumbnailComponent::paintIfFileLoaded(juce::Graphics& g)
    {
        noFileLabel.setVisible(false);
        g.fillAll(colours::smokyBlack);
        paintGrid(g);
        g.setColour(colours::brightBlue);
        audioThumbnail.drawChannels(g, getLocalBounds(), 0.0, audioThumbnail.getTotalLength(), 1.0f);
    }

    void ThumbnailComponent::paintGrid(juce::Graphics& g)
    {
        const juce::Rectangle<int> area = getLocalBounds();
        const float w = static_cast<float>(area.getWidth());
        const float h = static_cast<float>(area.getHeight());

        const int numColumns = 10;

        g.setColour(juce::Colours::white.withAlpha(0.05f));

        for(int i = 1; i < numColumns; ++i)
        {
            const float x = w * static_cast<float>(i) / static_cast<float>(numColumns);
            g.drawVerticalLine(static_cast<int>(x), 0.0f, h);
        }
        const int numRows = 4;
        for(int i = 1; i < numRows; ++i)
        {
            const float y = h * static_cast<float>(i) / static_cast<float>(numRows);

            const float alpha = (i == numRows / 2) ? 0.10f : 0.05f;
            g.setColour(juce::Colours::white.withAlpha(alpha));
            g.drawHorizontalLine(static_cast<int>(y), 0.0f, w);
        }
    }



    void ThumbnailComponent::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();
        //grainVisualComponent.setBounds(area);
        noFileLabel.setBounds(area);
    }

    void ThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if(source == &audioThumbnail)
        {
            //grainVisualComponent.setNumSamples(engineState.getNumSamples());
            repaint();
            if(audioThumbnail.isFullyLoaded())
            {
                // TODO INSERT AUDIO FILE FRAME CALLBACK IF YOU WANT
                // TO ENABLE PLAY BTN ONLY AFTER THE WAVEFORM IS SET

                if(onThumbnailReady)
                    onThumbnailReady();
            }
        }
    }

    void ThumbnailComponent::setCallbackOnThumbnailReady(std::function<void()> foo) { onThumbnailReady = std::move(foo); }

}

//void ThumbnailComponent::parameterChanged(const juce::String& parameterID, float newValue)
//{
//    if(parameterID == Param::Position::id)
//    {
//        //DBG("parameter view position new value : " + juce::String(paramsView.getFilePosition()));
//        updatePosition(newValue);
//        return;
//    }
//    if(parameterID == Param::Selection::id)
//    {
//        //DBG("parameter view selection new value : " + juce::String(paramsView.getWindowSelection()));
//        updateSelection(newValue);
//        return;
//    }
//}
