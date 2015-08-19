#pragma once
#include "IAlgorithm.h"
#include "boundingpolyhedron.h"


/** This class implements an algorithm inspired by primatte. */
namespace anima
{
    namespace ia
    {
        class InputAssembler;
    }

    namespace alg
    {
        namespace primatte
        {
            class IFittingAlgorithm;
            class IColourSegmenter;
            class IAlphaLocator;

            /** The algorithm descriptor. */
            struct AlgorithmPrimatteDesc
            {
                /* The algorithm to use to group clusters of points. */
                IColourSegmenter* segmenter;

                /* The locator used for alpha generation. */
                IAlphaLocator* alphaLocator;

                /* The bounding polyhedron descriptor. */
                BoundingPolyhedronDescriptor boundingPolyhedronDesc;

                /* The point to represent the background image colour. */
                Point backgroundPoint;
            };

            /** The primatte-inspired algorithm. */
            class AlgorithmPrimatte : public IAlgorithm
            {
                /* Warning: Must be in inner->outer order. */
                enum PolyId {POLY_INNER, POLY_MIDDLE, POLY_OUTER, POLY_COUNT};

                /* The polyhedron objects. */
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

                /** Uses old gl to draw a representation of the internal polyhedrons. */
                virtual void debugDraw() const;
            };
        }
    }
}
