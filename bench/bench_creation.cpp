#include "dfnn.h"
#include "neuron.h"


constexpr unsigned int nbNeurons = 2000;

int main(int argc, char **argv)
{
    DFNN network;
    for(auto ind = 0; ind < nbNeurons; ++ind)
    {
        network.createNeuron();
    }
    return 0;
}
