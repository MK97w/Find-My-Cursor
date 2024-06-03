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
    MedianFilter() : index(0), itsData(windowSize) {}

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
        std::cout << index << " " << data << std::endl;
        itsData[index++] = data;
        if (index >= windowSize -1)
            index = 0;
    }

    bool isFull()
    {
        return index == windowSize - 2;
    }

private:
    static constexpr int medianSize = 3;
    static constexpr int windowSize{ medianSize * 3 };
    std::vector<T> itsData;
    int index;
    
};