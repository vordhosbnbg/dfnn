#include "neuron.h"

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

void Neuron::DbgPrint() const
{
    std::cout << "N[" << _index << "](" << _outputs.size() << "){" <<  std::hex << _id << "}\n" << "outputs: \n";

    for(const Connection& conn : _outputs)
    {
        std::cout << "\t[" << conn.first << "] --> " << std::hex << conn.second->getID() << "\n";
    }
    std::cout << "Acc: " << _accumulator <<  "; Charge: " << _charge << "\n";
}
