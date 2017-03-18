#include "dfnn.h"
#include "neuron.h"


constexpr size_t nbLayer1 = 2000;
constexpr size_t nbLayer2 = 7000;
constexpr size_t nbLayer3 = 10;
constexpr size_t nbNeurons = nbLayer1 + nbLayer2 + nbLayer3;

int main(int argc, char **argv)
{
    DFNN network;
    for(size_t ind = 0; ind < nbNeurons; ++ind)
    {
        network.createNeuron();
    }
    for(size_t ind = 0; ind < nbLayer1; ++ind)
    {
        Neuron& n1 = network.getNeuronByIndex(ind);
        for(size_t ind = 0; ind < nbLayer2; ++ind)
        {
            Neuron& n2 = network.getNeuronByIndex(nbLayer1 + ind);
            n1.ConnectOutput(rand(), n2);
        }
    }
    for(size_t ind = 0; ind < nbLayer2; ++ind)
    {
        Neuron& n1 = network.getNeuronByIndex(nbLayer1 +ind);
        for(size_t ind = 0; ind < nbLayer3; ++ind)
        {
            Neuron& n2 = network.getNeuronByIndex(nbLayer1 + nbLayer2 + ind);
            n1.ConnectOutput(rand(), n2);
        }
    }
    return 0;
}
