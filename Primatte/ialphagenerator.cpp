#include "ialphagenerator.h"
#include "polyhedronalphagenerator.h"
#include <cassert>

IAlphaGenerator* IAlphaGenerator::create(IAlphaGeneratorData* data)
{
    if(!data)
        return nullptr;

    switch(data->type)
    {
        case EAlphaGeneratorPolyhedron:
        {
        PolyhedronAlphaGenerator* generator = new PolyhedronAlphaGenerator();
        if(!generator || !generator->initialise(data))
        {
            delete generator;
            return nullptr;
        }
        else
            return generator;
        }
        break;
    default:
        assert(0);
    }
    return nullptr;
}
