#include "dfnn.h"
#include "neuron.h"
#include <unordered_map>
#include <sstream>
#include "joelcolors.h"

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
    CreateInput,
    UpdateInput,
    RemoveInput,
    CreateOutput,
    UpdateOutput,
    RemoveOutput,
    SyntaxError,
    Help,
};

enum class Status
{
    OK,
    NOK,
};

Color::Modifier blue(Color::FG_LIGHT_BLUE);
Color::Modifier red(Color::FG_RED);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier yellow(Color::FG_LIGHT_YELLOW);
Color::Modifier def(Color::FG_DEFAULT);

using StringToCommandMap = std::unordered_map<std::string,Command>;
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
    {"cin", Command::CreateInput},
    {"uin", Command::UpdateInput},
    {"rin", Command::RemoveInput},
    {"cout", Command::CreateOutput},
    {"uout", Command::UpdateOutput},
    {"rout", Command::RemoveOutput},
    {"help", Command::Help},
};

static std::unique_ptr<DFNN> networkPtr;


void ErrNoNetwork()
{
    std::cout << yellow << "No network. Please create a network first with cnet command." << def << std::endl;
}

void ErrNeuronNotFound(Handle id)
{
    std::cout << yellow << "No neuron with ID " << green << std::hex << id << yellow << " in the network." << def << std::endl;
}

void ErrNotHex(const std::string& input)
{
    std::cout << yellow << "Expected a hex number, not \'" <<  def << input << yellow << "\'" << def << std::endl;
}

void ErrNotNumber(const std::string& input)
{
    std::cout << yellow << "Expected a number, not \'" <<  def << input << yellow << "\'" << def << std::endl;
}
Handle TryParseHandle()
{
    std::string input;
    std::cin >> input;
    Handle id = 0;
    std::stringstream converter(input);
    converter >> std::hex >> id;
    if(converter.fail())
    {
        ErrNotHex(input);
        id = 0;
    }
    return id;
}
bool TryParseDouble(double& val)
{
    bool retVal = false;
    std::string input;
    std::cin >> input;
    std::stringstream converter(input);
    double dbl;
    converter >> dbl;
    if(!converter.fail())
    {
        val = dbl;
        retVal = true;
    }
    else
    {
        ErrNotNumber(input);
    }
    return retVal;
}

void CreateNetwork()
{
    if(networkPtr != nullptr)
    {
        std::cout << yellow << "Destroying current network..." << def << std::endl;
        networkPtr.release();
    }
    std::cout << blue <<"Creating new empty network" << def << std::endl;
    networkPtr = std::make_unique<DFNN>();
}

void CreateNeuron()
{
    if(networkPtr != nullptr)
    {
        Handle hNrn = networkPtr->createNeuron();
        std::cout << blue << "Created neuron with id =" << green << " "<< std::hex << hNrn << def << std::endl;
    }
    else
    {
        ErrNoNetwork();
    }
}

void RemoveNeuron()
{
    if(networkPtr != nullptr)
    {
        Handle id = TryParseHandle();
        if(id)
        {
            if(networkPtr->removeNeuron(id))
            {
                std::cout << blue << "Removed neuron with ID " << green << std::hex << id << def << std::endl;
            }
            else
            {
                ErrNeuronNotFound(id);
            }
        }
    }
    else
    {
        ErrNoNetwork();
    }
}

void ConnectNeuron()
{
    if(networkPtr != nullptr)
    {
        Handle id1 = TryParseHandle();
        if(id1)
        {
            Handle id2 = TryParseHandle();
            if(id2)
            {
                double bias;
                if(TryParseDouble(bias))
                {
                    if(networkPtr->existsNeuron(id1))
                    {
                        if(networkPtr->existsNeuron(id2))
                        {
                            Neuron& n1 = networkPtr->getNeuron(id1);
                            Neuron& n2 = networkPtr->getNeuron(id2);
                            n1.ConnectOutput(bias, n2);
                            std::cout << blue << "Neuron " << green << std::hex << id1 << blue << " is now connected to "
                                      << green << std::hex << id2 << def<< std::endl;
                        }
                        else
                        {
                            ErrNeuronNotFound(id2);
                        }
                    }
                    else
                    {
                        ErrNeuronNotFound(id1);
                    }
                }
            }
        }
    }
    else
    {
        ErrNoNetwork();
    }
}

void AccumulateCharge()
{
    if(networkPtr != nullptr)
    {
        std::string input;
        std::cin >> input;
        Handle id = 0u;
        std::stringstream converter(input);
        converter >> std::hex >> id;
        if(!converter.fail())
        {
            double charge;
            std::cin >> input;
            converter.str("");
            converter.clear();
            converter << input;
            converter >> charge;
            if(!converter.fail())
            {
                if(networkPtr->existsNeuron(id))
                {
                    Neuron& n1 = networkPtr->getNeuron(id);
                    n1.AccumulateCharge(charge);
                    std::cout << yellow << charge << blue << " added to neuron " << green << std::hex << id
                              << blue << ". It now has " << yellow << n1.getAccumulator() << def << std::endl;
                }
                else
                {
                    ErrNeuronNotFound(id);
                }
            }
            else
            {
                ErrNotNumber(input);
            }
        }
        else
        {
            ErrNeuronNotFound(id);
        }
    }
    else
    {
        ErrNoNetwork();
    }
}
void PumpNetwork()
{
    if(networkPtr != nullptr)
    {
        networkPtr->pumpNetwork();
        std::cout << blue << "Pumped signals across connections." << def << std::endl;
    }
    else
    {
        ErrNoNetwork();
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
        ErrNoNetwork();
    }
}

void NewInput()
{
    if(networkPtr != nullptr)
    {
        double min;
        double max;
        if(TryParseDouble(min))
        {
            if(TryParseDouble(max))
            {
                Handle in = networkPtr->createInput(min, max);
                std::cout << blue << "Created input with id" << green << " "<< std::hex << in << def << std::endl;
            }
        }
    }
    else
    {
        ErrNoNetwork();
    }
}
void UpdateInput()
{
    if(networkPtr != nullptr)
    {
        Handle id = TryParseHandle();
        if(id)
        {
            double val;
            if(TryParseDouble(val))
            {
                networkPtr->updateInput(id, val);
                std::cout << blue << "Updated intput" << green << " " << std::hex << id << blue << " with value " << yellow << val << def << std::endl;
            }
        }
    }
    else
    {
        ErrNoNetwork();
    }
}
void RemoveInput()
{
    if(networkPtr != nullptr)
    {
        Handle id = TryParseHandle();
        if(id)
        {
            networkPtr->removeInput(id);
            std::cout << blue << "Removed input with id" << green << " "<< std::hex << id << def << std::endl;
        }
    }
    else
    {
        ErrNoNetwork();
    }
}
void NewOutput()
{
    if(networkPtr != nullptr)
    {
        double min;
        double max;
        if(TryParseDouble(min))
        {
            if(TryParseDouble(max))
            {
                Handle id = networkPtr->createOutput(min, max);
                std::cout << blue << "Created output with id " << green << " "<< std::hex << id << def << std::endl;
            }
        }
    }
    else
    {
        ErrNoNetwork();
    }
}

void UpdateOutput()
{
    if(networkPtr != nullptr)
    {
        Handle id = TryParseHandle();
        if(id)
        {
            double val;
            networkPtr->updateOutput(id, val);
            std::cout << blue << "Output" << green << " " << std::hex << id << blue << " has value " << yellow << val << def << std::endl;
        }
    }
    else
    {
        ErrNoNetwork();
    }
}
void RemoveOutput()
{
    if(networkPtr != nullptr)
    {
        Handle id = TryParseHandle();
        if(id)
        {
            networkPtr->removeInput(id);
            std::cout << blue << "Removed output with id" << green << " "<< std::hex << id << def << std::endl;
        }
    }
    else
    {
        ErrNoNetwork();
    }
}


void SyntaxError()
{
    std::cout << red << "Bad command" << def << std::endl;
}

void Help()
{
    std::cout << "Commands are:\n"
              << def << "quit                          " << blue << "# quits \n"
              << def << "cnet                          " << blue << "# create a new network\n"
              << def << "cnod                          " << blue << "# create a new neuron and get handle\n"
              << def << "rnod ID                       " << blue << "# remove a neuron by ID\n"
              << def << "conn srcID dstID bias         " << blue << "# connect two neurons with specified strength\n"
              << def << "accc ID ammount               " << blue << "# accumulate charge in a neuron\n"
              << def << "pump                          " << blue << "# pump signals across the network\n"
              << def << "prnt                          " << blue << "# print the network and its nodes\n"
              << def << "cin min max                   " << blue << "# new input between min and max\n"
              << def << "uin ID value                  " << blue << "# update input\n"
              << def << "rin ID                        " << blue << "# remove input by ID\n"
              << def << "cout min max                  " << blue << "# new output between min and max\n"
              << def << "uout ID                       " << blue << "# show output value\n"
              << def << "rout ID                       " << blue << "# remove output by ID\n"
              << def << "help                          " << blue << "# this\n" << def << " "
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
        case Command::CreateInput:
            NewInput();
            break;
        case Command::UpdateInput:
            UpdateInput();
            break;
        case Command::RemoveInput:
            RemoveInput();
            break;
        case Command::CreateOutput:
            NewOutput();
            break;
        case Command::UpdateOutput:
            UpdateOutput();
            break;
        case Command::RemoveOutput:
            RemoveOutput();
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

void Prompt()
{
    std::cout << def << "> ";
}

int main(int argc, char **argv)
{
    std::string input;
    Help();
    Prompt();
    std::cin >> input;
    while(ParseAndExecuteCommand(input) != Status::NOK)
    {
        Prompt();
        std::cin >> input;
    }
    return 0;
}
