#include "inputassembler.h"

namespace anima
{
    namespace ia
    {

        cv::Mat InputAssembler::loadRgbMatFromFile(const char* path)
        {
            cv::Mat mat;
            mat = cv::imread(path);
            if(mat.data==nullptr)
                throw std::runtime_error("Could not load image data.");

            cv::Mat matbgr;
            mat.convertTo(matbgr, CV_8UC3);

            cv::Mat imageMat;
            cv::cvtColor(matbgr, imageMat, CV_BGR2RGB);

            return imageMat;
        }

        std::vector<PixelRGB8> InputAssembler::loadFromFile(const char* path)
        {
            std::vector<PixelRGB8> pixels;

            auto matrgb = loadRgbMatFromFile(path);

            pixels.reserve(matrgb.cols*matrgb.rows);
            for(int y = 0; y < matrgb.rows; ++y)
                for(int x = 0; x < matrgb.cols; ++y)
                    pixels.push_back(matrgb.at<PixelRGB8>(x,y));
            return pixels;
        }

        InputAssembler::InputAssembler(InputAssemblerDescriptor& desc)
        {
            //Load the data
            switch(desc.inputSource)
            {
            case InputAssemblerDescriptor::EMEMORY:
            {
                const byte* data = desc.inputSourceMemory.data;
                size_t step = desc.inputSourceMemory.step;
                size_t dataSize = desc.inputSourceMemory.dataSize;

                if(desc.inputSourceMemory.type != InputAssemblerDescriptor::EPIXEL_RGB8)
                    throw std::runtime_error("Invalid source memory type.");
                if(data == nullptr)
                    throw std::runtime_error("Image data is null.");
                if(dataSize % step !=0)
                    throw std::runtime_error("Data size is not divisible by step.");


                mPixels.reserve(dataSize/step);
                const byte* end = data + dataSize;
                for(const byte* begin = data; begin < end; begin += step)
                    mPixels.push_back(PixelRGB8(*begin, *(begin+1), *(begin+2)));

            }
                    break;
            case InputAssemblerDescriptor::EFILE:
            {
                mPixels = loadFromFile(desc.inputSourceFile.path);
            }
                    break;
            default:
            {
                assert(0);
            }
            } //end switch

            mPoints = ProcessPixels(mPixels, desc.ipd);
        }

        const std::vector<PixelRGB8>& InputAssembler::pixels() const
        {
            return mPixels;
        }

        const std::vector<alg::Point>& InputAssembler::points() const
        {
            return mPoints;
        }
    }
}
