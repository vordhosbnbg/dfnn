#include "dfnn.h"
#include "neuron.h"
#include "file_archive.h"

constexpr unsigned int nbLayer1 = 20;
constexpr unsigned int nbLayer2 = 70;
constexpr unsigned int nbLayer3 = 10;
constexpr unsigned int nbNeurons = nbLayer1 + nbLayer2 + nbLayer3;

int main(int argc, char **argv)
{
    DFNN network;
    for(size_t ind = 0; ind < nbLayer1; ++ind)
    {
        network.createInput(0, 100);
    }
    for(size_t ind = 0; ind < nbLayer2; ++ind)
    {
        network.createNeuron();
    }
    for(size_t ind = 0; ind < nbLayer3; ++ind)
    {
        network.createOutput(-50, 50);
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

    Archive ark;
    std::string filename("saveload_test.txt");
    std::string filename2("saveload_test2.txt");
    ark.write(filename, network);

    DFNN net2;
    ark.read(filename, net2);
    ark.write(filename2, net2);
    return 0;
}
