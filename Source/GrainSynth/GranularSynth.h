#include <JuceHeader.h>
#include "../Utils/StateSaver.h"
#include "Scheduler.h"

class GranularSynth { //; : public juce::Synthesiser {
public:
	GranularSynth(StateSaver* stateSaver);
	~GranularSynth();
	void  prepareToPlay(double sampleRate, int samplesPerBlock);
	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&, int);

private:
	double sampleRate;
	int samplesPerBlock;
	//juce::AudioProcessorValueTreeState treeState;
	StateSaver* stateSaver;
	Scheduler scheduler; // responsible to determine when the next grain should be extracted
	//AudioFileLoader fileLoader; // responsible to load the audio source and put it in the samples pool
};