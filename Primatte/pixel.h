#pragma once
#include <stdlib.h>
#include <cassert>
#include <cstdint>
#include "io.h"
#include "util.h"

typedef uint8_t byte;
//Assumes little endian
class PixelRGB8
{

public:
    uint8_t mR,mG,mB, _padding;


    PixelRGB8() : mR(0),mG(0),mB(0), _padding(0) {}

    PixelRGB8(unsigned char r, unsigned char g, unsigned char b)
        : mR(r), mG(g), mB(b), _padding(0) {}

    PixelRGB8(const unsigned char* data)
    {
        mR = data[0];
        mG = data[1];
        mB = data[2];
        _padding = 0;
    }

    static uint32_t comparisonBitmask;

    /** Must be called before any comparisons take place. */
    static void comparisonBitsToIgnore(int n)
    {
        assert(n >= 0 && n <= 8);
        uint32_t componentMask = (0x000000ff >> (n)) << (n);

        if(!isBigEndian())
        {
            comparisonBitmask =
                    (componentMask) |
                    (componentMask << 8) |
                    (componentMask << 16);
        }
        else
        {
            comparisonBitmask =
                    (componentMask << 8) |
                    (componentMask << 16) |
                    (componentMask << 24);
        }
    }

    bool operator < (const PixelRGB8& px2) const
    {
        return (*reinterpret_cast<const uint32_t*>(this) & comparisonBitmask) <
                (*reinterpret_cast<const uint32_t*>(&px2) & comparisonBitmask);
    }


    static size_t logicalSize()
    {
        return sizeof(unsigned char)*3;
    }

    unsigned char r() const {return mR;}
    unsigned char g() const {return mG;}
    unsigned char b() const {return mB;}

    //Return normalised floating versions:
    float rf() const {return mR/255.f;}
    float gf() const {return mG/255.f;}
    float bf() const {return mB/255.f;}

    friend std::ostream& operator << (std::ostream& str, const PixelRGB8& px)
    {
        str<<"("<<ToString((int)px.mR) << ", "<<ToString((int)px.mG)<<", "<<ToString((int)px.mB)<<")";
        return str;
    }
};

//Single component floating point image.
class PixelFloatR32
{
public:
    float mComponent;


    PixelFloatR32() : mComponent(0) {}

    PixelFloatR32(const float f) {mComponent = f;}

    PixelFloatR32(const unsigned char* data)
    {
        mComponent = *(float*)data;
    }

    bool operator < (PixelFloatR32 px2) const
    {
        return (mComponent < px2.mComponent);
    }


    static size_t logicalSize()
    {
        return sizeof(float);
    }

    float r() const {return mComponent;}
    float g() const {return mComponent;}
    float b() const {return mComponent;}

    float rf() const {return mComponent;}
    float gf() const {return mComponent;}
    float bf() const {return mComponent;}

    friend std::ostream& operator << (std::ostream& str, const PixelFloatR32& px)
    {
        str<<"("<<ToString(px.mComponent)<<")";
        return str;
    }
};


//Single component floating point image.
//Stores a reference to the image instead of copying.
//Thus, setting pixels affects the original.
class PixelFloatR32Ref
{
public:
    float* mComponent;


    PixelFloatR32Ref() : mComponent(nullptr) {}

    PixelFloatR32Ref(float* f) {mComponent = f;}

    PixelFloatR32Ref(unsigned char* data)
    {
        mComponent = (float*)data;
    }

    PixelFloatR32Ref operator = (const float f)
    {
        *mComponent = f;
        return *this;
    }

    bool operator < (PixelFloatR32Ref px2) const
    {
        return (*mComponent < *px2.mComponent);
    }


    static size_t logicalSize()
    {
        return sizeof(float);
    }

    float& r() const {return *mComponent;}
    float& g() const {return *mComponent;}
    float& b() const {return *mComponent;}

    float& rf() const {return *mComponent;}
    float& gf() const {return *mComponent;}
    float& bf() const {return *mComponent;}


    friend std::ostream& operator << (std::ostream& str, const PixelFloatR32Ref& px)
    {
        str<<"("<<ToString(*px.mComponent)<<")";
        return str;
    }
};
