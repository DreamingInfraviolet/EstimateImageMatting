#include "outputassembler.h"
#include "io.h"
#include "IAlgorithm.h"

namespace anima
{
    namespace oa
    {
        void OutputAssembler::assemble(OutputAssemblerDescriptor desc)
        {
            if(desc.algorithm->prepared()==false)
                throw std::runtime_error("Algorithm not ready");

            if(desc.imageSource == nullptr)
            {
                Warning("No alpha image given for generation.");
            }

        }
    }
}
