#include "algorithmfactory.h"
#include "io.h"
namespace anima
{
    namespace af
    {
        alg::IAlgorithm* AlgorithmFactory::produce(AlgorithmFactoryDescriptor desc)
        {
            try
            {
                switch(desc.type)
                {
                    case AlgorithmFactoryDescriptor::EPrimatte:
                    return new alg::primatte::AlgorithmPrimatte(desc.algPrimatteDesc);
                    break;
                default:
                    assert(0);
                }
            }
            catch(std::runtime_error& err)
            {
                Error(std::string("Could not create algorithm object: ")+ err.what());
            }
            catch(...)
            {
                Error("Could not create algorithm object: Unknown error.");
            }
            return nullptr;
        }
    }
}
