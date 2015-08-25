#pragma once
#include "IAlgorithm.h"
#include "boundingpolyhedron.h"
#include "matrixd.h"
#include "inputassembler.h"

/** This class implements an algorithm inspired by primatte.
    Its main purpose is to combine all the other algorithms in the
    primatte namespace. */
namespace anima
{
    //Forward declaration.
    namespace ia
    {
        class InputAssembler;
    }

    namespace alg
    {
        namespace primatte
        {
            //Forward declaration.
            class IFittingAlgorithm;
            class IColourSegmenter;
            class IAlphaLocator;

            /** The algorithm descriptor to use when creating the algorithm. */
            struct AlgorithmPrimatteDesc
            {
                /* The algorithm to use to group clusters of points together. */
                IColourSegmenter* segmenter;

                /* The locator used for alpha generation. */
                IAlphaLocator* alphaLocator;

                /* The bounding polyhedron descriptor. */
                BoundingPolyhedronDescriptor boundingPolyhedronDesc;
            };

            /** The primatte-inspired algorithm. */
            class AlgorithmPrimatte : public IAlgorithm
            {
                /* The internal sphere ennumerations.
                 * Warning: Must be in inner->outer order.
                 * When improving the algorithm, one may wish to use more spheres. */
                enum PolyId {POLY_INNER, POLY_MIDDLE, POLY_OUTER, POLY_COUNT};

                /* The polyhedron objects in inner->outer order. */
                BoundingPolyhedron mPolys[POLY_COUNT];

                /* The algorithm descriptor. */
                AlgorithmPrimatteDesc mDesc;

            public:

                /** Initialises the class, throwing an exception upon failure
                 * (most commonly std::runtime_error) */
                AlgorithmPrimatte(AlgorithmPrimatteDesc desc);

                /** Prepares alpha computation for the given data set. */
                virtual void analyse();

                /** Computes the alphas for the given set of points in relation to the
                  * previously supplied inputs. */
                virtual cv::Mat computeAlphas() const;

                /** Uses OpenGL to draw a representation of the internal polyhedrons. */
                virtual void debugDraw() const;
            };
        }
    }
}
