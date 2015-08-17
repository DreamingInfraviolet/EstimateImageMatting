#include "util.h"

bool isBigEndian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

float mapRange(float x, float r1min, float r1max, float r2min, float r2max)
{
    return (x + r2min - r1min) * ((r1max-r1min)/(r2max-r2min));
}
