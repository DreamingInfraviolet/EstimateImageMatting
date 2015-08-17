#pragma once
#include <cstdint>

bool isBigEndian();

template<class T>
T clamp(T x, T min, T max)
{
    if(x<min)
        return min;
    if(x>max)
        return max;
    return x;
}

//maps x in range[r1min,r1max] to [r2min,r1max].
float mapRange(float x, float r1min, float r1max, float r2min, float r2max);
