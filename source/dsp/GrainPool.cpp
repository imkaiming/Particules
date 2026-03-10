/*
  ==============================================================================

	GrainPool.cpp
	Created: 6 Feb 2026 6:01:42pm
	Author:  kai

  ==============================================================================
*/

#include "GrainPool.h"
#include "../framework/GrainHandle.h"
#include "Grain.h"

GrainPool::GrainPool() { reset(); }

void GrainPool::reset()
{
    nextFree = 0;
    for(int i = 0; i < mCapacity; ++i)
    {
        freeIndices[i] = i;
        grains[i].setActive(false);
        grains[i].reset();
    }
}

// return grains corresponding handle's index if valid
Grain* GrainPool::get(const GrainHandle handle)
{
    if(!handle.isValid() || handle.index >= mCapacity) // check if index is not 0xFFFF
        return nullptr;

    Grain& g = grains[handle.index];
    if((g.getGeneration() == handle.gen) && g.getActive())
        return &g;
    else
        return nullptr; // return the grain only if the generation matches
}

// always provide and init a grain unless the pool capacity is insufficient
// return a valid handle and set active a grain at nextFree position
/*
bool GrainPool::acquire(GrainHandle& outHandle, Grain*& outGrain)
{
    if(nextFree >= mCapacity)
        return false;

    // accessing the next available grain
    const uint16_t i = freeIndices[nextFree++];
    Grain& g = grains[i];

    g.setActive(true);
    outHandle.index = i;
    outHandle.gen = g.getGeneration();
    outGrain = &g;

    return true;
}
*/

GrainHandle GrainPool::acquire()
{
    if(nextFree >= mCapacity)
        return GrainHandle::getInvalidState();

    const int i = freeIndices[nextFree++];
    Grain& g = grains[i];
    g.setActive(true);
    return GrainHandle{(uint16_t)i, g.getGeneration()};
}

// set a grain inactive and increment generation
void GrainPool::release(const GrainHandle handle)
{
    if(handle.index >= mCapacity || !handle.isValid())
        return; // security gards

    Grain& g = grains[handle.index];

    if(g.getGeneration() != handle.gen || !g.getActive())
        return; // prevent releasing the wrong grain

    g.setActive(false);
    g.incrementGen();

    freeIndices[--nextFree] = handle.index;
}
