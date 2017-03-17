#include <mutex>

using namespace std;
template<class T>
class NormalizedValue
{
public:
    NormalizedValue(T min, T max) :
        minVal(min),
        maxVal(max),
        rangeVal(max - min),
        reciprocalRangeVal(1 / (max - min))
    {

    }

    NormalizedValue(const NormalizedValue& other) :
        minVal(other.minVal),
        maxVal(other.maxVal),
        rangeVal(other.rangeVal),
        reciprocalRangeVal(other.reciprocalRangeVal)
    {

    }


    ~NormalizedValue()
    {
    }

    T get()
    {
        lock_guard<mutex> lock(value_mutex);
        T retVal = (T)(normalizedValue*rangeVal + minVal);
        return retVal;
    }

    void set(T val)
    {
        lock_guard<mutex> lock(value_mutex);
        normalizedValue = (val - minVal)*(reciprocalRangeVal);
    }

    double getNormalized()
    {
        lock_guard<mutex> lock(value_mutex);
        return normalizedValue;
    }

    void setNormalized(double val)
    {
        lock_guard<mutex> lock(value_mutex);
        normalizedValue = val;
    }

private:
    T minVal;
    T maxVal;
    const T rangeVal;
    const T reciprocalRangeVal;
    double normalizedValue;

    mutex value_mutex;
};
