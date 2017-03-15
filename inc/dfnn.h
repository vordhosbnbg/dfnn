#pragma once

#include <vector>
#include <map>
#include <random>
#include <memory>
#include "random_id_generator.h"

class Neuron;
using NeuronVector = std::vector<std::unique_ptr<Neuron>>;
using NeuronMapByID = std::map<Handle, Neuron *>;

class DFNN : public RandomIdGenerator
{
public:
    DFNN()
    {
    }
    ~DFNN(){}

    Handle createNeuron();

    const Neuron& getNeuron(Handle ID) const
    {
        return *_mapNeurons.at(ID);
    }

    Neuron& modifyNeuron(Handle ID)
    {
        return *_mapNeurons.at(ID);
    }

    void PumpNetwork();
    void DbgPrint() const;




private:

    NeuronVector _vecNeurons;
    NeuronMapByID _mapNeurons;
};
