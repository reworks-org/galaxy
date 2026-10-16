/** @file
 * @brief Implements the library's configured noise-generator wrapper.
 */

#include "noise.h"
#include <random>

namespace sl
{

Generator::Generator(int seed) : generator_(seed)
{
}

void Generator::set_frequency(float frequency)
{
    generator_.SetFrequency(frequency);
}

void Generator::set_type(FastNoiseLite::NoiseType type)
{
    generator_.SetNoiseType(type);
}

void Generator::set_fractal_type(FastNoiseLite::FractalType type)
{
    generator_.SetFractalType(type);
}
float Generator::get(float x, float y) const
{
    return generator_.GetNoise(x, y);
}

float Generator::get(float x, float y, float z) const
{
    return generator_.GetNoise(x, y, z);
}

const float prng()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    return dis(gen);
}
} // namespace sl
