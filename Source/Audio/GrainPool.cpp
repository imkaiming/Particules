/*
  ==============================================================================

	GrainPool.cpp
	Created: 6 Feb 2026 6:01:42pm
	Author:  kai

  ==============================================================================
*/


#include "GrainPool.h"
#include "../Framework/GrainHandle.h"


GrainPool::GrainPool() { reset(); }

void GrainPool::reset()
{
	nextFree = 0;
	for(uint16_t i = 0; i < mCapacity; ++i)
	{
		freeIndices[i] = i;
		grains[i].setActive(false);
		grains[i].reset();
	}
}

Grain* GrainPool::get(const GrainHandle handle) 
{
	if(!isValid(handle)) return nullptr;
	return &grains[handle.index]; // return pointer so the pool still have the ownership of its grains baby
}

bool GrainPool::isValid(const GrainHandle handle) const
{
	if(!handle.isValid())	return false; // does the handle has been init first ?
	if(handle.index >= mCapacity) return false;

	const Grain& g = grains[handle.index];
	return ((g.getGeneration() == handle.gen) && g.getActive()); // return the grain only if the generation matches 
}

bool GrainPool::acquire(GrainHandle& outHandle, Grain*& outGrain)
{
	if(nextFree >= mCapacity) return false;

	// accessing the next available grain
	const uint16_t i = freeIndices[nextFree++];
	Grain& g = grains[i];

	// setting it up
	g.setActive(true);
	outHandle.index = i;
	outHandle.gen = g.getGeneration();
	outGrain = &g;

	return true;
}


void GrainPool::release(const GrainHandle handle)
{
	if(handle.index >= mCapacity || nextFree == 0) return; // security gards

	Grain& g = grains[handle.index];
	if(g.getGeneration() != handle.gen) return; // prevent releasing the wrong grain

	g.setActive(false); g.incrementGen();

	freeIndices[--nextFree] = handle.index;
}




/*
void GrainPool::reset()
{
	freeTop  = 0;
	numActive = 0;
	for(uint16_t i = 0; i < mCapacity; ++i)
	{
		freeIndices[freeTop ++] = i;
	}
}

Grain* GrainPool::acquireGrain()
{
	if(freeTop == 0)return nullptr;

	uint16_t index = freeIndices[--freeTop];
	Grain* g = &grains[index];
	g->reset(); // maybe not usefull if we config the grain with the snapshot
	numActive++;
	return g;
}

void GrainPool::releaseGrain(Grain* grain)
{
	if(grain == nullptr) return;
	if(freeTop >= mCapacity) { jassertfalse; return; }

	ptrdiff_t index = grain - grains.data(); // ptr arithmetic
	if(index < 0 || index >= static_cast<ptrdiff_t>(mCapacity))
	{
		jassert(false && "problem with computing index from grain ptr");
		return;
	}
	freeIndices[freeTop++] = static_cast<uint16_t>(index);
	numActive--;

}

*/

