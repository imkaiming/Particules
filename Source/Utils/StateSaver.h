#pragma once

#include "AudioFileLoader.h"

class StateSaver {
public:
	StateSaver();
	~StateSaver();
	//AudioFileLoader audioFileLoader;
private:
	bool isAudioLoaded;
	int sampleRate;
};