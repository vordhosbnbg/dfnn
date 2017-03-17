#include <iostream>
#include "dfnn.h"
#include "neuron.h"
#include "joelcolors.h"

Handle DFNN::createNeuron()
{
    _vecNeurons.emplace_back();
    Neuron& newNeuron = _vecNeurons.back();
    Handle id = getNewID(_mapNeurons);
    newNeuron.setID(id);
    newNeuron.setIndex(_vecNeurons.size() - 1);
    RebuildMap();
    return id;
}

bool DFNN::removeNeuron(Handle ID)
{
    bool retVal = false;
    for(const Neuron& neuron : _vecNeurons)
    {
        if(neuron.getID() == ID)
        {
            _vecNeurons.erase(_vecNeurons.begin() + neuron.getIndex());
            RebuildMap();
            retVal = true;
            break;
        }
    }

    return retVal;
}

void DFNN::pumpNetwork()
{
    #pragma omp parallel for
    for(Neuron& neuron : _vecNeurons)
    {
        neuron.Discharge();
        neuron.ChargeFromAccumulator();
    }
}

void DFNN::dbgPrint() const
{

    std::cout << "DFNN with " <<  _vecNeurons.size() << " neurons.\n";
    for(const Neuron& neuron : _vecNeurons)
    {
        neuron.DbgPrint();
        std::cout << "\n";
    }
    std::cout << std::endl;
}

void DFNN::RebuildMap()
{
    _mapNeurons.clear();
    for(Neuron& neuron : _vecNeurons)
    {
        _mapNeurons.insert(std::make_pair(neuron.getID(), &neuron));
    }
}
