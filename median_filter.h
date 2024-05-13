#pragma once
#include <vector>
#include <type_traits>
#include <algorithm>


template<typename T>
//template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && !std::is_same_v<T, char>>> -> SFINAE alternative
class MedianFilter
{
    static_assert(std::conjunction<std::is_arithmetic<T>, std::negation<std::is_same<T, char>>>::value, 
        "Please instantiate MedianFilter class with an Arithmetic Type which is not char.");

public:
    MedianFilter() { index = 0; };

    T getData()
    {
        std::sort(itsData.begin(), itsData.end());
        T average{};
        for (int i{ medianSize }; i < medianSize * 2; i++)
        {
            average += itsData[i];
        }
        return average / medianSize;
    };

    int getIndex()
    {
        return index;
    }
    
    void enterData(T data)
    {
        itsData[index++] = data;
        if (index >= 29)
            index = 0;
    }

private:
    static constexpr int medianSize = 10;
    static constexpr int windowSize{ medianSize * 3 };
    std::vector<T> itsData;
    int index;
    
};