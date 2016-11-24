#pragma once

#include <vector>
#include <map>
#include <random>
#include <memory>
#include "neuron.h"
#include "random_id_generator.h"

using NeuronVector = std::vector<Neuron>;
using NeuronMapByID = std::map<NeuronHandle, Neuron*>;

class DFNN : public RandomIdGenerator
{
public:
    DFNN()
    {
    }
    ~DFNN(){}

    const Neuron& getNeuron(NeuronHandle ID) const
    {
        return *_mapNeurons.at(ID);
    }

    Neuron& modifyNeuron(NeuronHandle ID)
    {
        return *_mapNeurons.at(ID);
    }

    NeuronHandle createNeuron()
    {
        _vecNeurons.emplace_back();
        NeuronHandle id = getNewID(_mapNeurons);
        _vecNeurons.back().setID(id);
        _vecNeurons.back().setIndex(_vecNeurons.size() - 1);
        _mapNeurons[id] = &_vecNeurons.back();
    }




private:

    NeuronVector _vecNeurons;
    NeuronMapByID _mapNeurons;
};
