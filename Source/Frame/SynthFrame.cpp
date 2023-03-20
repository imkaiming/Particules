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
	mixSlider.setTextBoxIsEditable(true);
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


	mainFlexBox.items.add(juce::FlexItem(flexBox1).withFlex(0.25));
	mainFlexBox.items.add(juce::FlexItem(flexBox2).withFlex(0.25)); // .withMargin(h));

	mainFlexBox.performLayout(getLocalBounds().toFloat());
}

void SynthFrame::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &mixSlider) {
		apvts->getRawParameterValue(MIX_ID)->store(static_cast<float>(mixSlider.getValue()));
		stateParams->setMix(static_cast<float>(mixSlider.getValue()));
		//this->apvts->mix = mixSlider.getValue();
	}
	if (slider == &gainSlider) {
		apvts->getRawParameterValue(GAIN_ID)->store(
			static_cast<float>((gainSlider.getValue())));
		stateParams->setGain(
			static_cast<float>(gainSlider.getValue()));

	}

}