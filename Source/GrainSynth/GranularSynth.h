#include <JuceHeader.h>
#include "../Utils/StateSaver.h"
#include "Scheduler.h"
#include "../Utils/AudioFileLoader.h"

class GranularSynth {
public:
	GranularSynth();
	~GranularSynth();
	void  prepareToPlay(double sampleRate, int samplesPerBlock);
	void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
	StateSaver getStateSaver();

private:
	double sampleRate;
	int samplesPerBlock;
	//juce::AudioProcessorValueTreeState treeState;
	StateSaver* stateSaver;
	Scheduler scheduler; // responsible to determine when the next grain should be extracted
	AudioFileLoader fileLoader; // responsible to load the audio source and put it in the samples pool
};