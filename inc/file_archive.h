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
    Archive();
    ~Archive();

    template <typename T>
    std::unique_ptr<std::string> save(T obj)
    {
        return obj.save();
    }

    std::unique_ptr<std::string> save(int obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        std::stringstream converter;
        converter << obj;
        if(!converter.fail())
        {
            ret->reserve(16);
            ret->append("INT ");
            converter >> *ret;
            ret->append("\n");
        }

        return std::move(ret);
    }

    std::unique_ptr<std::string> save(unsigned int obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        std::stringstream converter;
        converter << std::hex << obj;
        if(!converter.fail())
        {
            ret->reserve(16);
            ret->append("HEX ");
            converter >> *ret;
            ret->append("\n");
        }

        return std::move(ret);
    }

    std::unique_ptr<std::string> save(double obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        std::stringstream converter;
        converter << std::hex << obj;
        if(!converter.fail())
        {
            ret->reserve(20);
            ret->append("DBL ");
            converter >> *ret;
            ret->append("\n");
        }

        return std::move(ret);
    }

    std::unique_ptr<std::string> save(std::string obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        ret->reserve(30);
        ret->append("STR ");
        ret->append(obj);
        ret->append("\n");

        return std::move(ret);
    }

    template <typename T>
    std::unique_ptr<std::string> save(const std::vector<T>obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        std::stringstream converter;
        converter << obj.size();
        ret->reserve(16);
        ret->append("VEC ");
        ret->append(converter.str());
        ret->append("\n");
        for(const auto& item : obj)
        {
            ret->append(*item.save());
        }

        return std::move(ret);
    }

    template <typename Key, typename T>
    std::unique_ptr<std::string> save(const std::map<Key, T>obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        std::stringstream converter;
        converter << obj.size();
        ret->reserve(16);
        ret->append("MAP ");
        ret->append(converter.str());
        ret->append("\n");
        for(const std::pair<Key, T>& pair : obj)
        {
            ret->append(*save(pair));
        }

        return std::move(ret);
    }

    template <typename Key, typename T>
    std::unique_ptr<std::string> save(const std::unordered_map<Key,T>obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        std::stringstream converter;
        converter << obj.size();
        ret->reserve(16);
        ret->append("UMAP ");
        ret->append(converter.str());
        ret->append("\n");
        for(const std::pair<Key, T>& pair : obj)
        {
            ret->append(*save(pair));
        }

        return std::move(ret);
    }

    template <typename Key, typename T>
    std::unique_ptr<std::string> save(const std::pair<Key,T>obj)
    {
        std::unique_ptr<std::string> ret = std::make_unique<std::string>();
        ret->reserve(10);
        ret->append("PAIR");
        ret->append(*save(obj.first));
        ret->append(*save(obj.second));
        ret->append("\n");
        for(const std::pair<Key, T>& pair : obj)
        {
            ret->append(*save(pair));
        }

        return std::move(ret);
    }


    template <typename T>
    void load(std::stringstream& ss, T& obj)
    {
        return T::load(ss);
    }

    void load(std::stringstream& ss, int& obj)
    {
        int ret = 0;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "INT ")
        {
            std::stringstream converter(line.substr(4));
            converter >> ret;
        }
        obj = ret;
    }

    void load(std::stringstream& ss, unsigned int& obj)
    {
        unsigned int ret = 0;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "HEX ")
        {
            std::stringstream converter(line.substr(4));
            converter >> std::hex >> ret;
        }
        obj = ret;
    }

    void load(std::stringstream& ss, double& obj)
    {
        double ret = 0.0;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "DBL ")
        {
            std::stringstream converter(line.substr(4));
            converter >> ret;
        }
        obj = ret;
    }

    void load(std::stringstream& ss, std::string& obj)
    {
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "STR ")
        {
            std::stringstream converter(line.substr(4));
            converter >> obj;
        }
    }

    template <typename T>
    void load(std::stringstream& ss, std::vector<T>& obj)
    {
        size_t size;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "VEC ")
        {
            std::stringstream converter(line.substr(4));
            converter >> size;
            obj.clear();
            obj.reserve(size);
            for(auto ind = 0; ind < size; ++ ind)
            {
                T item;
                load(ss, item);
                obj.emplace_back(std::move(item));
            }
        }
    }

    template <typename Key, typename T>
    void load(std::stringstream& ss, std::map<Key, T>& obj)
    {
        size_t size;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "MAP ")
        {
            std::stringstream converter(line.substr(4));
            converter >> size;
            obj.clear();
            for(auto ind = 0; ind < size; ++ ind)
            {
                std::pair<Key, T> pair;
                load(ss, pair);
                obj.emplace(std::move(pair));
            }
        }
    }

    template <typename Key, typename T>
    void load(std::stringstream& ss, std::unordered_map<Key, T>& obj)
    {
        size_t size;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "UMAP ")
        {
            std::stringstream converter(line.substr(4));
            converter >> size;
            obj.clear();
            for(auto ind = 0; ind < size; ++ ind)
            {
                std::pair<Key, T> pair;
                load(ss, pair);
                obj.emplace(std::move(pair));
            }
        }
    }

    template <typename Key, typename T>
    void load(std::stringstream& ss, std::pair<Key, T>& obj)
    {
        double ret = 0.0;
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "PAIR ")
        {
            load(ss, obj.first);
            load(ss, obj.second);
        }
    }

    template <typename T>
    void read(const std::string& filename, T& object)
    {
        std::ifstream ifs;
        ifs.open(filename);
        if(ifs.is_open())
        {
            load(ifs, object);
        }
    }

    template <typename T>
    void write(const std::string& filename, T& object)
    {
        std::ofstream ofs;
        ofs.open(filename);
        if(ofs.is_open())
        {
            std::string result = *save( object);
            ofs << result;
        }
    }

};
