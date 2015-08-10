#pragma once
#include <stdlib.h>
#include <iostream>

class PixelRGB8
{
public:
    unsigned char r,g,b;

    PixelRGB8() : r(0),g(0),b(0){}

    PixelRGB8(const unsigned char* data)
    {
        r = data[0];
        g = data[1];
        b = data[2];
    }

    static size_t logicalSize()
    {
        return sizeof(unsigned char)*3;
    }

    friend std::ostream& operator << (std::ostream& str, const PixelRGB8& px);
};
