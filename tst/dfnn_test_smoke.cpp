#include "dfnn.h"

int main(int argc, char **argv)
{
    DFNN network;
    NeuronHandle l1n1 = network.createNeuron();
    NeuronHandle l1n2 = network.createNeuron();
    NeuronHandle l2n1 = network.createNeuron();
    NeuronHandle l3n1 = network.createNeuron();
    NeuronHandle l3n2 = network.createNeuron();

    Neuron& n1 = network.modifyNeuron(l1n1);
    Neuron& n2 = network.modifyNeuron(l1n2);
    Neuron& n3 = network.modifyNeuron(l2n1);
    Neuron& n4 = network.modifyNeuron(l3n1);
    Neuron& n5 = network.modifyNeuron(l3n2);
    n1.ConnectOutput(0.3, n3);
    n2.ConnectOutput(0.2, n3);
    n3.ConnectOutput(1.0, n4);
    n3.ConnectOutput(2.0, n5);
    n5.ConnectOutput(1.0, n3);
    n1.AccumulateCharge(5.0);
    n2.AccumulateCharge(10.0);
    network.PumpNetwork();
    network.PumpNetwork();
    network.DbgPrint();
    return 0;
}
