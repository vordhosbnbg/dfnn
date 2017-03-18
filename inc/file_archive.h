#pragma once
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>

class Archive
{
public:
    Archive(){}
    ~Archive(){}

    template <typename Key, typename T>
    std::string save(std::pair<Key,T>& obj)
    {
        std::string ret;
        ret.reserve(10);
        ret += "PAIR\n";
        ret += save(obj.first);
        ret += save(obj.second);
        return (ret);
    }

    template <typename T>
    std::string save(std::vector<T>& obj)
    {
        std::string ret;
        std::string result;
        converter.str(std::string());
        converter.clear();
        converter << std::dec << obj.size();
        ret.reserve(16);
        ret += "VEC ";
        ret += converter.str();
        ret += "\n";
        for(T& item : obj)
        {
            result = save(item);
            ret += result;
        }

        return (ret);
    }

    template <typename Key, typename T>
    std::string save(std::map<Key, T>& obj)
    {
        std::string ret;
        converter.str(std::string());
        converter.clear();
        converter << std::dec << obj.size();
        ret.reserve(16);
        ret += "MAP ";
        ret += converter.str();
        ret += "\n";
        for(const std::pair<Key, T>& pair : obj)
        {
            ret += save(pair);
        }

        return (ret);
    }

    template <typename Key, typename T>
    std::string save(std::unordered_map<Key,T>& obj)
    {
        std::string ret;
        converter.str(std::string());
        converter.clear();
        converter << std::dec << obj.size();
        ret.reserve(16);
        ret += "UMAP ";
        ret += converter.str();
        ret += "\n";
        for(std::pair<Key, T> pair : obj)
        {
            ret += save(pair);
        }

        return (ret);
    }

    template <typename T>
    std::string save(T& obj)
    {
        return obj.save(this);
    }

    std::string save(int obj)
    {
        std::string ret;
        converter.str(std::string());
        converter.clear();
        converter << std::dec << obj;
        if(!converter.fail())
        {
            ret.reserve(16);
            ret += "INT ";
            ret += converter.str();
            ret += "\n";
        }

        return (ret);
    }
    std::string save(long int obj)
    {
        return save((int)obj);
    }

    std::string save(unsigned int obj)
    {
        std::string ret;
        converter.str(std::string());
        converter.clear();
        converter << std::hex << obj;
        if(!converter.fail())
        {
            ret.reserve(16);
            ret += "HEX ";
            ret += converter.str();
            ret += "\n";
        }

        return (ret);
    }

    std::string save(long unsigned int obj)
    {
        return save((unsigned int)obj);
    }

    std::string save(double obj)
    {
        std::string ret;
        converter.str(std::string());
        converter.clear();
        converter << std::dec <<obj;
        if(!converter.fail())
        {
            ret.reserve(20);
            ret += "DBL ";
            ret += converter.str();
            ret += "\n";
        }

        return (ret);
    }

    std::string save(std::string& obj)
    {
        std::string ret;
        ret.reserve(30);
        ret += "STR ";
        ret += obj;
        ret += "\n";

        return (ret);
    }


    template <typename T>
    void load(std::stringstream& ss, T& obj, Archive *ar)
    {
        obj.load(ss, obj, ar);
    }

    void load(std::stringstream& ss, int& obj, Archive *ar)
    {
        int ret = 0;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "INT ")
        {
            converter.str(line.substr(4));
            converter.clear();
            converter >> std::dec >> ret;
        }
        obj = ret;
    }

    void load(std::stringstream& ss, unsigned int& obj, Archive *ar)
    {
        unsigned int ret = 0;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "HEX ")
        {
            converter.str(line.substr(4));
            converter.clear();
            converter >> std::hex >> ret;
        }
        obj = ret;
    }

    void load(std::stringstream& ss, long unsigned int& obj, Archive *ar)
    {
        load(ss, reinterpret_cast<unsigned int&>(obj), ar);
    }

    void load(std::stringstream& ss, double& obj, Archive *ar)
    {
        double ret = 0.0;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "DBL ")
        {
            converter.str(line.substr(4));
            converter.clear();
            converter >> std::dec >> ret;
        }
        obj = ret;
    }

    void load(std::stringstream& ss, std::string& obj, Archive *ar)
    {
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "STR ")
        {
            converter.str(line.substr(4));
            converter.clear();
            converter >> obj;
        }
    }

    template <typename T>
    void load(std::stringstream& ss, std::vector<T>& obj, Archive *ar)
    {
        size_t size;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "VEC ")
        {
            converter.str(line.substr(4));
            converter.clear();
            converter >> std::dec >> size;
            obj.clear();
            obj.reserve(size);
            for(size_t ind = 0; ind < size; ++ ind)
            {
                T item;
                load(ss, item, ar);
                obj.emplace_back((item));
            }
        }
    }

    template <typename Key, typename T>
    void load(std::stringstream& ss, std::map<Key, T>& obj, Archive *ar)
    {
        size_t size;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "MAP ")
        {
            converter.str(line.substr(4));
            converter.clear();
            converter >> std::dec >> size;
            obj.clear();
            for(auto ind = 0; ind < size; ++ ind)
            {
                std::pair<Key, T> pair;
                load(ss, pair, ar);
                obj.emplace((pair));
            }
        }
    }

    template <typename Key, typename T>
    void load(std::stringstream& ss, std::unordered_map<Key, T>& obj, Archive *ar)
    {
        size_t size;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,5) == "UMAP ")
        {
            converter.str(line.substr(5));
            converter.clear();
            converter >> std::dec >> size;
            obj.clear();
            for(size_t ind = 0; ind < size; ++ ind)
            {
                std::pair<Key, T> pair;
                load(ss, pair, ar);
                obj.emplace((pair));
            }
        }
    }

    template <typename Key, typename T>
    void load(std::stringstream& ss, std::pair<Key, T>& obj, Archive *ar)
    {
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "PAIR")
        {
            load(ss, obj.first, ar);
            load(ss, obj.second, ar);
        }
    }

    template <typename T>
    void read(const std::string& filename, T& object)
    {
        std::ifstream ifs;
        std::stringstream ss;
        ifs.open(filename);
        if(ifs.is_open())
        {
            ss << ifs.rdbuf();
            load(ss, object, this);
        }
    }

    template <typename T>
    void write(const std::string& filename, T& object)
    {
        std::ofstream ofs;

        ofs.open(filename);
        if(ofs.is_open())
        {
            std::string result = save( object);
            ofs << result;
        }
    }

    std::stringstream converter;
};
