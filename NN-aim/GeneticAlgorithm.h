#ifndef GENETIC_ALGORITHM_H
#define GENETIC_ALGORITHM_H

const int POPULATION_SIZE = 30;

#include <vector>
#include <cstdlib>


struct Chromosome
{
	std::vector<float> weights;
	int fitness;

	Chromosome(){fitness = 0;};

	bool operator < (const Chromosome& other) const;
};


class GeneticAlgorithm
{
public:
	GeneticAlgorithm(size_t chromosomeSize);

	GeneticAlgorithm(const GeneticAlgorithm& other) = delete;
	GeneticAlgorithm& operator = (const GeneticAlgorithm& other) = delete;

	~GeneticAlgorithm(){};

	std::vector<Chromosome>& GetGeneration();
	void AddChromosomeFitness(size_t chromosomeIndex, int fitness);
	void NextGenetarion();

private:
	size_t Select() const;
	void Mutate(size_t index);
	void Crossover(size_t parentIndex1, size_t parentIndex2, Chromosome& child1, Chromosome& child2)const;
	void CrossoverOneChild(size_t parentIndex1, size_t parentIndex2, Chromosome& child)const;

	std::vector<Chromosome> currentGeneration;
	std::vector<int> partialSums;

	size_t chromosomeSize;
	size_t generationNumber;
};

double generateGaussianNoise(double mu, double sigma);

#endif
