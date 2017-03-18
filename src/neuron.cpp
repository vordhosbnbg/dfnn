#include "neuron.h"
#include "file_archive.h"
#include "dfnn.h"
#include <sstream>

void Neuron::Discharge()
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

void Neuron::DbgPrint(bool input, bool output) const
{
    std::string type;
    if(input)
    {
        type = "<input>";
    }
    if(output)
    {
        type = "<output>";
    }
    std::cout << "N[" << _index << "](" << _outputs.size() << "){" <<  std::hex << _id
              << "} "<< type << "\n" << "connections: \n";

    for(const Connection& conn : _outputs)
    {
        std::cout << "\t[" << conn.first << "] --> " << std::hex << conn.second->getID() << "\n";
    }
    std::cout << "Acc: " << _accumulator <<  "; Charge: " << _charge << "\n";
}

std::string Neuron::save(Archive * ar)
{
    std::stringstream ss;
    std::string ret;
    ss << "NRN " <<  _outputs.size() << "\n";
    ret += ss.str();
    ret += ar->save(_index);
    ret += ar->save(_id);
    ret += ar->save(_accumulator);
    ret += ar->save(_charge);
    std::vector<std::pair<double, NeuronHandle>> tempVec;
    for(const std::pair<double, Neuron*>& item : _outputs)
    {
        tempVec.emplace_back(std::make_pair(item.first, item.second->getID()));
    }
    ret += ar->save(tempVec);

    return ret;
}
void Neuron::load(std::stringstream& ss, Neuron &obj, Archive *ar)
{
    std::string line;
    std::getline(ss, line);
    if(line.substr(0,4) == "NRN ")
    {
        ar->load(ss, _index, ar);
        ar->load(ss, _id, ar);
        ar->load(ss, _accumulator, ar);
        ar->load(ss, _charge, ar);
        std::vector<std::pair<double, NeuronHandle>> tempVec;
        ar->load(ss, tempVec, ar);
        for(const std::pair<double, NeuronHandle> pair : tempVec)
        {
            _outputs.emplace_back(std::make_pair(pair.first, reinterpret_cast<Neuron*>(pair.second)));
        }
    }
}

void Neuron::reconstructPointers(DFNN* parentNetwork)
{
    for(std::pair<double, Neuron*>& pair : _outputs)
    {
        unsigned long temp = reinterpret_cast<unsigned long>(pair.second);
        NeuronHandle id = static_cast<NeuronHandle>(temp);
        pair.second = &parentNetwork->getNeuron(id);
    }
}

