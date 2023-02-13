#include "GranularSynth.h"

GranularSynth::GranularSynth() 
{
	this->stateSaver = new StateSaver();
};

GranularSynth::~GranularSynth() {

};

void GranularSynth::prepareToPlay(double sampleRate, int samplesPerBlock) {

};

void GranularSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages, 
	int totalNumOutputChannels) {

	for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
		//auto* channelData = buffer.getWritePointer(channel);
		//const auto* readPointer = buffer.getReadPointer(channel);

		//for (auto sample = 0; sample < buffer.getNumChannels(); ++sample) {
			//channelData[sample] = channelData[sample] * gainFactor;
		//}
	}

};

StateSaver GranularSynth::getStateSaver() {
	return *stateSaver; // retour par valeur
}