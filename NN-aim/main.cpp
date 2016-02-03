#if _MSC_VER >= 1900
	#include <cstdio>
	extern "C" { FILE __iob_func[3] = { *stdin, *stdout, *stderr }; }
#endif

#include <iostream>
#include <ctime>
#include <algorithm>

#include "GeneticAlgorithm.h"
#include "Game.h"
#include "NeuralNetwork.h"
#include "RandomGenerator.h"

#include <thread>

inline void setFitness(std::vector<Game>& games, GeneticAlgorithm& genAlgo)
{
	for (int i = 0; i < games.size(); ++i)
	{
		int left = 0, right = 0;
		games[i].getNNRating(left, right);
		games[i].reset();
		genAlgo.AddChromosomeFitness(games[i].getPlayerIndex(0), left);
		genAlgo.AddChromosomeFitness(games[i].getPlayerIndex(1), right);
	}
}
//makes move on every game
inline void makeMove(std::vector<Game>& games)
{
	for (int i = 0; i < games.size(); ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			games[i].makeMove(j);
			games[i].move();
		}
	}
}

void play(int startIndex, GeneticAlgorithm& genAlgo, SDLWrapper& sdl, std::vector<Game>& games)
{
	int populationSize = POPULATION_SIZE;
	int playersCount = populationSize - startIndex;
	int gamesCount = (playersCount * (playersCount - 1)) / 2;
	auto& population = genAlgo.GetGeneration();

	games.resize(gamesCount);

	int leftPlayerIndex = startIndex;
	int rightPlayerIndex = leftPlayerIndex;
	for(int i = 0; i < games.size(); ++i)
	{
		++rightPlayerIndex;
		if(rightPlayerIndex > populationSize - 1)
		{
			++leftPlayerIndex;
			rightPlayerIndex = leftPlayerIndex + 1;
		}

		games[i].init(&sdl);
		games[i].setPlayerIndex(0, leftPlayerIndex);
		games[i].setPlayerIndex(1, rightPlayerIndex);
		games[i].setNetworkWeights(0, population[leftPlayerIndex].weights);
		games[i].setNetworkWeights(1, population[rightPlayerIndex].weights);
	}

	while(!games[0].end())
	{
		makeMove(games);
	}
}

void displayGame(NeuralNetwork nets[2], SDLWrapper& sdl, int framesCount)
{
	Game game(framesCount);
	game.init(&sdl);
	while(!sdl.quit() && !game.end())
	{
		sdl.checkForEvent();
		SDL_Delay(3);
		game.draw();

		Move move;

		for(int i = 0; i < 2; ++i)
		{
			game.makeMove(i);
			game.move();
		}
	}

}

int main(int argc, char * argv[])
{
	SDLWrapper sdl(width, height);
	if (!sdl.initSDL())
	{
		return -1;
	}

	GeneticAlgorithm genAlgo(NeuralNetwork().weightsCount());
	auto& nnVals = genAlgo.GetGeneration();

	int iteration = 0;
	char iterChar[64];

	NeuralNetwork displayGameNets[2];

	int player1Index = RandomGen::getInstance().intInRange(0, POPULATION_SIZE - 1);
	int player2Index = RandomGen::getInstance().intInRange(0, POPULATION_SIZE - 1);
	while(player1Index == player2Index)
	{
		player2Index = RandomGen::getInstance().intInRange(0, POPULATION_SIZE - 1);
	}
	auto firstBestWeights = nnVals[player1Index].weights;
	auto secondBestWeights = nnVals[player2Index].weights;

	while (!sdl.quit())
	{
		sprintf(iterChar, "NN-Aim :) Iter: %d", iteration);
		sdl.setWinTitle(iterChar);

		displayGameNets[0].setWeights(firstBestWeights);
		displayGameNets[1].setWeights(secondBestWeights);

		std::vector<Game> gamesInfo;
		std::thread game(play, 0, std::ref(genAlgo), std::ref(sdl), std::ref(gamesInfo));

		displayGame(displayGameNets, sdl, 10000);

		game.join();
		setFitness(gamesInfo, genAlgo);

		std::sort(nnVals.begin(), nnVals.end());
		firstBestWeights = nnVals[0].weights;
		secondBestWeights = nnVals[1].weights;

		genAlgo.NextGenetarion();
		iteration++;
	}

	return 0;
}
