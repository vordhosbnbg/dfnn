#include "dfnn.h"

int main(int argc, char **argv)
{
    DFNN network;
    NeuronHandle l1n1 = network.createNeuron();
    NeuronHandle l1n2 = network.createNeuron();
    NeuronHandle l2n1 = network.createNeuron();


    Neuron& n1 = network.modifyNeuron(l1n1);
    Neuron& n2 = network.modifyNeuron(l1n2);
    Neuron& n3 = network.modifyNeuron(l2n1);
    n1.ConnectOutput(0.3, n3);
    n2.ConnectOutput(0.2, n3);
    n1.AccumulateCharge(5.0);
    n2.AccumulateCharge(10.0);
    network.DbgPrint();
    network.PumpNetwork();
    network.DbgPrint();
    return 0;
}
