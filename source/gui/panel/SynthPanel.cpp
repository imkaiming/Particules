#include "SynthPanel.h"
#include "../../utils/MyColours.h"
//#include "../../utils/PluginParams.h"

namespace particules
{

    SynthPanel::SynthPanel(ValueTreeState& apvts)
    {
        mixSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, globalMixId, mixSlider);

        outputSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, globalOutputId, outputSlider);

        positionSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, globalPositionId, positionSlider);

        selectionSliderAttachment =
            std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, globalSelectionId, selectionSlider);

        mixSlider.setName("mixSlider");
        mixSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBar);
        mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
        mixSlider.setTextBoxIsEditable(true);
        mixSlider.setRange(0.0, 100.0);
        //mixSlider.setTextValueSuffix(" %");
        //mixSlider.addListener(this);
        mixSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);
        // mixSlider.onValueChane = [this]() { foo(); }

        mixLabel.setText((const str)globalMixName, juce::dontSendNotification);
        mixLabel.attachToComponent(&mixSlider, false);
        mixLabel.setJustificationType(juce::Justification::centred);

        outputSlider.setName("outputSlider");
        outputSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
        outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 25);
        outputSlider.setTextBoxIsEditable(true);
        outputSlider.setRange(globalOutputMin, globalOutputMax);
        outputSlider.setSkewFactorFromMidPoint(-12.0);
        //outputSlider.setTextValueSuffix(" dB");
        //outputSlider.addListener(this);
        outputSlider.setColour(juce::Slider::textBoxOutlineColourId, MyColours::black);

        outputLabel.setText((const str)globalOutputName, juce::dontSendNotification);
        outputLabel.attachToComponent(&outputSlider, false);
        outputLabel.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(&mixSlider);
        addAndMakeVisible(&outputSlider);
        addAndMakeVisible(&mixLabel);
        addAndMakeVisible(&outputLabel);

        positionSlider.setName("positionSlider");
        positionSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
        positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
        positionSlider.setTextBoxIsEditable(true);
        positionSlider.setRange(globalPositionMin, globalPositionMax);
        //filePosSlider.addListener(this);

        positionLabel.setText((const str)globalPositionName, juce::dontSendNotification);
        positionLabel.attachToComponent(&positionSlider, false);
        positionLabel.setJustificationType(juce::Justification::centred);

        selectionSlider.setName("selectionSlider");
        selectionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        selectionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
        selectionSlider.setTextBoxIsEditable(true);
        selectionSlider.setRange(globalSelectionMin, globalSelectionMax);
        //windowSelectionSlider.addListener(this);

        selectionLabel.setText((const str)globalSelectionName, juce::dontSendNotification);
        selectionLabel.attachToComponent(&selectionSlider, false);
        selectionLabel.setJustificationType(juce::Justification::centred);

        addAndMakeVisible(&positionSlider);
        addAndMakeVisible(&selectionSlider);
        addAndMakeVisible(&positionLabel);
        addAndMakeVisible(&selectionLabel);
    }

    void SynthPanel::paint(juce::Graphics& g) { g.fillAll(MyColours::black); }

    void SynthPanel::resized()
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

        flexBox2.items.add(juce::FlexItem(outputSlider).withFlex(0.8f).withMargin(h));
        flexBox2.items.add(juce::FlexItem(outputLabel).withFlex(0.2f));

        juce::FlexBox flexBox3;
        flexBox3.flexDirection = juce::FlexBox::Direction::column;
        flexBox3.flexWrap = juce::FlexBox::Wrap::noWrap;
        flexBox3.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
        flexBox3.alignContent = juce::FlexBox::AlignContent::center;

        flexBox3.items.add(juce::FlexItem(positionLabel).withFlex(0.2f).withMargin(h));
        flexBox3.items.add(juce::FlexItem(positionSlider).withFlex(0.5f));

        juce::FlexBox flexBox4;
        flexBox4.flexDirection = juce::FlexBox::Direction::column;
        flexBox4.flexWrap = juce::FlexBox::Wrap::noWrap;
        flexBox4.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
        flexBox4.alignContent = juce::FlexBox::AlignContent::center;

        flexBox4.items.add(juce::FlexItem(selectionLabel).withFlex(0.2f).withMargin(h));
        flexBox4.items.add(juce::FlexItem(selectionSlider).withFlex(0.5f));

        juce::FlexBox flexBox5;
        flexBox5.flexDirection = juce::FlexBox::Direction::column;

        flexBox5.items.add(juce::FlexItem(flexBox3).withFlex(0.5f).withMargin(h));
        flexBox5.items.add(juce::FlexItem(flexBox4).withFlex(0.5f).withMargin(h));

        mainFlexBox.items.add(juce::FlexItem(flexBox5).withFlex(0.66f));
        mainFlexBox.items.add(juce::FlexItem(flexBox1).withFlex(0.25f));
        mainFlexBox.items.add(juce::FlexItem(flexBox2).withFlex(0.25f)); // .withMargin(h));

        mainFlexBox.performLayout(getLocalBounds().toFloat());
    }
}