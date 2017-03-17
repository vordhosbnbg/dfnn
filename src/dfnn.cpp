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
    rebuildMap();
    return id;
}

bool DFNN::removeNeuron(Handle id)
{
    bool retVal = false;
    if(_mapNeurons.find(id) != _mapNeurons.end())
    {
        const Neuron& neuron = *_mapNeurons[id];
        if(neuron.getID() == id)
        {
            _vecNeurons.erase(_vecNeurons.begin() + neuron.getIndex());
            rebuildMap();
            retVal = true;
        }
        if(_mapInputs.find(id) != _mapInputs.end())
        {
            _mapInputs.erase(id);
        }
        if(_mapOutputs.find(id) != _mapOutputs.end())
        {
            _mapOutputs.erase(id);
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

void DFNN::rebuildMap()
{
    _mapNeurons.clear();
    for(Neuron& neuron : _vecNeurons)
    {
        _mapNeurons.insert(std::make_pair(neuron.getID(), &neuron));
    }
}
Handle DFNN::createInput(double min, double max)
{
    Handle id = createNeuron();
    _mapInputs.insert(std::pair<Handle,NormalizedValue<double>>(id, NormalizedValue<double>(min,max)));
}

bool DFNN::updateInput(Handle id, double value)
{
    bool retVal = false;
    if(_mapInputs.find(id) != _mapInputs.end())
    {
        NormalizedValue<double>& nVal = _mapInputs.at(id);
        nVal.set(value);
        retVal = true;
    }
    return retVal;
}

bool DFNN::removeInput(Handle id)
{
    bool retVal = false;
    if(_mapInputs.find(id) != _mapInputs.end())
    {
        removeNeuron(id);
        _mapInputs.erase(id);
        retVal = true;
    }
    return retVal;
}

Handle DFNN::createOutput(double min, double max)
{
    Handle id = createNeuron();
    _mapOutputs.insert(std::pair<Handle,NormalizedValue<double>>(id, NormalizedValue<double>(min,max)));
}

bool DFNN::updateOutput(Handle id, double &value)
{
    bool retVal = false;
    if(_mapOutputs.find(id) != _mapOutputs.end())
    {
        NormalizedValue<double>& nVal = _mapOutputs.at(id);
        value = nVal.get();
        retVal = true;
    }
    return retVal;
}

bool DFNN::removeOutput(Handle id)
{
    bool retVal = false;
    if(_mapOutputs.find(id) != _mapOutputs.end())
    {
        removeNeuron(id);
        _mapOutputs.erase(id);
        retVal = true;
    }
    return retVal;
}
