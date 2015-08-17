#include "outputassembler.h"
#include "io.h"

namespace anima
{
    namespace oa
    {
        void OutputAssembler::assemble(OutputAssemblerDescriptor desc)
        {
            if(desc.algorithm->prepared()==false)
                desc.algorithm->prepare();

            if(desc.imageSource == nullptr)
            {
                Warning("No alpha image given for generation.");
            }

        }
    }
}
