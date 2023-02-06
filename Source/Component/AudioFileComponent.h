#pragma once
#include <JuceHeader.h>
#include "../Utils/FileLoader.h"

enum TransportState
{
	Stopped,
	Starting,
	Playing,
	Stopping
};


class AudioFileComponent : public juce::Component, public juce::ChangeListener {
public:
	AudioFileComponent();
	~AudioFileComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	void AudioFileComponent::changeState(TransportState newState);
	TransportState getCurrentState();
	std::unique_ptr<juce::AudioFormatReaderSource> getReaderSource();
	juce::AudioTransportSource getTransportSource();

private:

	juce::DrawableButton openFileButton;
	juce::DrawableButton stopFileButton;
	juce::DrawableButton playFileButton;

	void openFileButtonClicked();
	void stopFileButtonClicked();
	void playFileButtonClicked();

	juce::AudioFormatManager formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
	juce::AudioTransportSource transportSource;
	TransportState state;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileComponent)
};
