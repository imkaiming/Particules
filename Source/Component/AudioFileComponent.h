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
	AudioFileComponent();
	~AudioFileComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;
	void init(StateSaver*);
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
	void loadAudioFile(); // open window, select a file using loader's methods and put in the StaeSaver ptr



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileComponent)
};
