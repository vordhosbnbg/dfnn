#include "dfnn.h"
#include "neuron.h"
#include <map>
#include <sstream>

enum class Command
{
    Quit,
    CreateNetwork,
    CreateNeuron,
    RemoveNeuron,
    ConnectNeuron,
    AccumulateCharge,
    PumpNetwork,
    PrintNetwork,
    SyntaxError,
    Help,
};

enum class Status
{
    OK,
    NOK,
};

using StringToCommandMap = std::map<std::string,Command>;
StringToCommandMap commandMap =
{
    {"quit", Command::Quit},
    {"cnet", Command::CreateNetwork},
    {"cnod", Command::CreateNeuron},
    {"rnod", Command::RemoveNeuron},
    {"conn", Command::ConnectNeuron},
    {"accc", Command::AccumulateCharge},
    {"pump", Command::PumpNetwork},
    {"prnt", Command::PrintNetwork},
    {"help", Command::Help},
};

static std::unique_ptr<DFNN> networkPtr;

void CreateNetwork()
{
    if(networkPtr != nullptr)
    {
        std::cout << "Destroying current network..." << std::endl;
        networkPtr.release();
    }
    std::cout << "Creating new empty network" << std::endl;
    networkPtr = std::make_unique<DFNN>();
}

void CreateNeuron()
{
    if(networkPtr != nullptr)
    {
        Handle hNrn = networkPtr->createNeuron();
        std::cout << "Created neuron with id = " << std::hex << hNrn << std::endl;
    }
    else
    {
        std::cout << "No network. Please create a network first with cnet command." << std::endl;
    }
}
void RemoveNeuron()
{
    if(networkPtr != nullptr)
    {
        std::string input;
        std::cout << "ID: ";
        std::cin >> input;
        Handle id = 0;
        std::stringstream converter(input);
        converter >> std::hex >> id;
        if(converter.fail())
        {
            std::cout << "Not a hex number." << std::endl;
        }
        else
        {
            if(networkPtr->removeNeuron(id))
            {
                std::cout << "Removed neuron with ID " << std::hex << id << std::endl;
            }
            else
            {
                std::cout << "No neuron with this ID in the network." << std::endl;
            }
        }
}
    else
    {
        std::cout << "No network. Please create a network first with cnet command." << std::endl;
    }
}
void ConnectNeuron()
{
    if(networkPtr != nullptr)
    {
        std::string input;
        std::cin >> input;
        Handle id1 = 0;
        Handle id2 = 0;
        double bias;
        std::stringstream converter(input);
        converter >> bias;
        if(!converter.fail())
        {
            std::cin >> input;
            converter.str("");
            converter.clear();
            converter << input;
            converter >> std::hex >> id1;
            if(!converter.fail())
            {
                std::cin >> input;
                converter.str("");
                converter.clear();
                converter << input;
                converter >> std::hex >> id2;
                if(!converter.fail())
                {
                    if(networkPtr->existsNeuron(id1))
                    {
                        if(networkPtr->existsNeuron(id2))
                        {
                            Neuron& n1 = networkPtr->modifyNeuron(id1);
                            Neuron& n2 = networkPtr->modifyNeuron(id2);
                            n1.ConnectOutput(bias, n2);
                            std::cout << "Neuron " << std::hex << id1 << " is now connected to "
                                      << std::hex << id2 << std::endl;
                        }
                        else
                        {
                            std::cout << "Target neuron with ID " << std::hex << id2 << " not found in network." << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "Source neuron with ID " << std::hex << id1 << " not found in network." << std::endl;
                    }
                }
                else
                {
                    std::cout << "Not a valid ID (" << input << ")" << std::endl;
                }
            }
            else
            {
                std::cout << "Not a valid ID (" << input << ")" << std::endl;
            }
        }
    }
    else
    {
        std::cout << "No network. Please create a network first with cnet command." << std::endl;
    }
}
void AccumulateCharge()
{
    if(networkPtr != nullptr)
    {
        Handle hNrn = networkPtr->createNeuron();
        std::cout << "Created neuron with id = " << std::hex << hNrn << std::endl;
    }
    else
    {
        std::cout << "No network. Please create a network first with cnet command." << std::endl;
    }
}
void PumpNetwork()
{
    if(networkPtr != nullptr)
    {
        networkPtr->pumpNetwork();
        std::cout << "Pumped signals across connections." << std::endl;
    }
    else
    {
        std::cout << "No network. Please create a network first with cnet command." << std::endl;
    }
}
void PrintNetwork()
{
    if(networkPtr != nullptr)
    {
        networkPtr->dbgPrint();
    }
    else
    {
        std::cout << "No network. Please create a network first with cnet command." << std::endl;
    }
}

void SyntaxError()
{
    std::cout << "Bad command" << std::endl;
}

void Help()
{
    std::cout << "Commands are:\n"
              << "quit\n"
              << "cnet\n"
              << "cnod\n"
              << "rnod\n"
              << "conn bias sourceID targetID\n"
              << "accc ammount\n"
              << "pump\n"
              << "prnt\n"
              << "help\n"
              << std::endl ;
}

Status ParseAndExecuteCommand(const std::string& command)
{
    StringToCommandMap::const_iterator mapIt = commandMap.find(command);

    if(mapIt != commandMap.end())
    {
        Command cmd = mapIt->second;
        switch (cmd)
        {
        case Command::Quit:
            return Status::NOK;
            break;
        case Command::CreateNetwork:
            CreateNetwork();
            break;
        case Command::CreateNeuron:
            CreateNeuron();
            break;
        case Command::RemoveNeuron:
            RemoveNeuron();
            break;
        case Command::ConnectNeuron:
            ConnectNeuron();
            break;
        case Command::AccumulateCharge:
            AccumulateCharge();
            break;
        case Command::PumpNetwork:
            PumpNetwork();
            break;
        case Command::PrintNetwork:
            PrintNetwork();
            break;
        case Command::Help:
            Help();
            break;
        default:
            SyntaxError();
            break;
        }
    }
    else
    {
        SyntaxError();
    }
    return Status::OK;
}

int main(int argc, char **argv)
{
    std::string input;
    std::cin >> input;
    while(ParseAndExecuteCommand(input) != Status::NOK)
    {
        std::cin >> input;
    }
    return 0;
}
