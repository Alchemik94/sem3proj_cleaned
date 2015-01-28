#pragma once

#define ABS(x) (x) >= 0 ? (x) : (-(x))
#define PI 3.14159265

#include "Neuron.h"
#include <list>
#include <math.h>
#include <time.h>
#include <vector>
#include <string>
#include <fstream>

namespace NeuralNetwork
{
	template <typename FloatingNumber = long double>
	class NeuralNetwork
	{
		std::vector<std::vector<Neuron<FloatingNumber> > > network;

		const FloatingNumber learningCoefficientLow = 0.05;
		const FloatingNumber learningCoefficientHigh = 0.9;

		FloatingNumber ParticularMeanSquaredError(const FloatingNumber& given, const FloatingNumber& expected) const
		{
			return ((FloatingNumber)(0.5))*((FloatingNumber)(given - expected))*((FloatingNumber)(given - expected));
		}

		FloatingNumber MeanSquaredError(std::list<std::pair<std::vector<FloatingNumber>, std::vector<FloatingNumber> > >& checkingSet)
		{
			FloatingNumber res = 0;

			for (auto it = checkingSet.begin(); it != checkingSet.end(); ++it)
			{
				Use(it->first);
				for (int i = 0; i < it->second.size(); ++i)
				{
					res += ParticularMeanSquaredError(network[network.size() - 1][i].lastResult, it->second[i]);
				}
			}

			res /= checkingSet.size();

			return res;
		}

		FloatingNumber ParticularError(const FloatingNumber& given, const FloatingNumber& expected) const
		{
			return (FloatingNumber)(expected - given);
		}

		FloatingNumber LearningCoefficientForIthEpoch(const int& currentEpoch, const int& maxEpochs) const
		{
			return std::sqrt(learningCoefficientLow*learningCoefficientLow*(ABS(std::sinl(((FloatingNumber)(currentEpoch) / (FloatingNumber)(maxEpochs)) * PI - PI / 2))) + learningCoefficientHigh*learningCoefficientHigh*(ABS(std::cosl(((FloatingNumber)(currentEpoch) / (FloatingNumber)(maxEpochs)) * PI - PI / 2))));
		}

		FloatingNumber ErrorOnHidden(const int& layer, const int& neuronNumber, const std::vector<std::vector<FloatingNumber> >& error)
		{
			FloatingNumber result = 0;
			for (int i = 0; i < network[layer + 1].size(); ++i)
				result += (error[layer + 1][i] * network[layer + 1][i].wages[neuronNumber]);
			return result;
		}

		std::vector<std::vector<FloatingNumber> > ErrorValues(const std::vector<FloatingNumber>& input, const std::vector<FloatingNumber>& output)
		{
			std::vector<std::vector<FloatingNumber> > error(network.size());

			//output layer
			error[error.size() - 1].resize(network[network.size() - 1].size());
			for (int i = 0; i < network[network.size() - 1].size(); ++i)
				error[error.size() - 1][i] = ParticularError(network[network.size() - 1][i].lastResult, output[i]);

			//rest of the layers
			for (int i = network.size() - 2; i >= 0; --i)
			{
				error[i].resize(network[i].size());

				//error for each neuron
				for (int j = 0; j < network[i].size(); ++j)
					error[i][j] = ErrorOnHidden(i, j, error);

			}
			return error;
		}

		void TeachCase(const std::vector<FloatingNumber>& input, const std::vector<FloatingNumber>& output, const FloatingNumber& learningCoefficient)
		{
			Use(input);
			std::vector<std::vector<FloatingNumber> > error = ErrorValues(input, output);
			std::vector<FloatingNumber> currentInput = input;

			for (int i = 0; i < network.size(); ++i)
			{
				for (int j = 0; j < network[i].size(); ++j)
					network[i][j].Teach(error[i][j], learningCoefficient, currentInput);
				currentInput.clear();
				for (int j = 0; j < network[i].size(); ++j)
					currentInput.push_back(network[i][j].lastResult);
			}
		}

	public:
		//Network has to have at least three layers: input, hidden and output
		NeuralNetwork(const std::vector<int>& structureDescription)
		{
			std::srand(time(NULL));
			if (structureDescription.size() < 2)
				throw;
			network.clear();
			network.push_back(std::vector<Neuron<FloatingNumber> >(structureDescription[0]));
			for (int i = 0; i < network[0].size(); ++i)
			{
				network[0][i] = Neuron<FloatingNumber>(1);
				network[0][i].wages[0] = ABS(network[0][i].wages[0]);
			}
			for (int i = 1; i < structureDescription.size() - 1; ++i)
			{
				network.push_back(std::vector<Neuron<FloatingNumber> >(structureDescription[i] + 1));
				for (int j = 0; j < network[i].size(); ++j)
					network[i][j] = Neuron<FloatingNumber>(network[i - 1].size());
				network[i][structureDescription[i]].type = Bias;
			}
			network.push_back(std::vector<Neuron<FloatingNumber> >(structureDescription[structureDescription.size() - 1]));
			for (int i = 0; i < network[network.size() - 1].size(); ++i)
				network[network.size() - 1][i] = Neuron<FloatingNumber>(structureDescription[structureDescription.size() - 2] + 1);
		}

		//Constructing from file
		NeuralNetwork(const std::string& filename)
		{
			std::srand(time(NULL));
			std::fstream in;
			in.open(filename.c_str(), std::ios_base::in);

			int tmp;
			in >> tmp;
			network = std::vector<std::vector<Neuron<FloatingNumber> > >(tmp, std::vector<Neuron<FloatingNumber> >());

			//for every layer
			for (int i = 0; i < network.size(); ++i)
			{
				//number of neurons
				in >> tmp;
				network[i].resize(tmp);
				//for every neuron
				for (int j = 0; j < network[i].size(); ++j)
				{
					//number of wages
					in >> tmp;
					network[i][j].wages.resize(tmp);
					//every wage
					for (int k = 0; k < network[i][j].wages.size(); ++k)
					{
						in >> network[i][j].wages[k];
					}
					//and type of activation function
					in >> tmp;
					network[i][j].type = (ActivationFunction)tmp;
				}
			}

			in.close();
		}

		std::vector<FloatingNumber> Use(const std::vector<FloatingNumber>& input)
		{
			std::vector<FloatingNumber> tmp, result;
			tmp = input;
			for (int i = 0; i < 1; ++i)
			{
				result.clear();
				result.resize(network[i].size());
				for (int j = 0; j < network[i].size(); ++j)
					result[j] = network[i][j].Use(std::vector<FloatingNumber>(1, tmp[j]));
				tmp = result;
			}
			for (int i = 1; i < network.size(); ++i)
			{
				result.clear();
				result.resize(network[i].size());
				for (int j = 0; j < network[i].size(); ++j)
					result[j] = network[i][j].Use(tmp);
				tmp = result;
			}
			return result;
		}

		void Teach(std::list<std::pair<std::vector<FloatingNumber>, std::vector<FloatingNumber> > >& teachingSet, int epochs)
		{
			auto it = teachingSet.begin();
			FloatingNumber learningCoefficient;
			for (int i = 0; i < epochs; ++i)
			{
				int j = 0;
				learningCoefficient = LearningCoefficientForIthEpoch(i, epochs);
				for (it = teachingSet.begin(); it != teachingSet.end(); ++it)
				{
					TeachCase(it->first, it->second, learningCoefficient);
				}
			}
		}

		void Teach(std::list<std::pair<std::vector<FloatingNumber>, std::vector<FloatingNumber> > >& teachingSet, int epochs, int whenReport)

		{
			std::cout.precision(10);
			auto it = teachingSet.begin();
			FloatingNumber learningCoefficient;
			std::cout << "Epochs:\t\tCurrent Error : \n";
			for (int i = 0; i < epochs; ++i)
			{
				int j = 0;
				if ((i + 1 == 1) || ((i + 1) % whenReport == 0))
				{
					std::cout << i + 1 << "\t\t" << MeanSquaredError(teachingSet) << "\n";
				}
				learningCoefficient = LearningCoefficientForIthEpoch(i, epochs);
				for (it = teachingSet.begin(); it != teachingSet.end(); ++it)
				{
					TeachCase(it->first, it->second, learningCoefficient);
				}
			}
		}

		void Teach(std::list<std::pair<std::vector<FloatingNumber>, std::vector<FloatingNumber> > >& teachingSet, FloatingNumber error, int whenReport)

		{
			std::cout.precision(10);
			long long int epochs = 20000;
			long long int i = 0;
			auto it = teachingSet.begin();
			FloatingNumber learningCoefficient;
			std::cout << "Epochs:\t\tCurrent Error : \n";
			while (MeanSquaredError(teachingSet) > error)
			{
				int j = 0;
				if ((i + 1 == 1) || ((i + 1) % whenReport == 0))
				{
					std::cout << i + 1 << "\t\t" << MeanSquaredError(teachingSet) << "\n";
				}
				learningCoefficient = LearningCoefficientForIthEpoch(i, epochs);
				for (it = teachingSet.begin(); it != teachingSet.end(); ++it)
				{
					TeachCase(it->first, it->second, learningCoefficient);
				}
				++i;
				if (4 * i >= 3 * epochs)
					epochs *= 2;
			}
		}

		void Save(const std::string& filename)
		{
			std::fstream out;
			out.open(filename.c_str(), std::ios_base::out);

			out << network.size() << "\n";

			//for every layer
			for (int i = 0; i < network.size(); ++i)
			{
				out << network[i].size() << "\n";
				//for every neuron
				for (int j = 0; j < network[i].size(); ++j)
				{
					//number of wages
					out << network[i][j].wages.size() << "\n";
					//every wage
					for (int k = 0; k < network[i][j].wages.size(); ++k)
					{
						out << network[i][j].wages[k] << " ";
					}
					out << "\n";
					//and type of activation function
					out << (int)network[i][j].type << "\n";
				}
			}

			out.close();
		}

		typedef std::list<std::pair<std::vector<FloatingNumber>, std::vector<FloatingNumber> > > TeachingSet;
	};
}

#undef ABS
#undef PI