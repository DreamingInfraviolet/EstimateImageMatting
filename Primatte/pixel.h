#pragma once
#include <stdlib.h>
#include "io.h"

//define this byte mask to be applied to each colour component
//before comparison. 0xff is full precision. 0x00 is no precision.
//Undefine for full precision.
#define PIXEL_IGNORE_MASK 0xF9

class PixelRGB8
{

public:
    unsigned char mR,mG,mB, _padding;


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

    bool operator < (PixelRGB8 px2) const
    {
#ifdef PIXEL_IGNORE_MASK
        PixelRGB8 px1 = *this;
        px1.mR &= PIXEL_IGNORE_MASK;
        px1.mG &= PIXEL_IGNORE_MASK;
        px1.mB &= PIXEL_IGNORE_MASK;

        px2.mR &= PIXEL_IGNORE_MASK;
        px2.mG &= PIXEL_IGNORE_MASK;
        px2.mB &= PIXEL_IGNORE_MASK;

        return (*reinterpret_cast<const int*>(&px1)) < (*reinterpret_cast<const int*>(&px2));
#else
        return (*reinterpret_cast<const int*>(this)) < (*reinterpret_cast<const int*>(&px2));
#endif
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

    //    //The following two types are used to allow external classes to construct
    //    //pixel-specific histograms covering the range of a pixel.

    //    //Defines the type of the index to use to for a histogram
    //    //Must be an integral type.
    //    static int index;

    //    //The maximum index that the pixel may have.
    //    static decltype(index) maxIndex;
    //    static PixelRGB8 fromIndex(const decltype(index) i)
    //    {
    //        PixelRGB8 out;
    //        out.mR = i & 0x000000ff;
    //        out.mG = i & 0x0000ff00;
    //        out.mB = i & 0x00ff0000;

    //        return out;
    //    }

    //    decltype(index) toIndex() const
    //    {
    //        return (((int)mB) << 16) | (((int)mG<<8)) | ((int)mR);
    //    }
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

    float r() const {return *mComponent;}
    float g() const {return *mComponent;}
    float b() const {return *mComponent;}

    float rf() const {return *mComponent;}
    float gf() const {return *mComponent;}
    float bf() const {return *mComponent;}


    friend std::ostream& operator << (std::ostream& str, const PixelFloatR32Ref& px)
    {
        str<<"("<<ToString(*px.mComponent)<<")";
        return str;
    }
};
