#pragma once

#include <vector>
#include <unordered_map>
#include <random>
#include <memory>
#include "random_id_generator.h"
#include "normalized_value.h"

class Neuron;
using NeuronVector = std::vector<Neuron>;
using NeuronMapByID = std::unordered_map<Handle, Neuron *>;
using NValueMapByID = std::unordered_map<Handle, NormalizedValue<double>>;

class DFNN : public RandomIdGenerator
{
public:
    DFNN()
    {
    }
    ~DFNN(){}

    Handle createNeuron();

    Neuron& getNeuron(Handle id)
    {
        return *_mapNeurons.at(id);
    }

    Neuron& getNeuronByIndex(size_t index)
    {
        return _vecNeurons[index];
    }

    bool existsNeuron(Handle id)
    {
        return(_mapNeurons.find(id) != _mapNeurons.end());
    }
    bool removeNeuron(Handle id);
    void pumpNetwork();

    Handle createInput(double min, double max);
    bool updateInput(Handle id, double value);
    bool removeInput(Handle id);

    Handle createOutput(double min, double max);
    bool updateOutput(Handle id, double& value);
    bool removeOutput(Handle id);

    void dbgPrint();



private:
    void rebuildMap();
    NeuronVector _vecNeurons;
    NeuronMapByID _mapNeurons;
    NValueMapByID _mapInputs;
    NValueMapByID _mapOutputs;
};
