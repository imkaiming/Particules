#include "ThumbnailComponent.h"

#include <juce_graphics/juce_graphics.h> // graphics
#include <juce_audio_utils/juce_audio_utils.h> // audiothumbnail

#include "../../framework/bridge/UIState.h"
#include "../lookandfeel/MyColours.h"
#include "../../utils/struct/UIContext.h"
#include "../../framework/bridge/EngineState.h"

namespace particules
{
    ThumbnailComponent::ThumbnailComponent(UIContext& uic)
        : audioThumbnail{uiState.getAudioThumbnail()}, grainVisualComponent(uic), uiState{uic.uiState},
          engineState{uic.engineState}
    {
        audioThumbnail.addChangeListener(this);

        addAndMakeVisible(&grainVisualComponent);

        if(audioThumbnail.isFullyLoaded())
            repaint();

        setOpaque(false);
        grainVisualComponent.toBack();

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
        g.setColour(colours::cream);
        g.drawFittedText("No File Loaded", getLocalBounds(), juce::Justification::centred, 1);
    }

    void ThumbnailComponent::paintIfFileLoaded(juce::Graphics& g)
    {
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
        // vertical lines
        g.setColour(juce::Colours::white.withAlpha(0.06f));

        for(int i = 1; i < numColumns; ++i)
        {
            const float x = w * static_cast<float>(i) / static_cast<float>(numColumns);
            g.drawVerticalLine(static_cast<int>(x), 0.0f, h);
        }

        // horizontal lines
        const int numRows = 4;
        for(int i = 1; i < numRows; ++i)
        {
            const float y = h * static_cast<float>(i) / static_cast<float>(numRows);

            const float alpha = (i == numRows / 2) ? 0.12f : 0.06f;
            g.setColour(juce::Colours::white.withAlpha(alpha));
            g.drawHorizontalLine(static_cast<int>(y), 0.0f, w);
        }
    }

    void ThumbnailComponent::resized() { grainVisualComponent.setBounds(getLocalBounds()); }

    void ThumbnailComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
    {
        if(source == &audioThumbnail)
        {
            grainVisualComponent.setNumSamples(engineState.getNumSamples());
            repaint();
            if(audioThumbnail.isFullyLoaded())
            {
                //juce::Logger::outputDebugString("FINI DE PEINTURER CALLBACK");
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
