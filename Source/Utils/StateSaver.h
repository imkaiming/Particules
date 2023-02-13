/**
 * This class is used to store the plugin's state, such as the values of the parameters.
*/

#pragma once

#include "AudioFileLoader.h"

// doit inherits from : public juce::AudioProcessorValueTreeState
// ou alors: public juce::AudioProcessorState

class StateSaver {
public:
	StateSaver();
	~StateSaver();
	void setAudioLoaded(bool);
	bool getAudioLoaded();
	//AudioFileLoader audioFileLoader;
private:
	bool isAudioLoaded;
	int sampleRate;

	// all parameters
	float position;
	float density;
	float grainSize;
	float speed;
};