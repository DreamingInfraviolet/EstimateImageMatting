#pragma once
#include "ialgorithm.h"
#include "boundingpolyhedron.h"
#include "inputassembler.h"

/** This class implements an algorithm inspired by primatte.
    Its main purpose is to combine all the other algorithms in the
    primatte namespace and provide a single interface.
    In it's current form, it's potentially a little unstable as I did not have the time
    (or the images) to test it fully. But it seems to work! */
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

            /** The algorithm descriptor to use when creating the algorithm.
                You must fill this in entirely and pass it to the constructor. */
            struct AlgorithmPrimatteDesc
            {
                //The following specify sub-algorithms to be used.

                /* The algorithm to use to group clusters of points together. */
                IColourSegmenter* segmenter;

                /* The locator used for alpha generation. */
                IAlphaLocator* alphaLocator;

                /* The bounding polyhedron descriptor. */
                BoundingPolyhedronDescriptor boundingPolyhedronDesc;

                //The following values specify the parameters used by the algorithm.

                //The parameter passed to the colour segmenter when segmenting the background
                //points for the inner polyhedron. The segmenter splits the points into Inner
                //and Outer, and the Inner points are wrapped around. Should be the approximate
                //radius of the sphere inside which the points should be marked as Inner,
                //but may be different if you use your own segmenter.
                float innerShrinkingThreshold;

                //The minimum size of the inner polyhedron after shrinking.
                //Values that are too low may yield in parts of the background being visible.
                float innerShrinkingMinDistance;

                //After shrinking, the inner polyhedron is scaled by this amount.
                float innerPostShrinkingMultiplier;

                //The foreground pixels are then segmented based on this parameter,
                //And the outer polyhedron is positioned around them before expansion.
                float outerExpansionStartThreshold;

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

                bool mAnalysed;

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
