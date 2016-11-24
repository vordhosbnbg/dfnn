#pragma once
#include <memory>
#include <random>
#include "neuron.h"

using UniformDistributionNeuronHandles = std::uniform_int_distribution<NeuronHandle>;


class RandomIdGenerator
{
public:
    RandomIdGenerator() : _engRandom(_rnd()), _uniform_dist(0, UINT32_MAX)
    {

    }
    ~RandomIdGenerator(){}

protected:
    template<class MapT>
    NeuronHandle getNewID(const MapT& keyMap)
    {
        NeuronHandle candidate;
        do
        {
            candidate = _uniform_dist(_engRandom);
        }
        while(keyMap.find(candidate) != keyMap.end());
        return candidate;
    }

private:
    std::random_device _rnd;
    std::mt19937_64 _engRandom;
    UniformDistributionNeuronHandles _uniform_dist;

};
