#include "GrainsPanel.h"

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_graphics/juce_graphics.h>

#include "../../../../framework/bridge/EngineState.h"
#include "../../../../utils/UIHelpers.h"
#include "../../../../utils/struct/UIContext.h"
#include "../../../lookandfeel/MyColours.h"
#include "../../../lookandfeelv2/Colours.h"
#include "BinaryData.h"

namespace particules
{
    GrainsPanel::GrainsPanel(UIContext& uic)
        : engineState{uic.engineState}, linkBtn{"linkBtn"},
          emissionSlider{engineState, RotaryType::primaryWithAux, uic.apvts, params::emission::name, params::emission::id},
          durationSlider{engineState, RotaryType::primaryWithAux, uic.apvts, params::duration::name, params::duration::id},
          envelopeRotaryMenu{
              uic.apvts, params::envelopeMode::id, params::envelopeRatio::id, params::envelopeRatio_jitter::id},
          traversalRotaryMenu{uic.apvts, params::traversalMode::id, params::traversalFreq::id, params::traversalFreq_jitter::id},
          speedSlider{engineState, RotaryType::secondaryWithAux, uic.apvts, params::speed::name, params::speed::id},
          panSlider{engineState, RotaryType::secondaryWithAux, uic.apvts, params::pan::name, params::pan::id}

    {
        // slider linking feature
        linkInIcon = UIHelpers::loadSVG(BinaryData::link_in_svg, BinaryData::link_in_svgSize, juce::Colours::white);
        linkOffIcon = UIHelpers::loadSVG(BinaryData::link_off_svg, BinaryData::link_off_svgSize, juce::Colours::white);

        linkBtn.setIcon(linkOffIcon.get());

        linkBtn.onClick = [this]() { linkButtonClicked(); };

        emissionSlider.setOnPrimaryValueChanged([this](double emissionVal) {
            if(!engineState.getIsLinked())
                return;

            double durationVal = 1.0 / emissionVal;
            durationVal = juce::jlimit(durationSlider.getPrimaryMinimum(), durationSlider.getPrimaryMaximum(), durationVal);
            durationSlider.setPrimaryValue(durationVal, juce::dontSendNotification);
        });

        durationSlider.setOnPrimaryValueChanged([this](double durationVal) {
            if(!engineState.getIsLinked())
                return;
            double emissionVal = 1.0 / durationVal;
            emissionVal = juce::jlimit(emissionSlider.getPrimaryMinimum(), emissionSlider.getPrimaryMaximum(), emissionVal);
            emissionSlider.setPrimaryValue(emissionVal, juce::dontSendNotification);
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

        attackLabel.getProperties().set("isValue", true);
        decayLabel.getProperties().set("isValue", true);
        sustainLabel.getProperties().set("isValue", true);
        releaseLabel.getProperties().set("isValue", true);

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
    }

    void GrainsPanel::linkButtonClicked() { setLinkButtonImage(); }

    void GrainsPanel::setLinkButtonImage()
    {
        if(engineState.getIsLinked())
        {
            engineState.setLink(false);
            linkBtn.setIcon(linkOffIcon.get());
        }
        else
        {
            engineState.setLink(true);
            linkBtn.setIcon(linkInIcon.get());

            double emissionVal = emissionSlider.getPrimaryValue();
            double durationVal = 1.0 / emissionVal;
            durationVal = juce::jlimit(durationSlider.getPrimaryMinimum(), durationSlider.getPrimaryMaximum(), durationVal);
            durationSlider.setPrimaryValue(durationVal, juce::dontSendNotification);
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

        juce::Justification centerBot = juce::Justification::centredBottom;
        attackLabel.setJustificationType(centerBot);
        decayLabel.setJustificationType(centerBot);
        sustainLabel.setJustificationType(centerBot);
        releaseLabel.setJustificationType(centerBot);
    }
}