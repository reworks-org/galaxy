#pragma once

#include "FastNoiseLite.h"

namespace sl
{

    /** A configurable 2D/3D procedural noise generator. */
    class Generator
    {
    public:
        /** Construct a noise generator with the given deterministic seed. */
        explicit Generator(int seed = 1337);

        /** Set the noise frequency. */
        void set_frequency(float frequency);
        /** Set the FastNoiseLite algorithm. */
        void set_type(FastNoiseLite::NoiseType type);

        void set_fractal_type(FastNoiseLite::FractalType type);
        /** Return noise in the range approximately [-1, 1]. */
        float get(float x, float y) const;
        /** Return 3D noise in the range approximately [-1, 1]. */
        float get(float x, float y, float z) const;

    private:
        FastNoiseLite generator_;
    };

    const float prng();
} // namespace sl
