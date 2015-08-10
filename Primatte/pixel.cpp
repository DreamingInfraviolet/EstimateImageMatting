#include "pixel.h"
#include "io.h"

std::ostream& operator << (std::ostream& str, const PixelRGB8& px)
{
    str<<"("<<ToString((int)px.r) << ", "<<ToString((int)px.g)<<", "<<ToString((int)px.b)<<")";
    return str;
}
