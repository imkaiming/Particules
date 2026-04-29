#include "Scheduler.h"
#include "utils/struct/AudioPayload.h"
#include "utils/struct/ParameterSnapshot.h"

namespace particules
{
    Scheduler::Scheduler() : emission{0.f}, sampleRate{0.0}, phase{0.0}, interOnSet{0.0}, nextOnSet{0.0} {}

    // reset should never flush users settings
    void Scheduler::reset()
    {
        phase = interOnSet;
        nextOnSet = 0.0;
    }

    // prepare to play
    void Scheduler::setSampleRate(double sr) noexcept
    {
        sampleRate = sr;
        if(emission > 0.f)
            setEmission(emission); // in case emission has been setted befor the sample rate
        reset();
    }

    // start of the process function
    void Scheduler::setEmission(float e) noexcept
    {
        //emission = std::clamp(e, 0.01f, 500.f);
        emission = std::clamp(e, params::emission::min, params::emission::max);

        if(sampleRate > 0.0) // in case sample rate hasnt been initialized yet
            interOnSet = sampleRate / (double)emission;
        else
            interOnSet = 0.0;

        // clamp the phase to interOnSet  
        if(phase > interOnSet)
            phase = interOnSet;
    }

    void Scheduler::tick(SpawnGrainCallback spawn, const ParameterSnapshot& ps, AudioPayload* payload, int indexVoice, float pitchRatio, float gain)
    {
        // doing nothing is scheduler hasnt been initialized
        if(interOnSet <= 0.0)
            return;

        if(phase >= interOnSet) // zero latency trigger
        {
            spawn(ps, payload, indexVoice, pitchRatio, gain);
            phase -= interOnSet;
        }

        phase++;
    }
}