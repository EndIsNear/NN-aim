#include "NeuralNetwork.h"
#include <cstdlib>
#include <cmath>
#include <limits>

Neuron::Neuron(int inputs): m_inputs(inputs + 1) // plus 1 for the bias
{
	m_weights.resize(m_inputs);
}

NeuronLayer::NeuronLayer()
{
	m_numNeurons = 0;
}

NeuronLayer::NeuronLayer(int numNeurons, int inputsPerNeuron)
{
	m_numNeurons = numNeurons;
	for(int i = 0; i < m_numNeurons; ++i)
	{
		m_neurons.push_back(Neuron(inputsPerNeuron));
	}
}

NeuralNetwork::NeuralNetwork(): m_weightsCount(0)
{
	m_layers[0] = NeuronLayer(neuronsPerLayer, layer1InputsPerNeuron);
	m_layers[1] = NeuronLayer(neuronsPerLayer, layer2InputsPerNeuron);
	m_layers[2] = NeuronLayer(neuronsPerLayer, layer2InputsPerNeuron);

	m_weightsCount += m_layers[0].m_numNeurons * m_layers[0].m_neurons[0].m_inputs;
	m_weightsCount += m_layers[1].m_numNeurons * m_layers[1].m_neurons[0].m_inputs;
	m_weightsCount += m_layers[2].m_numNeurons * m_layers[2].m_neurons[0].m_inputs;
}

float NeuralNetwork::sigmoid(float activation) const
{
	return 1.f / (1.f + exp(-activation));
}

bool NeuralNetwork::castToBool(float activation) const
{
	return activation >= 0.5f ? true : false;
}

Move NeuralNetwork::calculateMove(bool inFov, bool bulletInFov, bool canShoot, float fov)
{
	Move move{0};

	std::vector<float> inputs = {static_cast<float>(inFov), static_cast<float>(bulletInFov),
			static_cast<float>(canShoot),fov};

	std::vector<float> activations(neuronsPerLayer);

	for(auto i = 0; i < inputs.size() - 1; ++i)
	{
		inputs[i] = inputs[i] ? 1.f : -1.f;
	}

	for(const auto& layer: m_layers)
	{
		int neuronIndex = 0;
		std::fill(activations.begin(), activations.end(), 0.f);

		for(const auto& neuron: layer.m_neurons)
		{
			auto neuronInputs = neuron.m_inputs;
			for(int inputIndex = 0; inputIndex < neuronInputs - 1; ++inputIndex)
			{
				activations[neuronIndex] += inputs[inputIndex] * neuron.m_weights[inputIndex];
			}
			activations[neuronIndex] += (-1.f) * neuron.m_weights[neuronInputs - 1]; //bias

			activations[neuronIndex] = sigmoid(activations[neuronIndex]);

			++neuronIndex;
		}
		inputs = activations;
	}

	move.advanceStraight = castToBool(activations[0]);
	move.turnLeft = castToBool(activations[1]);
	move.turnRight = castToBool(activations[2]);
	move.shoot = castToBool(activations[3]);
	/* Activation[i] is in range[0,1]
	 * but fov can be 1.5 max
	 * So we add 0.5 to it
	 * */
	move.fovMult = activations[4] + 0.5f;

	return move;
}

int NeuralNetwork::weightsCount() const
{
	return m_weightsCount;
}

std::vector<float> NeuralNetwork::getWeights() const
{
	std::vector<float> weights;

	for(const auto& layer: m_layers)
	{
		for(const auto& neuron: layer.m_neurons)
		{
			weights.insert(weights.end(), neuron.m_weights.begin(), neuron.m_weights.end());
		}
	}

	return weights;
}

void NeuralNetwork::setWeights(const std::vector<float>& newWeights)
{
	auto start = newWeights.begin();
	for(auto& layer: m_layers)
	{
		for(auto& neuron: layer.m_neurons)
		{
			auto end = start + neuron.m_inputs;
			neuron.m_weights = std::vector<float>(start, end);
			start = end;
		}
	}
}
