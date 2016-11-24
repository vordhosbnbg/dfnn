#pragma once
#include <vector>
#include <memory>

class Neuron;

using NeuronHandle = unsigned int;
using NeuronIndex = size_t;
using NeuronPtr = Neuron*;
using NeuronPtrVector = std::vector<NeuronPtr>;

class Neuron
{
public:
    Neuron()
    {}
    ~Neuron()
    {}

    void setID(NeuronHandle id)
    {
        _id = id;
    }

    NeuronHandle getID()
    {
        return _id;
    }

    void setIndex(NeuronIndex index)
    {
        _index = index;
    }

    NeuronIndex getIndex()
    {
        return _index;
    }

    void ConnectInput(Neuron& input)
    {
        _inputs.emplace_back(&input);
    }

    void ConnectOutput(Neuron& output)
    {
        _outputs.emplace_back(&output);
    }


private:

    NeuronIndex _index;
    NeuronHandle _id;
    double _charge;
    NeuronPtrVector _inputs;
    NeuronPtrVector _outputs;
};
