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

    bool existsNeuron(Handle ID)
    {
        return(_mapNeurons.find(ID) != _mapNeurons.end());
    }
    bool removeNeuron(Handle ID)
    {
        bool retVal = false;
        NeuronMapByID::iterator mapIt = _mapNeurons.find(ID);
        if(mapIt != _mapNeurons.end())
        {
            _mapNeurons.erase(mapIt);
            retVal = true;
        }

        return retVal;
    }

    Neuron& modifyNeuron(Handle ID)
    {
        return *_mapNeurons.at(ID);
    }

    void pumpNetwork();
    void dbgPrint() const;




private:

    NeuronVector _vecNeurons;
    NeuronMapByID _mapNeurons;
};
