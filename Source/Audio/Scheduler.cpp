/*
  ==============================================================================

	Scheduler.cpp
	Created: 15 Feb 2023 1:44:18pm
	Author:  user

  ==============================================================================
*/

#include "Scheduler.h"
#include "../Framework/ParameterView.h"

Scheduler::Scheduler(ParameterView& paramsView): paramsView(paramsView), nextOnSet{0}
{
}

/*
Scheduler::~Scheduler()
{
	freeActiveGrains();
}


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

// Realtime distribution of grains must be activated in timesequential order.
// Non realtime distribution ofgrains must be activated in random order according to the required density (nextOnset).
// generate one active grain at a time and set the inter-onset value for the next grain

void Scheduler::synthesize(AudioBlock* audioBlock, int sample, int numSamples)
{
	// si on a aucun grain alors on écrit du silence dans le buffer
	if(grains.isEmpty())
	{

		for(size_t channel = 0; channel < numChannels; channel++)
		{
			audioBlock->addSample(channel, sample, 0.f);
		}
	} else
	{
		for(Grain* grain : grains)
		{
			for(size_t channel = 0; channel < numChannels; ++channel)
			{
				// add rms here + amplitude to the grains
				float* blockPointer = audioBlock->getChannelPointer(channel);
				blockPointer[sample] += grain->getCurrentSample(channel); // add rms here
			}

			grain->update();

			if(!grain->isActive())
			{
				grains.remove(grains.indexOf(grain));
				delete grain;
				--nbActiveGrains;

				if(grains.isEmpty())
					paramsView.setIsGrainsEmpty(true);


			}
		}

	}

	if(paramsView.getIsPlaying() == true)
	{
		if(--nextOnSet == 0) // on avance à chaque sample
		{
			// TODO : récupérer les valeur random du stateParam pour les donner au grain avant de le générer.

			Grain* unGrain = generateGrain(numSamples);
			++nbActiveGrains;

			paramsView.setIsGrainsEmpty(false);


			grains.add(unGrain);
			int interOnset = paramsView.getInterOnSet(); // ajouter le random ici
			nextOnSet += interOnset; // determine le moment où prochain grain sera créer
		}
	}
	//else if (grains.isEmpty()) { // ça sonne moins réactif
	//	nextOnset = 1;
	//}
	else
	{
		nextOnSet = 1;
	}

	phaseMod.advance();

	// pour le controle de la sommation (on ne veut pas de division par zero)
	//float weight = 1.0f / static_cast<float>(nbActiveGrains + 1);

	//float linearCoef = juce::Decibels::decibelsToGain(-3.f); // logarithmique... max 1 - min 0.7
	// 0.707 = 10 ^ (-3 / 10)
					// on vérifie si il reste encore des grains sinon on met à jours le verrou de play

}

//float getWeight(const juce::AudioBuffer<float>& grain)
//{
//
//	float rms = 0.0f;
//	for (int ch = 0; ch < grain.getNumChannels(); ++ch)
//	{
//		const float* channelData = grain.getReadPointer(ch);
//		for (int i = 0; i < grain.getNumSamples(); ++i)
//		{
//			rms += channelData[i] * channelData[i];
//		}
//	}
//	rms = std::sqrt(rms / (grain.getNumChannels() * grain.getNumSamples()));
//
//	// Calculate the gain factor to prevent saturation
//	const float maxGain = 1.0f / std::sqrt(2.0f);
//	const float targetRMS = 0.707f; // Target RMS value for the overall mix
//	const float gain = std::min(maxGain, targetRMS / rms);
//
//	// Calculate the weight as the reciprocal of the gain factor
//	const float weight = 1.0f / gain;
//
//	return weight;
//}

*/

double Scheduler::getInterOnSet(float density, double sampleRate) const noexcept
{
	if(density <= 0.0) return -1.0;
	return sampleRate / (double)density;
}

void Scheduler::process(int bufferSize, double sampleRate, float density,
						std::function<void(int, const ParameterSnapshot&)> spawn, const ParameterSnapshot& parameters)
{
	const double interOnSet = getInterOnSet(density, sampleRate);
	if(interOnSet <= 1)
	{
		setOffset(0.0);
		jassertfalse;
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

/*
int Scheduler::computeEvents(int bufferSize, double sampleRate, float density, std::array<uint16_t, Param::MaxEvents>& events)
{
	const double interOnSet = getInterOnSet(density, sampleRate);
	if(interOnSet <= 1) { setOffset(0.0);	return -1; }

	int count = 0;
	double offset = getOffset(); // offset of the next outBuffer call
	while(offset < bufferSize && count < mCapacity)
	{
		events[count] = static_cast<uint16_t>(std::floor(offset));
		offset += interOnSet;
		count++;
	}

	setOffset(offset - bufferSize);
	return count;
}
*/