/*
  ==============================================================================

	SynthFrame.cpp
	Created: 3 Feb 2023 11:44:16pm
	Author:  user

  ==============================================================================
*/

#include "SynthFrame.h"
#include "../utils/MyColours.h"

SynthFrame::SynthFrame(ValueTreeState& apvts)
{
    mixSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, Param::Mix::id, mixSlider);

    gainSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, Param::Gain::id, gainSlider);

    filePosSliderAttachment =
        std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, Param::Position::id, filePosSlider);

    windowSelectionSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, Param::Selection::id, windowSelectionSlider);

    mixSlider.setName("mixSlider");
    mixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    mixSlider.setTextBoxIsEditable(true);
    mixSlider.setRange(0.0, 100.0);
    mixSlider.setTextValueSuffix(" %");
    //mixSlider.addListener(this);
    mixSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);
    // mixSlider.onValueChane = [this]() { foo(); }

    mixLabel.setText((const juce::String)Param::Mix::name, juce::dontSendNotification);
    mixLabel.attachToComponent(&mixSlider, false);
    mixLabel.setJustificationType(juce::Justification::centred);

    gainSlider.setName("gainSlider");
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
    gainSlider.setTextBoxIsEditable(true);
    gainSlider.setRange(Param::Gain::min, Param::Gain::max);
    gainSlider.setSkewFactorFromMidPoint(-12.0);
    gainSlider.setTextValueSuffix(" dB");
    //gainSlider.addListener(this);
    gainSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

    gainLabel.setText((const juce::String)Param::Gain::name, juce::dontSendNotification);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(&mixSlider);
    addAndMakeVisible(&gainSlider);
    addAndMakeVisible(&mixLabel);
    addAndMakeVisible(&gainLabel);

    filePosSlider.setName("filePosSlider");
    filePosSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    filePosSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
    filePosSlider.setTextBoxIsEditable(true);
    filePosSlider.setRange(Param::Position::min, Param::Position::max);
    //filePosSlider.addListener(this);

    filePosLabel.setText((const juce::String)Param::Position::name, juce::dontSendNotification);
    filePosLabel.attachToComponent(&filePosSlider, false);
    filePosLabel.setJustificationType(juce::Justification::centred);

    windowSelectionSlider.setName("windowSelection");
    windowSelectionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    windowSelectionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
    windowSelectionSlider.setTextBoxIsEditable(true);
    windowSelectionSlider.setRange(Param::Selection::min, Param::Selection::max);
    //windowSelectionSlider.addListener(this);

    windowSelectionLabel.setText((const juce::String)Param::Selection::name, juce::dontSendNotification);
    windowSelectionLabel.attachToComponent(&windowSelectionSlider, false);
    windowSelectionLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(&filePosSlider);
    addAndMakeVisible(&windowSelectionSlider);
    addAndMakeVisible(&filePosLabel);
    addAndMakeVisible(&windowSelectionLabel);
}

void SynthFrame::paint(juce::Graphics& g) { g.fillAll(MyColours::black); }

void SynthFrame::resized()
{
    float h = getHeight() / 30.f;

    juce::FlexBox mainFlexBox;
    mainFlexBox.flexDirection = juce::FlexBox::Direction::row;

    juce::FlexBox flexBox1;
    flexBox1.flexDirection = juce::FlexBox::Direction::column;
    flexBox1.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox1.alignContent = juce::FlexBox::AlignContent::center;

    flexBox1.items.add(juce::FlexItem(mixSlider).withFlex(0.8f).withMargin(h));
    flexBox1.items.add(juce::FlexItem(mixLabel).withFlex(0.2f));

    juce::FlexBox flexBox2;
    flexBox2.flexDirection = juce::FlexBox::Direction::column;
    flexBox2.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox2.alignContent = juce::FlexBox::AlignContent::center;

    flexBox2.items.add(juce::FlexItem(gainSlider).withFlex(0.8f).withMargin(h));
    flexBox2.items.add(juce::FlexItem(gainLabel).withFlex(0.2f));

    juce::FlexBox flexBox3;
    flexBox3.flexDirection = juce::FlexBox::Direction::column;
    flexBox3.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexBox3.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox3.alignContent = juce::FlexBox::AlignContent::center;

    flexBox3.items.add(juce::FlexItem(filePosLabel).withFlex(0.2f).withMargin(h));
    flexBox3.items.add(juce::FlexItem(filePosSlider).withFlex(0.5f));

    juce::FlexBox flexBox4;
    flexBox4.flexDirection = juce::FlexBox::Direction::column;
    flexBox4.flexWrap = juce::FlexBox::Wrap::noWrap;
    flexBox4.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
    flexBox4.alignContent = juce::FlexBox::AlignContent::center;

    flexBox4.items.add(juce::FlexItem(windowSelectionLabel).withFlex(0.2f).withMargin(h));
    flexBox4.items.add(juce::FlexItem(windowSelectionSlider).withFlex(0.5f));

    juce::FlexBox flexBox5;
    flexBox5.flexDirection = juce::FlexBox::Direction::column;

    flexBox5.items.add(juce::FlexItem(flexBox3).withFlex(0.5f).withMargin(h));
    flexBox5.items.add(juce::FlexItem(flexBox4).withFlex(0.5f).withMargin(h));

    mainFlexBox.items.add(juce::FlexItem(flexBox5).withFlex(0.66f));
    mainFlexBox.items.add(juce::FlexItem(flexBox1).withFlex(0.25f));
    mainFlexBox.items.add(juce::FlexItem(flexBox2).withFlex(0.25f)); // .withMargin(h));

    mainFlexBox.performLayout(getLocalBounds().toFloat());
}