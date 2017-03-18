#include "dfnn.h"
#include "neuron.h"


constexpr size_t nbNeurons = 2000;

int main(int argc, char **argv)
{
    DFNN network;
    for(size_t ind = 0u; ind < nbNeurons; ++ind)
    {
        network.createNeuron();
    }
    return 0;
}
