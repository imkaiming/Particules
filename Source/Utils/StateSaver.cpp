#include "StateSaver.h"

StateSaver::StateSaver() {
	isAudioLoaded = false;
};

StateSaver::~StateSaver() {

};

void StateSaver::setAudioLoaded(bool b) {
	juce::Logger::outputDebugString("StateSaver -> audioLoaded !");
	isAudioLoaded = b;
}
bool StateSaver::getAudioLoaded() {
	return isAudioLoaded;
}