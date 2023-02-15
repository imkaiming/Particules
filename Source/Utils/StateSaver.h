/**
 * This class is used to store the plugin's state, such as the values of the parameters.
*/

#pragma once
#include <JuceHeader.h>

// doit inherits from : public juce::AudioProcessorValueTreeState
// ou alors: public juce::AudioProcessorState

class StateSaver {
public:
	StateSaver();
	~StateSaver();
	void setAudioLoaded(bool);
	bool* getAudioLoaded();
	void setSampleRate(double);
	double getSampleRate();
	void setAudioBuffer(juce::AudioBuffer<float>* buffer);
	juce::AudioBuffer<float>* getAudioBuffer();
	//AudioFileLoader audioFileLoader;

private:
	bool isAudioLoaded;
	double sampleRate;

	// all parameters
	float position;
	float density;
	float grainSize;
	float speed;

	// Le state saver contient le buffer du projet  car on veut 
	// pouvoir sauvegarder le presets avec le son qui lui est associé
	juce::AudioBuffer<float> buffer;
};