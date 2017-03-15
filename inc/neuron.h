#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <iostream>
#include "joelcolors.h"

class Neuron;

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

    NeuronHandle getID()
    {
        return _id;
    }

    void setIndex(size_t index)
    {
        _index = index;
    }

    size_t getIndex()
    {
        return _index;
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

    void Discharge()
    {
        if(_outputs.size())
        {
            double biasSum = 0.0;
            for(Connection& outputConn : _outputs)
            {
                biasSum += outputConn.first;
            }

            if(biasSum != 0.0)
            {
                for(Connection& outputConn : _outputs)  // normalize bias
                {
                    outputConn.first /= biasSum;
                }
            }

            for(Connection& outputConn : _outputs) // apply current charge, using bias
            {
                double bias = outputConn.first;
                Neuron& output = *outputConn.second;
                output.AccumulateCharge(bias * _charge);
            }
            _charge = 0.0;
        }
    }

    void DbgPrint() const
    {
        Color::Modifier def(Color::FG_DEFAULT);
        Color::Modifier yel(Color::FG_YELLOW);
        Color::Modifier red(Color::FG_RED);
        Color::Modifier blu(Color::FG_BLUE);
        Color::Modifier grn(Color::FG_GREEN);

        std::cout << def << "N[" << def << _index
                  << def << "](" << def << _outputs.size()
                  << def << "){" <<  std::hex << _id
                  << def << "}\n"
                  << "outputs: \n";

        for(const Connection& conn : _outputs)
        {
            std::cout << "\t[" << conn.first << "] --> " << std::hex << conn.second->getID() << "\n";
        }
        std::cout << "Acc: " << _accumulator <<  "; Charge: " << _charge << "\n";
    }

private:

    size_t _index;
    NeuronHandle _id;
    double _accumulator;
    double _charge;

    ConnectionVector _outputs;
};
