#pragma once
#include <memory>
#include <random>

using Handle = unsigned int;

class RandomIdGenerator
{
public:
    RandomIdGenerator() : _engRandom(_rnd()), _uniform_dist(0, UINT32_MAX)
    {

    }
    ~RandomIdGenerator(){}

protected:
    template<class MapT>
    Handle getNewID(const MapT& keyMap)
    {
        Handle candidate;
        do
        {
            candidate = _uniform_dist(_engRandom);
        }
        while(keyMap.find(candidate) != keyMap.end());
        return candidate;
    }

private:
    std::random_device _rnd;
    std::mt19937_64 _engRandom;
    std::uniform_int_distribution<Handle> _uniform_dist;

};
