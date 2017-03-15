#include <iostream>
#include "dfnn.h"
#include "neuron.h"
#include "joelcolors.h"

Handle DFNN::createNeuron()
{
    _vecNeurons.emplace_back(std::make_unique<Neuron>());
    Neuron& newNeuron = *_vecNeurons.back().get();
    Handle id = getNewID(_mapNeurons);
    newNeuron.setID(id);
    newNeuron.setIndex(_vecNeurons.size() - 1);
    _mapNeurons[id] = &newNeuron;
    return id;
}


void DFNN::pumpNetwork()
{
    #pragma omp parallel for
    for(std::unique_ptr<Neuron>& neuron : _vecNeurons)
    {
        neuron->ChargeFromAccumulator();
        neuron->Discharge();
    }
}

void DFNN::dbgPrint() const
{
    Color::Modifier yellow(Color::FG_YELLOW);
    Color::Modifier def(Color::FG_DEFAULT);

    std::cout << "DFNN with " << yellow << _vecNeurons.size() << def << " neurons.\n";
    for(const std::unique_ptr<Neuron>& neuron : _vecNeurons)
    {
        neuron->DbgPrint();
        std::cout << "\n";
    }
    std::cout << std::endl;
}
