#pragma once
#include "algorithmprimatte.h"
#include "inputassembler.h"

/**
  * This class is responsible for constructing an algorithm from an
  * AlgorithmFactoryDescriptor object. Its main purpose is to simplify
  * the concept of creating an algorithm by having a single interface
  * for this creation.
  * To use it, first create and fill in the descriptor object.
  * Only the union fields of the corresponding to the algorithm must
  * be filled in. Afterwards, instantiate a factory object (trivial overhead)
  * and produce an algorithm with the descriptor.
  *
  * WARNING: When creating custom algorithms, add them to the anonymous union.
  * Ensure that the types used do not require a destructor/constructor (raw data).
  * Examples of disallowed types are std::string, std::vector, etc. Use pointers instead.
  *
  * Ensure that the data pointed to by the descriptor remains valid for the lifetime
  * of the algorithm.
  */


namespace anima
{
    namespace af
    {
        /** This is the descriptor that must be passed in for the production
          * of an algorithm.
          */
        struct AlgorithmFactoryDescriptor
        {
            enum AlgType { EPrimatte };

            /* The algorithm to be instantiated. */
            AlgType type;

            /* Fill in the algorithm that is being used. */
            union
            {
                alg::primatte::AlgorithmPrimatteDesc algPrimatteDesc;
            };

            /* Default-set values to zero. */
            AlgorithmFactoryDescriptor()
            {
                memset(this, 0, sizeof(*this));
            }
        };

        /** The factory class. */
        class AlgorithmFactory
        {
        public:
            /** Produces the algorithm described in the descriptor.
              * @param desc The descriptor object used to initialise the algorithm.
              * @return An algorithm interface, or null upon failure. */
            alg::IAlgorithm* produce(AlgorithmFactoryDescriptor desc);
        };
    }
}
