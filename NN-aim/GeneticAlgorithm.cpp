#include "GeneticAlgorithm.h"
#include "RandomGenerator.h"

#include <cmath>
#include <random>
#include <limits>
#include <algorithm>

const float mu = 0.1;
const float sigma = 0.2;
const int maxMutations = 1;
const int maxCromosomesToMutate = 0.1 * POPULATION_SIZE;
const float acceptableDif = 0.3;

bool Chromosome::operator < (const Chromosome& other) const
{
	return this->fitness > other.fitness;
}


GeneticAlgorithm::GeneticAlgorithm(size_t chromosomeSize)
{
	this->chromosomeSize = chromosomeSize;

	this->currentGeneration.resize(POPULATION_SIZE);
	this->partialSums.resize(POPULATION_SIZE);

	for(size_t i = 0; i < POPULATION_SIZE; ++i)
	{
		this->currentGeneration[i].weights = std::vector<float>(this->chromosomeSize);

		for(size_t j = 0; j < chromosomeSize; ++j)
		{
			this->currentGeneration[i].weights[j] =  RandomGen::getInstance().uniformDouble(-1.f, 1.f);
		}
	}

	this->generationNumber = 1;
}


std::vector<Chromosome>& GeneticAlgorithm::GetGeneration()
{
	return this->currentGeneration;
}


void GeneticAlgorithm::AddChromosomeFitness(size_t chromosomeIndex, int fitness)
{
	this->currentGeneration[chromosomeIndex].fitness += fitness;
}


void GeneticAlgorithm::NextGenetarion()
{
	sort(this->currentGeneration.begin(), this->currentGeneration.end());

	this->partialSums[0] = this->currentGeneration[0].fitness;

	for (size_t i = 1; i < POPULATION_SIZE; ++i)
	{
		 this->partialSums[i] = this->partialSums[i - 1] + this->currentGeneration[i].fitness;
	}

	std::vector<Chromosome> newGeneration(POPULATION_SIZE);

	for(size_t i = 0; i < POPULATION_SIZE; ++i)
	{
		newGeneration[i].weights.resize(this->chromosomeSize);
	}

	for(size_t i = 0; i < POPULATION_SIZE; i += 2)
	{
		this->Crossover(this->Select(), this->Select(), newGeneration[i], newGeneration[i + 1]);
//		size_t pIdx1 = this->Select();
//		size_t pIdx2 = this->Select();
//		this->CrossoverOneChild(pIdx1, pIdx2, newGeneration[i]);
//		this->CrossoverOneChild(pIdx2, pIdx1, newGeneration[i + 1]);
	}

	this->currentGeneration = newGeneration;

	size_t numChromosomesToMutate = RandomGen::getInstance().intInRange(0, maxCromosomesToMutate);

	for(size_t i = 0; i < numChromosomesToMutate; ++i)
	{
		this->Mutate(RandomGen::getInstance().intInRange(0, POPULATION_SIZE - 1));
	}

	++(this->generationNumber);
}


size_t GeneticAlgorithm::Select() const
{
	 int x = RandomGen::getInstance().intInRange(0, this->partialSums[POPULATION_SIZE - 1]);

	 int res = 0;

	 for(int i = 0; i < POPULATION_SIZE && this->partialSums[i] < x; ++i)
	 {
	    res = i;
	 }

	 return res;
}


void GeneticAlgorithm::Mutate(size_t index)
{
	size_t numPositionsToMutate = RandomGen::getInstance().intInRange(0, maxMutations);
	size_t currentPosition;

	for(size_t i = 0; i < numPositionsToMutate; ++i)
	{
		currentPosition = RandomGen::getInstance().intInRange(0, this->chromosomeSize - 1);
		this->currentGeneration[index].weights[currentPosition] +=
				RandomGen::getInstance().gaussian(mu, sigma);
	}
}


void GeneticAlgorithm::Crossover(size_t parentIndex1, size_t parentIndex2, Chromosome& child1,
								 Chromosome& child2) const
{
	size_t crosspoint1, crosspoint2;

	crosspoint1 = RandomGen::getInstance().intInRange(0, this->chromosomeSize - 1);

	do{
		crosspoint2 = RandomGen::getInstance().intInRange(0, this->chromosomeSize - 1);
	}while(crosspoint1 == crosspoint2);

	if(crosspoint1 > crosspoint2)
	{
		std::swap(crosspoint1, crosspoint2);
	}

	for(size_t i = 0; i < crosspoint1; ++i)
	{
		child1.weights[i] = this->currentGeneration[parentIndex1].weights[i];
		child2.weights[i] = this->currentGeneration[parentIndex2].weights[i];
	}

	for(size_t i = crosspoint1; i < crosspoint2; ++i)
	{
		child1.weights[i] = this->currentGeneration[parentIndex2].weights[i];
		child2.weights[i] = this->currentGeneration[parentIndex1].weights[i];
	}

	for(size_t i = crosspoint1; i < this->chromosomeSize; ++i)
	{
		child1.weights[i] = this->currentGeneration[parentIndex1].weights[i];
		child2.weights[i] = this->currentGeneration[parentIndex2].weights[i];
	}
}


void GeneticAlgorithm::CrossoverOneChild(size_t parentIndex1, size_t parentIndex2, Chromosome& child) const
{
	Chromosome p1 = this->currentGeneration[parentIndex1];
	Chromosome p2 = this->currentGeneration[parentIndex2];

	for(size_t i = 0; i < this->chromosomeSize; ++i)
	{
		if(fabs(p1.weights[i] - p2.weights[i]) > acceptableDif) child.weights[i] = p1.weights[i];
		else
		{
			if(RandomGen::getInstance().intInRange(0,1))
				child.weights[i] = 0.5 * (p1.weights[i] + p2.weights[i]);

			else child.weights[i] = p2.weights[i];
		}
	}

}
