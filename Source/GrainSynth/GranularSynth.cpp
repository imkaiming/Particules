#include "GranularSynth.h"

GranularSynth::GranularSynth() {
	this->stateSaver = new StateSaver();
};

GranularSynth::~GranularSynth() {

};

void GranularSynth::prepareToPlay(double sampleRate, int samplesPerBlock) {

};

void GranularSynth::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {


};

StateSaver GranularSynth::getStateSaver() {
	return *stateSaver; // retour par valeur
}