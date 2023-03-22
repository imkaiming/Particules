/*
  ==============================================================================

	SynthFrame.cpp
	Created: 3 Feb 2023 11:44:16pm
	Author:  user

  ==============================================================================
*/

#include "SynthFrame.h"

SynthFrame::SynthFrame(ValueTreeState* apvts, StateParameters* stateParams) :
	apvts(apvts), stateParams(stateParams)
{
	mixSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, MIX_ID, mixSlider);

	gainSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, GAIN_ID, gainSlider);

	mixSlider.setName("mixSlider");
	mixSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
	mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	mixSlider.setTextBoxIsEditable(true);
	mixSlider.setRange(0.0, 100.0);
	mixSlider.setTextValueSuffix(" %");
	mixSlider.addListener(this);


	mixLabel.setText((const juce::String)MIX_NAME, juce::dontSendNotification);
	mixLabel.attachToComponent(&mixSlider, false);
	mixLabel.setJustificationType(juce::Justification::centred);

	gainSlider.setName("gainSlider");
	gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
	gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		true, 100, 25);
	gainSlider.setTextBoxIsEditable(true);
	gainSlider.setRange(GAIN_MIN, GAIN_MAX);
	gainSlider.setSkewFactorFromMidPoint(-12.0);
	gainSlider.setTextValueSuffix(" dB");
	gainSlider.addListener(this);


	gainLabel.setText((const juce::String)GAIN_NAME, juce::dontSendNotification);
	gainLabel.attachToComponent(&gainSlider, false);
	gainLabel.setJustificationType(juce::Justification::centred);


	addAndMakeVisible(&mixSlider);
	addAndMakeVisible(&gainSlider);
	addAndMakeVisible(&mixLabel);
	addAndMakeVisible(&gainLabel);



	filePosSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, POSITION_ID, filePosSlider);

	windowSelectionSliderAttachment =
		std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
			*apvts, SELECTION_ID, windowSelectionSlider);

	filePosSlider.setName("filePosSlider");
	filePosSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	filePosSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
	filePosSlider.setTextBoxIsEditable(true);
	filePosSlider.setRange(POSITION_MIN, POSITION_MAX);
	filePosSlider.addListener(this);


	filePosLabel.setText((const juce::String)POSITION_NAME, juce::dontSendNotification);
	filePosLabel.attachToComponent(&filePosSlider, false);
	filePosLabel.setJustificationType(juce::Justification::centred);

	windowSelectionSlider.setName("windowSelection");
	windowSelectionSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
	windowSelectionSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 100, 25);
	windowSelectionSlider.setTextBoxIsEditable(true);
	windowSelectionSlider.setRange(SELECTION_MIN, SELECTION_MAX);
	windowSelectionSlider.addListener(this);


	windowSelectionLabel.setText((const juce::String)SELECTION_NAME, juce::dontSendNotification);
	windowSelectionLabel.attachToComponent(&windowSelectionSlider, false);
	windowSelectionLabel.setJustificationType(juce::Justification::centred);

	addAndMakeVisible(&filePosSlider);
	addAndMakeVisible(&windowSelectionSlider);
	addAndMakeVisible(&filePosLabel);
	addAndMakeVisible(&windowSelectionLabel);
}

SynthFrame::~SynthFrame()
{
	mixSlider.removeListener(this);
	gainSlider.removeListener(this);
	apvts = nullptr;
	stateParams = nullptr;
}

void SynthFrame::paint(juce::Graphics& g) {
	//juce::Rectangle<float> synthFrame(0.f, 0.f, getWidth(), getHeight());
	//g.setColour(juce::Colours::slategrey);
	//g.fillRect(synthFrame);
	//g.drawRect(synthFrame);
	g.fillAll(MyColours::black);
}

void SynthFrame::resized()
{

	float h = getHeight() / 30.f;
	//juce::Rectangle<int> localArea = getLocalBounds();
	//localArea.removeFromTop(h);
	//localArea.removeFromBottom(h);

	juce::FlexBox mainFlexBox;
	mainFlexBox.flexDirection = juce::FlexBox::Direction::row;

	juce::FlexBox flexBox1;
	flexBox1.flexDirection = juce::FlexBox::Direction::column;
	flexBox1.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox1.alignContent = juce::FlexBox::AlignContent::center;

	flexBox1.items.add(juce::FlexItem(mixSlider).withFlex(0.8).withMargin(h));
	flexBox1.items.add(juce::FlexItem(mixLabel).withFlex(0.2));

	juce::FlexBox flexBox2;
	flexBox2.flexDirection = juce::FlexBox::Direction::column;
	flexBox2.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox2.alignContent = juce::FlexBox::AlignContent::center;

	flexBox2.items.add(juce::FlexItem(gainSlider).withFlex(0.8).withMargin(h));
	flexBox2.items.add(juce::FlexItem(gainLabel).withFlex(0.2));


	juce::FlexBox flexBox3;
	flexBox3.flexDirection = juce::FlexBox::Direction::column;
	flexBox3.flexWrap = juce::FlexBox::Wrap::noWrap;
	flexBox3.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox3.alignContent = juce::FlexBox::AlignContent::center;

	flexBox3.items.add(juce::FlexItem(filePosLabel).withFlex(0.2).withMargin(h));
	flexBox3.items.add(juce::FlexItem(filePosSlider).withFlex(0.5));

	juce::FlexBox flexBox4;
	flexBox4.flexDirection = juce::FlexBox::Direction::column;
	flexBox4.flexWrap = juce::FlexBox::Wrap::noWrap;
	flexBox4.justifyContent = juce::FlexBox::JustifyContent::spaceAround;
	flexBox4.alignContent = juce::FlexBox::AlignContent::center;

	flexBox4.items.add(juce::FlexItem(windowSelectionLabel).withFlex(0.2).withMargin(h));
	flexBox4.items.add(juce::FlexItem(windowSelectionSlider).withFlex(0.5));

	juce::FlexBox flexBox5;
	flexBox5.flexDirection = juce::FlexBox::Direction::column;

	flexBox5.items.add(juce::FlexItem(flexBox3).withFlex(0.5).withMargin(h));
	flexBox5.items.add(juce::FlexItem(flexBox4).withFlex(0.5).withMargin(h));

	mainFlexBox.items.add(juce::FlexItem(flexBox5).withFlex(0.66));
	mainFlexBox.items.add(juce::FlexItem(flexBox1).withFlex(0.25));
	mainFlexBox.items.add(juce::FlexItem(flexBox2).withFlex(0.25)); // .withMargin(h));

	mainFlexBox.performLayout(getLocalBounds().toFloat());
}

void SynthFrame::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &mixSlider)
	{
		float mixValue = static_cast<float>(mixSlider.getValue());
		apvts->getRawParameterValue(MIX_ID)->store(mixValue);
		stateParams->setMix(mixValue);
		//this->apvts->mix = mixSlider.getValue();
	}
	if (slider == &gainSlider)
	{
		float gainValue = static_cast<float>(gainSlider.getValue());
		apvts->getRawParameterValue(GAIN_ID)->store(gainValue);
		stateParams->setGain(gainValue);
	}
	if (slider == &filePosSlider)
	{
		float filePosValue = static_cast<float>(filePosSlider.getValue());
		apvts->getRawParameterValue(POSITION_ID)->store(filePosValue);
		stateParams->setFilePosition(filePosValue);
		thumbnailComponent->updatePosition(filePosValue);
	}
	if (slider == &windowSelectionSlider)
	{
		float windowValue = static_cast<float>(windowSelectionSlider.getValue());
		apvts->getRawParameterValue(SELECTION_ID)->store(windowValue);
		stateParams->setWindowSelection(windowValue);
		thumbnailComponent->updateSelection(windowValue);
	}

}

void SynthFrame::init(ThumbnailComponent* thumbnailComponent)
{
	this->thumbnailComponent = thumbnailComponent;
}
