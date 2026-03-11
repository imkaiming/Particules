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

const double Scheduler::getInterOnSet(float Emission, double sampleRate) const noexcept
{
    Emission = std::min(Emission, 500.f);
    if(Emission <= 0.0)
        Emission = std::max(Emission, 0.1f);
    return sampleRate / (double)Emission;
}

void Scheduler::process(int bufferSize, double sampleRate, float Emission,
    std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& parameters)
{
    const double interOnSet = getInterOnSet(Emission, sampleRate);
    if(interOnSet <= 1) // only occur with uncommon sample rates configs
    {
        setOffset(0.0);
        return; 
    } 

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