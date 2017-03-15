#pragma once

#include <vector>
#include <map>
#include <random>
#include <memory>
#include "neuron.h"
#include "random_id_generator.h"

using NeuronVector = std::vector<std::unique_ptr<Neuron>>;
using NeuronMapByID = std::map<NeuronHandle, Neuron *>;

class DFNN : public RandomIdGenerator
{
public:
    DFNN()
    {
    }
    ~DFNN(){}

    NeuronHandle createNeuron();

    const Neuron& getNeuron(NeuronHandle ID) const
    {
        return *_mapNeurons.at(ID);
    }

    Neuron& modifyNeuron(NeuronHandle ID)
    {
        return *_mapNeurons.at(ID);
    }

    void PumpNetwork();
    void DbgPrint() const;




private:

    NeuronVector _vecNeurons;
    NeuronMapByID _mapNeurons;
};
