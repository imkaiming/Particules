#include "GrainsPanel.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include "framework/state/AudioState.h"
#include "framework/core/PluginParams.h"
#include "gui/lookandfeel/MyColours.h"
#include "gui/lookandfeelv2/Colours.h"
#include "utils/UIHelpers.h"
#include "utils/struct/UIContext.h"

#include "BinaryData.h"

namespace particules
{
    GrainsPanel::GrainsPanel(UIContext& uic)
        : audioState{uic.audioState}, linkBtn{"linkBtn"}, apvts{uic.apvts}, playback{nullptr}, playbackBtn{"playbackBtn"},
          emissionSlider{audioState, RotaryType::primaryWithAux, uic.apvts, params::emission::name, params::emission::id},
          durationSlider{audioState, RotaryType::primaryWithAux, uic.apvts, params::duration::name, params::duration::id},
          envelopeRotaryMenu{uic.apvts, params::envelopeMode::id, params::envelopeRatio::id, params::envelopeRatio_jitter::id},
          traversalRotaryMenu{uic.apvts, params::traversalMode::id, params::traversalFreq::id, params::traversalFreq_jitter::id},
          speedSlider{audioState, RotaryType::secondaryWithAux, uic.apvts, params::speed::name, params::speed::id},
          panSlider{audioState, RotaryType::secondaryWithAux, uic.apvts, params::pan::name, params::pan::id}, outputSlider{},
          isLinkingUpdate{false}

    {
        // slider linking feature
        linkInIcon = UIHelpers::loadSVG(BinaryData::link_in_svg, BinaryData::link_in_svgSize, juce::Colours::white);
        linkOffIcon = UIHelpers::loadSVG(BinaryData::link_off_svg, BinaryData::link_off_svgSize, juce::Colours::white);

        // playback feature
        forwardIcon =
            UIHelpers::loadSVG(BinaryData::playback_forward2_svg, BinaryData::playback_forward2_svgSize, juce::Colours::white);
        backwardIcon =
            UIHelpers::loadSVG(BinaryData::playback_backward2_svg, BinaryData::playback_backward2_svgSize, juce::Colours::white);

        linkBtn.setIcon(linkOffIcon.get());
        playbackBtn.setIcon(forwardIcon.get());

        linkBtn.onClick = [this]() { setLinkButtonImage(); };
        playbackBtn.onClick = [this]() { onPlaybackButtonClicked(); };

        linkBtn.internalPaddingCoef = 0.25f;
        playbackBtn.internalPaddingCoef = 0.10f;

        emissionSlider.setOnPrimaryValueChanged([this](double emissionVal) {
            if(!audioState.getIsLinked() || isLinkingUpdate)
                return;

            if(emissionVal <= 0.0)
                return;

            isLinkingUpdate = true;

            const double durationVal = 1.0 / emissionVal;

            if(juce::RangedAudioParameter* param = apvts.getParameter(params::duration::id))
                param->setValueNotifyingHost(param->convertTo0to1(static_cast<float>(durationVal)));

            isLinkingUpdate = false;
        });

        durationSlider.setOnPrimaryValueChanged([this](double durationVal) {
            if(!audioState.getIsLinked() || isLinkingUpdate)
                return;

            if(durationVal <= 0.0)
                return;

            isLinkingUpdate = true;

            const double emissionVal = 1.0 / durationVal;

            if(juce::RangedAudioParameter* param = apvts.getParameter(params::emission::id))
                param->setValueNotifyingHost(param->convertTo0to1(static_cast<float>(emissionVal)));

            isLinkingUpdate = false;
        });

        emissionSliderAttachment = emissionSlider.attachPrimaryToAPVTS(uic.apvts, params::emission::id);
        durationSliderAttachment = durationSlider.attachPrimaryToAPVTS(uic.apvts, params::duration::id);
        speedSliderAttachment = speedSlider.attachPrimaryToAPVTS(uic.apvts, params::speed::id);
        panSliderAttachment = panSlider.attachPrimaryToAPVTS(uic.apvts, params::pan::id);

        attackSliderAttachment = std::make_unique<ValueTreeState::SliderAttachment>(uic.apvts, params::attack::id, attackSlider);
        decaySliderAttachment = std::make_unique<ValueTreeState::SliderAttachment>(uic.apvts, params::decay::id, decaySlider);
        sustainSliderAttachment =
            std::make_unique<ValueTreeState::SliderAttachment>(uic.apvts, params::sustain::id, sustainSlider);
        releaseSliderAttachment =
            std::make_unique<ValueTreeState::SliderAttachment>(uic.apvts, params::release::id, releaseSlider);
        outputSliderAttachment = std::make_unique<ValueTreeState::SliderAttachment>(uic.apvts, params::output::id, outputSlider);

        attackSlider.setSliderStyle(juce::Slider::LinearBarVertical);
        decaySlider.setSliderStyle(juce::Slider::LinearBarVertical);
        sustainSlider.setSliderStyle(juce::Slider::LinearBarVertical);
        releaseSlider.setSliderStyle(juce::Slider::LinearBarVertical);

        attackSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        decaySlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        sustainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        releaseSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

        attackLabel.setText("A", juce::NotificationType::dontSendNotification);
        decayLabel.setText("D", juce::NotificationType::dontSendNotification);
        sustainLabel.setText("S", juce::NotificationType::dontSendNotification);
        releaseLabel.setText("R", juce::NotificationType::dontSendNotification);
        outputLabel.setText("output", juce::NotificationType::dontSendNotification);

        juce::Justification centerBot = juce::Justification::centredBottom;
        attackLabel.setJustificationType(centerBot);
        decayLabel.setJustificationType(centerBot);
        sustainLabel.setJustificationType(centerBot);
        releaseLabel.setJustificationType(centerBot);
        outputLabel.setJustificationType(centerBot);

        attackLabel.getProperties().set("isValue", true);
        decayLabel.getProperties().set("isValue", true);
        sustainLabel.getProperties().set("isValue", true);
        releaseLabel.getProperties().set("isValue", true);

        outputLabel.setJustificationType(juce::Justification::centredTop);
        outputLabel.setInterceptsMouseClicks(false, false);
        outputLabel.setColour(juce::Label::textColourId, coloursv2::white);
        outputLabel.getProperties().set("isName", true);

        addAndMakeVisible(&linkBtn);
        addAndMakeVisible(&emissionSlider);
        addAndMakeVisible(&durationSlider);
        addAndMakeVisible(&attackSlider);
        addAndMakeVisible(&decaySlider);
        addAndMakeVisible(&sustainSlider);
        addAndMakeVisible(&releaseSlider);
        addAndMakeVisible(&attackLabel);
        addAndMakeVisible(&decayLabel);
        addAndMakeVisible(&sustainLabel);
        addAndMakeVisible(&releaseLabel);
        addAndMakeVisible(&envelopeRotaryMenu);
        addAndMakeVisible(&traversalRotaryMenu);
        addAndMakeVisible(&panSlider);
        addAndMakeVisible(&speedSlider);
        addAndMakeVisible(&playbackBtn);
        addAndMakeVisible(&outputSlider);
        addAndMakeVisible(&outputLabel);
    }

    void GrainsPanel::onPlaybackButtonClicked()
    {
        playback = apvts.getRawParameterValue(params::playback::id);

        if(playback->load(std::memory_order_relaxed) > 0.5f)
        {
            playbackBtn.setIcon(forwardIcon.get());
            apvts.getParameter(params::playback::id)->setValueNotifyingHost(0.f);
        }
        else
        {
            playbackBtn.setIcon(backwardIcon.get());
            apvts.getParameter(params::playback::id)->setValueNotifyingHost(1.f);
        }
    }
    void GrainsPanel::setLinkButtonImage()
    {
        if(audioState.getIsLinked())
        {
            audioState.setLink(false);
            linkBtn.setIcon(linkOffIcon.get());
        }
        else
        {
            audioState.setLink(true);
            linkBtn.setIcon(linkInIcon.get());

            double emissionVal = emissionSlider.getPrimaryValue();
            if(emissionVal > 0.0)
            {
                isLinkingUpdate = true;
                const double durationVal = 1.0 / emissionVal;
                if(juce::RangedAudioParameter* param = apvts.getParameter(params::duration::id))
                    param->setValueNotifyingHost(param->convertTo0to1(static_cast<float>(durationVal)));
                isLinkingUpdate = false;
            }
        }
    }

    void GrainsPanel::paint(juce::Graphics& g) { g.fillAll(coloursv2::deepBlack); /* ou lightBlack*/ }

    void GrainsPanel::resized()
    {
        juce::Rectangle<int> area = getLocalBounds();
        const int margin = juce::jmax(5, area.getWidth() / 50);
        const int sliderMargin = margin / 2;
        area.reduced(margin);
        juce::Rectangle<int> leftCoreArea = area.removeFromLeft(static_cast<int>(area.getWidth() * 0.5f));
        area.removeFromLeft(margin);
        juce::Rectangle<int> rightArea = area;

        juce::Rectangle<int> topRow = leftCoreArea.removeFromTop(static_cast<int>(leftCoreArea.getHeight() * 0.5f));
        juce::Rectangle<int> botRow = leftCoreArea;

        int maxSliderHeight = topRow.getHeight();
        int linkBtnWidth = leftCoreArea.getWidth() / 12;
        int sliderWidth = leftCoreArea.getWidth() / 2;

        juce::Rectangle<int> emissionArea = topRow.removeFromLeft(sliderWidth);
        juce::Rectangle<int> durationArea = topRow;

        int botHalfWidth = botRow.getWidth() / 2;
        juce::Rectangle<int> envelopeArea = botRow.removeFromLeft(botHalfWidth);
        juce::Rectangle<int> traversalArea = botRow;

        juce::Rectangle<int> rightTopRow = rightArea.removeFromTop(static_cast<int>(rightArea.getHeight() * 0.5f));

        int motionKnobWidth = rightTopRow.getWidth() / 2;
        juce::Rectangle<int> speedArea = rightTopRow.removeFromLeft(motionKnobWidth);
        juce::Rectangle<int> panArea = rightTopRow;

        int tightestWidth = juce::jmin(emissionArea.getWidth(), envelopeArea.getWidth(), speedArea.getWidth());
        int globalSliderSize = juce::jmin(maxSliderHeight, tightestWidth);

        emissionSlider.setBounds(emissionArea.withSizeKeepingCentre(globalSliderSize, globalSliderSize).reduced(sliderMargin));
        durationSlider.setBounds(durationArea.withSizeKeepingCentre(globalSliderSize, globalSliderSize).reduced(sliderMargin));
        envelopeRotaryMenu.setBounds(
            envelopeArea.withSizeKeepingCentre(globalSliderSize, globalSliderSize).reduced(sliderMargin));
        traversalRotaryMenu.setBounds(
            traversalArea.withSizeKeepingCentre(globalSliderSize, globalSliderSize).reduced(sliderMargin));
        speedSlider.setBounds(speedArea.withSizeKeepingCentre(globalSliderSize, globalSliderSize).reduced(sliderMargin));
        panSlider.setBounds(panArea.withSizeKeepingCentre(globalSliderSize, globalSliderSize).reduced(sliderMargin));
        linkBtn.setBounds(emissionArea.getRight() - (linkBtnWidth / 2), emissionArea.getCentreY() - (linkBtnWidth / 2),
            linkBtnWidth, linkBtnWidth);

        // playback button
        {
            juce::Rectangle<int> speedSliderBounds = speedSlider.getBounds();

            const float minDim = juce::jmin((float)speedSliderBounds.getWidth(), (float)speedSliderBounds.getHeight());
            const float maxExtentFromCenter = minDim * 0.62f;
            const float auxSize = juce::jlimit(18.0f, 32.0f, minDim * 0.18f);

            const float jitterOuterMultiplier = 1.10f;
            const float auxGap = 4.0f;
            const float safetyPadding = 3.0f;
            const float jitterLineWidthHalf = 1.0f;

            const float maxAuxCenterDist = maxExtentFromCenter - safetyPadding - (auxSize * 0.5f);
            const float availableForJitter = maxAuxCenterDist - auxGap - (auxSize * 0.5f);
            const float primaryVisualRadius =
                juce::jmax(1.0f, (availableForJitter - jitterLineWidthHalf) / jitterOuterMultiplier);
            const float jitterOuterEdge = (primaryVisualRadius * jitterOuterMultiplier) + jitterLineWidthHalf;

            const float auxDistance = jitterOuterEdge + auxGap + (auxSize * 0.5f);

            // 10 o'clock
            const float anglePlayback = -pi + (pi / 5.0f);

            // Apply the distance relative to the slider's center within the GrainsPanel coordinate space
            const float px = speedSliderBounds.getCentreX() + std::cos(anglePlayback) * auxDistance;
            const float py = speedSliderBounds.getCentreY() + std::sin(anglePlayback) * auxDistance;

            playbackBtn.setBounds(juce::Rectangle<float>(auxSize, auxSize).withCentre(juce::Point<float>(px, py)).toNearestInt());
        }
        // ADSR
        {
            juce::Rectangle<int> adsrArea = rightArea.removeFromLeft(rightArea.getWidth() / 2);
            adsrArea = adsrArea.withSizeKeepingCentre(adsrArea.getWidth(), adsrArea.getHeight());

            const int adsrHeight = adsrArea.getHeight() / 8;
            const int adsrWidth = adsrArea.getWidth() / 4;
            juce::Rectangle<int> labelsArea = adsrArea.removeFromTop(adsrHeight);
            adsrArea.removeFromBottom(adsrHeight);
            adsrArea.removeFromLeft(adsrWidth);
            adsrArea.removeFromRight(adsrWidth);
            labelsArea.removeFromLeft(adsrWidth);
            labelsArea.removeFromRight(adsrWidth);

            int numSliders = 4;
            int slotWidth = adsrArea.getWidth() / numSliders;

            juce::Rectangle<int> attackSlot = adsrArea.removeFromLeft(slotWidth);
            juce::Rectangle<int> decaySlot = adsrArea.removeFromLeft(slotWidth);
            juce::Rectangle<int> sustainSlot = adsrArea.removeFromLeft(slotWidth);
            juce::Rectangle<int> releaseSlot = adsrArea.removeFromLeft(slotWidth);

            attackSlider.setBounds(attackSlot);
            decaySlider.setBounds(decaySlot);
            sustainSlider.setBounds(sustainSlot);
            releaseSlider.setBounds(releaseSlot);

            juce::Rectangle<int> attackLblSlot = labelsArea.removeFromLeft(slotWidth);
            juce::Rectangle<int> decayLblSlot = labelsArea.removeFromLeft(slotWidth);
            juce::Rectangle<int> sustainLblSlot = labelsArea.removeFromLeft(slotWidth);
            juce::Rectangle<int> releaseLblSlot = labelsArea.removeFromLeft(slotWidth);

            attackLabel.setBounds(attackLblSlot);
            decayLabel.setBounds(decayLblSlot);
            sustainLabel.setBounds(sustainLblSlot);
            releaseLabel.setBounds(releaseLblSlot);
        }

        // output
        {
            juce::Rectangle<int> outputArea = rightArea;
            juce::Rectangle<int> outputCompArea =
                outputArea.withSizeKeepingCentre(globalSliderSize, globalSliderSize).reduced(sliderMargin);

            // same logic than main slider with aux
            const float minDim = juce::jmin(outputCompArea.getWidth(), outputCompArea.getHeight());
            const float maxExtentFromCenter = minDim * 0.62f;
            const float auxSize = juce::jlimit(18.0f, 32.0f, minDim * 0.18f);
            const float auxGap = 4.0f;
            const float safetyPadding = 3.0f;
            const float jitterLineWidthHalf = 1.0f;
            const float jitterOuterMultiplier = 1.10f;

            const float maxAuxCenterDist = maxExtentFromCenter - safetyPadding - (auxSize * 0.5f);
            const float availableForJitter = maxAuxCenterDist - auxGap - (auxSize * 0.5f);
            const float primaryVisualRadius =
                juce::jmax(1.0f, (availableForJitter - jitterLineWidthHalf) / jitterOuterMultiplier);
            const float jitterOuterEdge = (primaryVisualRadius * jitterOuterMultiplier) + jitterLineWidthHalf;

            const int primaryBoundsSize = static_cast<int>((jitterOuterEdge + 1.0f) * 2.0f);

            outputSlider.setBounds(outputCompArea.withSizeKeepingCentre(primaryBoundsSize, primaryBoundsSize));

            // output label
            const int labelHeight = static_cast<int>((outputCompArea.getHeight() - jitterOuterEdge) * 0.4f - safetyPadding);
            outputLabel.setBounds(
                outputCompArea.getX(), outputCompArea.getBottom() - labelHeight, outputCompArea.getWidth(), labelHeight);
        }
    }
}