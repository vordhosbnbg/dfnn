#include "dfnn.h"
#include "neuron.h"


constexpr unsigned int nbLayer1 = 20;
constexpr unsigned int nbLayer2 = 70;
constexpr unsigned int nbLayer3 = 10;
constexpr unsigned int nbNeurons = nbLayer1 + nbLayer2 + nbLayer3;
constexpr unsigned int nbPumpsTotal = 20000000;
constexpr unsigned int nbPumps = nbPumpsTotal / nbNeurons;

int main(int argc, char **argv)
{
    DFNN network;
    for(auto ind = 0; ind < nbNeurons; ++ind)
    {
        network.createNeuron();
    }
    for(auto ind = 0; ind < nbLayer1; ++ind)
    {
        Neuron& n1 = network.getNeuronByIndex(ind);
        for(auto ind = 0; ind < nbLayer2; ++ind)
        {
            Neuron& n2 = network.getNeuronByIndex(nbLayer1 + ind);
            n1.ConnectOutput(rand(), n2);
        }
    }
    for(auto ind = 0; ind < nbLayer2; ++ind)
    {
        Neuron& n1 = network.getNeuronByIndex(nbLayer1 +ind);
        for(auto ind = 0; ind < nbLayer3; ++ind)
        {
            Neuron& n2 = network.getNeuronByIndex(nbLayer1 + nbLayer2 + ind);
            n1.ConnectOutput(rand(), n2);
        }
    }
    for(auto ind = 0; ind < nbPumps; ++ind)
    {
        network.pumpNetwork();
    }
    return 0;
}
