#include <iostream>
#include <sstream>
#include "dfnn.h"
#include "neuron.h"
#include "joelcolors.h"
#include "file_archive.h"

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
    for(Neuron& neuron : _vecNeurons)
    {
        neuron.Discharge();
        neuron.ChargeFromAccumulator();
        if(_mapOutputs.find(neuron.getID()) != _mapOutputs.end())
        {
            _mapOutputs.at(neuron.getID()).setNormalized(neuron.getCharge());
        }
    }
}

void DFNN::dbgPrint()
{

    std::cout << "DFNN with " <<  _vecNeurons.size() << " neurons.\n";
    for(Neuron& neuron : _vecNeurons)
    {
        bool input = false;
        bool output = false;
        if(_mapInputs.find(neuron.getID()) != _mapInputs.end())
        {
            input = true;
        }
        if(_mapOutputs.find(neuron.getID()) != _mapOutputs.end())
        {
            output = true;
        }

        neuron.DbgPrint(input, output);
        if(input)
        {
            NormalizedValue<double>& nVal = _mapInputs.at(neuron.getID());
            std::cout << "InputValue(" << nVal.getMin() << "," << nVal.getMax()
                      << "): " << nVal.get() << " [" << nVal.getNormalized() << "]";
        }
        if(output)
        {
            NormalizedValue<double>& nVal = _mapOutputs.at(neuron.getID());
            std::cout << "OutputValue(" << nVal.getMin() << "," << nVal.getMax()
                      << "): " << nVal.get() << " [" << nVal.getNormalized() << "]";
        }
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
    return id;
}

bool DFNN::updateInput(Handle id, double value)
{
    bool retVal = false;
    if(_mapInputs.find(id) != _mapInputs.end())
    {
        NormalizedValue<double>& nVal = _mapInputs.at(id);
        nVal.set(value);
        Neuron& n1 = *_mapNeurons.at(id);
        n1.AccumulateCharge(nVal.getNormalized());
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
    return id;
}

bool DFNN::updateOutput(Handle id, double &value)
{
    bool retVal = false;
    if(_mapOutputs.find(id) != _mapOutputs.end())
    {
        NormalizedValue<double>& nVal = _mapOutputs.at(id);
        Neuron& n1 = *_mapNeurons.at(id);
        nVal.setNormalized(n1.getCharge());
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

std::string DFNN::save(Archive * ar)
{
    std::stringstream ss;
    std::string ret;
    ss << "DFNN " << _vecNeurons.size() << " " << _mapInputs.size() << " " << _mapOutputs.size() << "\n";
    ret += ss.str();
    ret += ar->save(_vecNeurons);
    ret += ar->save(_mapInputs);
    ret += ar->save(_mapOutputs);
    return ret;
}

void DFNN::load(std::stringstream& ss, DFNN &obj, Archive * ar)
{
    size_t nbNeurons;
    size_t nbInputs;
    size_t nbOutputs;

    std::string line;
    std::getline(ss, line);
    if(line.substr(0,5) == "DFNN ")
    {
        std::stringstream converter(line.substr(4));
        converter >> nbNeurons >> nbInputs >> nbOutputs;
        _vecNeurons.reserve(nbNeurons);
        ar->load(ss, _vecNeurons, ar);
        ar->load(ss, _mapInputs, ar);
        ar->load(ss, _mapOutputs, ar);
        rebuildMap();
        for(Neuron& nrn : _vecNeurons)
        {
            nrn.reconstructPointers(this);
        }
    }
}
