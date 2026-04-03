#include "Scheduler.h"
#include "../utils/struct/ParameterSnapshot.h"

namespace particules
{
    Scheduler::Scheduler() : emission{0.f}, sampleRate{0.0}, phase{0.0}, interOnSet{0.0} {}

    void Scheduler::reset()
    {
        emission = 0.f;
        interOnSet = 0.0;
        phase = 0.0;
    }

    // prepare to play
    void Scheduler::init(double sr) noexcept { sampleRate = sr; }

    // start of the process function
    void Scheduler::setEmission(float e) noexcept
    {
        emission = std::clamp(e, 0.01f, 500.f);
        interOnSet = sampleRate / (double)emission;
    }

    //void Scheduler::tick(int index, std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& snapshot)
    void Scheduler::tick(std::function<void(const ParameterSnapshot&)> spawn, const ParameterSnapshot& ps)
    {
        if(phase >= interOnSet)
        {
            spawn(ps);
            phase -= interOnSet;
        }
        phase++;
    }
}
//const double Scheduler::getInterOnSet(float emission, double sampleRate) const noexcept
//{
//emission = std::min(emission, 500.f);
//if(emission <= 0.0)
//    emission = std::max(emission, 0.1f);
//    return sampleRate / (double)emission;
//}
/*
void Scheduler::processBlock(int bufferSize, double sampleRate, float emission,
    std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& snapshot)
{
    if(emission <= 0.f)
        return;

    emission = std::min(emission, 500.f);
    const double interOnSet = sampleRate / (double)emission;
    if(interOnSet <= 1) // only occur with uncommon sample rates configs
    {
        setNextOnSet(0.0);
        return;
    }

    int count = 0;
    double offset = getNextOnSet(); // offset of the next outBuffer call
    while(offset < static_cast<double>(bufferSize) && count < SIZE)
    {
        spawn(static_cast<int>(std::floor(offset)), snapshot); // call the voice manager
        offset += interOnSet;
        count++;
    }

    setNextOnSet(offset - static_cast<double>(bufferSize));
};
*/