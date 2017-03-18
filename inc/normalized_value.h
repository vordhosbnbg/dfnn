#pragma once
#include <mutex>
#include <memory>
#include "file_archive.h"

class Archive;
template<class T>
class NormalizedValue
{
public:

    NormalizedValue():
        minVal(0),
        maxVal(100),
        rangeVal(100),
        reciprocalRangeVal(1 / 100),
        normalizedValue(0)
    {

    }

    NormalizedValue(T min, T max) :
        minVal(min),
        maxVal(max),
        rangeVal(max - min),
        reciprocalRangeVal(1 / (max - min)),
        normalizedValue(0)
    {

    }

    NormalizedValue(const NormalizedValue& other) :
        minVal(other.minVal),
        maxVal(other.maxVal),
        rangeVal(other.rangeVal),
        reciprocalRangeVal(other.reciprocalRangeVal),
        normalizedValue(other.normalizedValue)
    {

    }


    ~NormalizedValue()
    {
    }

    void updateMinMax()
    {
        rangeVal= maxVal - minVal;
        reciprocalRangeVal = 1 / (maxVal - minVal);
    }

    T get()
    {
        std::lock_guard<std::mutex> lock(value_mutex);
        T retVal = (T)(normalizedValue*rangeVal + minVal);
        return retVal;
    }

    void set(T val)
    {
        std::lock_guard<std::mutex> lock(value_mutex);
        normalizedValue = (val - minVal)*(reciprocalRangeVal);
    }

    double getNormalized()
    {
        std::lock_guard<std::mutex> lock(value_mutex);
        return normalizedValue;
    }

    void setNormalized(double val)
    {
        std::lock_guard<std::mutex> lock(value_mutex);
        normalizedValue = val;
    }

    T getMin() const
    {
        return minVal;
    }

    void setMin(T val)
    {
        minVal = val;
        updateMinMax();
    }

    T getMax() const
    {
        return maxVal;
    }

    void setMax(T val)
    {
        maxVal = val;
        updateMinMax();
    }

    std::string save(Archive *ar)
    {
        std::string ret;
        ret += "NVAL\n";
        ret += ar->save(minVal);
        ret += ar->save(maxVal);
        ret += ar->save(normalizedValue);
        return ret;
    }
    void load(std::stringstream& ss, NormalizedValue& obj, Archive *ar)
    {
        std::string line;
        std::getline(ss, line);
        if(line.substr(0,4) == "NVAL")
        {
            ar->load(ss, minVal, ar);
            ar->load(ss, maxVal, ar);
            updateMinMax();
            ar->load(ss, normalizedValue, ar);
            setNormalized(normalizedValue);
        }
    }

private:
    T minVal;
    T maxVal;
    T rangeVal;
    T reciprocalRangeVal;
    double normalizedValue;

    std::mutex value_mutex;
};
