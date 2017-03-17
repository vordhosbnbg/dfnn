#include "dfnn.h"
#include "neuron.h"

int main(int argc, char **argv)
{
    DFNN network;
    Handle l1n1 = network.createNeuron();
    Handle l1n2 = network.createNeuron();
    Handle l2n1 = network.createNeuron();
    Handle l3n1 = network.createNeuron();
    Handle l3n2 = network.createNeuron();

    Neuron& n1 = network.getNeuron(l1n1);
    Neuron& n2 = network.getNeuron(l1n2);
    Neuron& n3 = network.getNeuron(l2n1);
    Neuron& n4 = network.getNeuron(l3n1);
    Neuron& n5 = network.getNeuron(l3n2);
    n1.ConnectOutput(0.3, n3);
    n2.ConnectOutput(0.2, n3);
    n3.ConnectOutput(1.0, n4);
    n3.ConnectOutput(2.0, n5);
    n5.ConnectOutput(1.0, n3);
    n1.AccumulateCharge(5.0);
    n2.AccumulateCharge(10.0);
    network.pumpNetwork();
    network.pumpNetwork();
    network.dbgPrint();
    return 0;
}
