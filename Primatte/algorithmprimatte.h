#pragma once
#include "ialgorithm.h"
#include "boundingpolyhedron.h"
#include "inputassembler.h"

/** This class implements an algorithm inspired by primatte.
    Its main purpose is to combine all the other algorithms in the
    primatte namespace and provide a single interface. */
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

                /* The bounding polyhebescriptor. */
                BoundingPolyhedronDescriptor boundingPolyhedronDesc;


                //Parameters

                //The parameter passed to the colour segmenter.
                //The inner background points returned are wrapped around.
                float innerShrinkingShreshold;

                //The distance of a shrunken inner vertex from the centre.
                float innerShrinkingMinDistance;

                //After shrinking the inner polyhedron is multiplied by this amount.
                float innerPostShrinkingMultiplier;

                //After the outer polyhedron is wrapped around the foreground points,
                //It is multiplied by this amount before expansion.
                float outerExpansionStartRadiusMultiplier;

                //The approximate amount the outer sphere should try to expand.
                float outerExpandDelta;

                //The amount the outer sphere should be scaled after expansion relative to
                //the inner sphere. It is made so that it never collides with the inner polyhedron,
                //such that a value of 0 = as close as possible to inner. A value of 1 = minimal scaling.
                //Note that it is relative to the final size of the inner polyhedron.
                float outerScaleParameter;

            };

            /** The primatte-inspired algorithm. */
            class AlgorithmPrimatte : public IAlgorithm
            {
                /* The internal sphere ennumerations.
                 * Warning: Must be in inner->outer order.
                 * When improving the algorithm, one may wish to use more spheres. */
                enum PolyId {POLY_INNER, POLY_OUTER, POLY_COUNT};

                /* The polyhedron objects in inner->outer order. */
                BoundingPolyhedron mPolys[POLY_COUNT];

                /* The algorithm descriptor. */
                AlgorithmPrimatteDesc mDesc;

            public:

                /** Initialises the class, throwing an exception upon failure
                 * (most commonly std::runtime_error) */
                AlgorithmPrimatte(AlgorithmPrimatteDesc desc);

                /** Prepares alpha computation for the given data set by analysing the input. */
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
