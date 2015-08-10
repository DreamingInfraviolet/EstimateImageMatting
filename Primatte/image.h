#pragma once

/**
  * A class used to wrap around raw image data.
  * Takes in a Pixel parameter that must have the following constructor:
  *     Pixel(const unsigned char* data);
  * It must also support static a size_t Pixel::logicalSize() method,
  * which returns the number of components * component width.
  * This constructor must interpret the data.
  * The data may not be copied for optimisation, although then care must be taken
  * to ensure that the pixel does not go out of scope before the image.
  * */

template<class Pixel>
class Image
{
    unsigned char* mData;
    size_t mWidth, mHeight, mComponents, mComponentWidth, mStep, mDataSize, mPixelOffset;
public:

    /** Wraps around the given data.
      * @param data The image data
      * @param height The height of the image
      * @param width The width of the image
      * @param components The number of components in a pixel
      * @param componentWidth The width of a single component in bytes
      * @param step The size in bytes given to store each pixel
      * @param pixelOffset The number of bytes in padding at the start of each pixel.
      * */
    Image(unsigned char* data,
             size_t width,
             size_t height,
             size_t components,
             size_t componentWidth,
             size_t step,
             size_t pixelOffset)
                : mData(data),
                  mWidth(width),
                  mHeight(height),
                  mComponents(components),
                  mComponentWidth(componentWidth),
                  mStep(step),
                  mDataSize(width*height*step),
                  mPixelOffset(pixelOffset)
    {
        assert(Pixel::logicalSize() == componentWidth*components);
        assert(mComponentWidth <= mStep);
        assert(mPixelOffset < componentWidth*components);
    }


    size_t width() { return mWidth; }
    size_t height() { return mHeight; }
    size_t components() { return mComponents; }
    size_t step() { return mStep; }

    Pixel pixelAt(size_t x, size_t y)
    {
        const size_t offset = (x  + mWidth * y) * mStep + mPixelOffset;
        assert(offset < mDataSize);
        return Pixel(mData + offset);
    }
};
