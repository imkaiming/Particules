#pragma once
#include <JuceHeader.h>
#include "../Utils/AudioFileLoader.h"
#include "../Utils/StateSaver.h"

//enum TransportState
//{
//	Stopped,
//	Starting,
//	Playing,
//	Stopping
//};

// , public juce::ChangeListener 
class AudioFileComponent : public juce::Component {
public:
	AudioFileComponent(StateSaver*);
	~AudioFileComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;

	//void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	//void AudioFileComponent::changeState(TransportState newState);
	//TransportState getCurrentState();
	//std::unique_ptr<juce::AudioFormatReaderSource> getReaderSource();
	//juce::AudioTransportSource getTransportSource();

private:

	juce::DrawableButton openFileButton;
	juce::DrawableButton stopFileButton;
	juce::DrawableButton playFileButton;

	void openFileButtonClicked();
	void stopFileButtonClicked();
	void playFileButtonClicked();

	StateSaver* stateSaver;
	AudioFileLoader loader;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileComponent)
};
