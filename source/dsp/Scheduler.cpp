/*
  ==============================================================================

	Scheduler.cpp
	Created: 15 Feb 2023 1:44:18pm
	Author:  user

  ==============================================================================
*/

#include "Scheduler.h"
#include "PositionModulator.h"

Scheduler::Scheduler() : nextOnSet{0} {}

/*

void Scheduler::freeActiveGrains()
{
	for(int i = 0; i < this->grains.size(); ++i)
	{
		Grain* grain = this->grains.removeAndReturn(i);
		delete grain;
	}
}


Grain* Scheduler::generateGrain(int numSamples)
{
	// on récupère la valeur en samples par rapport au pourcentage de la position dans le fichier audio

	int durationSamples = static_cast<int>(paramsView.getDuration() * paramsView.getSampleRate());
	int widthSamples = static_cast<int>(durationSamples * paramsView.getEnvWidth());
	int positionSamples = static_cast<int>(paramsView.getNumSamples() * paramsView.getFilePosition());
	int selectionSamples = static_cast<int>(paramsView.getNumSamples() * paramsView.getWindowSelection());


	phaseMod.setFrequency(1 / paramsView.getTraversalTime());
	phaseMod.setMod(paramsView.getTraversalMode());
	positionSamples += phaseMod.getValue() * selectionSamples;

	//DBG("value : " << phaseMod.getValue());
	//DBG("selectionSamples : " << selectionSamples);
	//DBG("positionSamples : " << positionSamples);

	return new Grain(
		durationSamples, // le nombre total de sample dans le grain
		numChannels,
		paramsView.getEnvelopeType(),
		paramsView.getSpeed(),
		widthSamples, // le nombre de sample qu'il y a entre la fin du fade in et le debut du fade out
		positionSamples // le nombre de sample qui determine la position dans le fichier pour le depart
		//paramsView.getAudioBuffer()
	);

}

*/

double Scheduler::getInterOnSet(float density, double sampleRate) const noexcept
{
    if(density <= 0.0)
        density = std::max(density, 0.1f);
    return sampleRate / (double)density;
}

void Scheduler::process(int bufferSize, double sampleRate, float density,
    std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& parameters)
{
    const double interOnSet = getInterOnSet(density, sampleRate);
    if(interOnSet <= 1)
    {
        setOffset(0.0);
        //throw std::logic_error("interOnSet too short too schedule grains");
        return;
    } // dont need 1 grain per sample this is too much

    int count = 0;
    double offset = getOffset(); // offset of the next outBuffer call
    while(offset < static_cast<double>(bufferSize) && count < mCapacity)
    {
        spawn(static_cast<int>(std::floor(offset)), parameters); // call the voice manager
        offset += interOnSet;
        count++;
    }

    setOffset(offset - static_cast<double>(bufferSize));
};