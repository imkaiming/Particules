/*
  ==============================================================================

	GrainPool.cpp
	Created: 6 Feb 2026 6:01:42pm
	Author:  kai

  ==============================================================================
*/

#include "GrainPool.h"
#include "../utils/struct/GrainHandle.h"
//#include "GrainEnvelope.h"
#include "Grain.h"

namespace particules
{

    GrainPool::GrainPool() { reset(); }

    void GrainPool::reset()
    {
        nextFree = 0;
        for(int i = 0; i < SIZE; ++i)
        {
            freeIndices[i] = i;
            grains[i].setActive(false);
            grains[i].reset();
        }
    }

    // return grains corresponding handle's index if valid
    Grain* GrainPool::get(const GrainHandle handle)
    {
        if(!handle.isValid() || handle.index >= SIZE) // check if index is not 0xFFFF
            return nullptr;

        Grain& g = grains[handle.index];
        if((g.getGeneration() == handle.gen) && g.getActive())
            return &g;
        else
            return nullptr; // return the grain only if the generation matches
    }

    GrainHandle GrainPool::acquire()
    {
        if(nextFree >= SIZE)
            return GrainHandle::getInvalidState();

        const int i = freeIndices[nextFree++];
        Grain& g = grains[i];
        g.setActive(true);
        return GrainHandle{(uint16_t)i, g.getGeneration()};
    }

    // set a grain inactive and increment generation
    void GrainPool::release(const GrainHandle handle)
    {
        if(handle.index >= SIZE || !handle.isValid())
            return; // security gards

        Grain& g = grains[handle.index];

        if(g.getGeneration() != handle.gen || !g.getActive())
            return; // prevent releasing the wrong grain

        g.setActive(false);
        g.incrementGen();

        freeIndices[--nextFree] = handle.index;
    }
}