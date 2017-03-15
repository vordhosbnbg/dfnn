#include "dfnn.h"
#include <iostream>

NeuronHandle DFNN::createNeuron()
{
    _vecNeurons.emplace_back(std::make_unique<Neuron>());
    Neuron& newNeuron = *_vecNeurons.back().get();
    NeuronHandle id = getNewID(_mapNeurons);
    newNeuron.setID(id);
    newNeuron.setIndex(_vecNeurons.size() - 1);
    _mapNeurons[id] = &newNeuron;
    return id;
}


void DFNN::PumpNetwork()
{
    #pragma omp parallel for
    for(std::unique_ptr<Neuron>& neuron : _vecNeurons)
    {
        neuron->ChargeFromAccumulator();
        neuron->Discharge();
    }
}

void DFNN::DbgPrint() const
{
    std::cout << "DFNN with " << _vecNeurons.size() << " neurons.\n";
    for(const std::unique_ptr<Neuron>& neuron : _vecNeurons)
    {
        neuron->DbgPrint();
        std::cout << "\n";
    }
    std::cout << std::endl;
}
