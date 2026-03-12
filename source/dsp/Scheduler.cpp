/*
  ==============================================================================

	Scheduler.cpp
	Created: 15 Feb 2023 1:44:18pm
	Author:  user

  ==============================================================================
*/

#include "Scheduler.h"
//#include "PositionModulator.h"

Scheduler::Scheduler() : nextOnSet{0} {}

const double Scheduler::getInterOnSet(float emission, double sampleRate) const noexcept
{
    emission = std::min(emission, 500.f);
    if(emission <= 0.0)
        emission = std::max(emission, 0.1f);
    return sampleRate / (double)emission;
}

void Scheduler::process(int bufferSize, double sampleRate, float emission,
    std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& snapshot)
{
    const double interOnSet = getInterOnSet(emission, sampleRate);
    if(interOnSet <= 1) // only occur with uncommon sample rates configs
    {
        setOffset(0.0);
        return; 
    } 

    int count = 0;
    double offset = getOffset(); // offset of the next outBuffer call
    while(offset < static_cast<double>(bufferSize) && count < mCapacity)
    {
        spawn(static_cast<int>(std::floor(offset)), snapshot); // call the voice manager
        offset += interOnSet;
        count++;
    }

    setOffset(offset - static_cast<double>(bufferSize));
};