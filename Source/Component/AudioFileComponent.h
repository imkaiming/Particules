#pragma once
#include <JuceHeader.h>
#include "../Utils/FileLoader.h"



class AudioFileComponent : public juce::Component {
public:
	AudioFileComponent();
	~AudioFileComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;


private:
	juce::DrawableButton openFileButton;
	juce::DrawableButton saveFileButton;
	juce::DrawableButton playFileButton;

	void openFileButtonClicked();
	void saveFileButtonClicked();
	void playFileButtonClicked();

	juce::AudioFormatManager audioFormatManager;
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileComponent)
};
