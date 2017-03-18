#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include "joelcolors.h"

class Neuron;
class Archive;
class DFNN;
using NeuronHandle = unsigned int;
using NeuronPtr = Neuron*;
using Connection = std::pair<double, NeuronPtr>;
using ConnectionVector = std::vector<Connection>;



class Neuron
{
public:
    Neuron() : _accumulator(0.0), _charge(0.0)
    {}
    ~Neuron()
    {}

    void setID(NeuronHandle id)
    {
        _id = id;
    }

    NeuronHandle getID() const
    {
        return _id;
    }

    void setIndex(size_t index)
    {
        _index = index;
    }

    size_t getIndex() const
    {
        return _index;
    }

    double getCharge()
    {
        double retVal = _charge;
        _charge = 0.0;
        return retVal;
    }

    void ConnectOutput(double bias, Neuron& output)
    {
        _outputs.emplace_back(std::make_pair(bias,&output));
    }

    void ChargeFromAccumulator()
    {
        _charge += _accumulator;
        _accumulator = 0.0;
    }

    void AccumulateCharge(double chargeAmmount)
    {
        _accumulator += chargeAmmount;
    }

    double getAccumulator() const
    {
        return _accumulator;
    }

    void Discharge();
    void DbgPrint(bool input = false, bool output = false) const;
    std::string save(Archive *ar);
    void load(std::stringstream& ss, Neuron& obj, Archive *ar);
    void reconstructPointers(DFNN* parentNetwork);

private:

    size_t _index;
    NeuronHandle _id;
    double _accumulator;
    double _charge;

    ConnectionVector _outputs;
};
