
#include "AudioFileComponent.h"

AudioFileComponent::AudioFileComponent(juce::DrawableButton* open_btn, 
	juce::DrawableButton* play_btn, juce::DrawableButton* stop_btn)
{

	this->open_btn = open_btn;
	this->play_btn = play_btn;
	this->stop_btn = stop_btn;

	addAndMakeVisible(this->open_btn);
	addAndMakeVisible(this->play_btn);
	addAndMakeVisible(this->stop_btn);
}

AudioFileComponent::~AudioFileComponent() {
	this->open_btn = nullptr;
	this->play_btn = nullptr;
	this->stop_btn = nullptr;
}

void AudioFileComponent::paint(juce::Graphics&) {

}

void AudioFileComponent::resized() {
	juce::Rectangle<int> area = getLocalBounds();
	int h = getHeight() / 3;

	open_btn->setBounds(area.removeFromTop(h));
	play_btn->setBounds(area.removeFromTop(h));
	stop_btn->setBounds(area.removeFromTop(h));
}