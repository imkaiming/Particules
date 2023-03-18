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
	AudioFileComponent(juce::DrawableButton*, juce::DrawableButton*, juce::DrawableButton*);
	~AudioFileComponent() override;
	void paint(juce::Graphics&) override;
	void resized() override;


private:

	juce::DrawableButton* open_btn;
	juce::DrawableButton* play_btn;
	juce::DrawableButton* stop_btn;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioFileComponent)
};
