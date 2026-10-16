#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include "audio.h"

namespace sl
{

enum class Waveform
{
    sine,
    square,
    saw,
    triangle,
    noise
};

struct Adsr
{
    float attack  = 0.01f;
    float decay   = 0.10f;
    float sustain = 0.70f;
    float release = 0.10f;
};

struct Pitch
{
    float start = 1.0f;
    float end   = 1.0f;
};

struct SynthOscillator
{
    Waveform waveform = Waveform::sine;
    float frequency = 440.0f;
    float amplitude = 1.0f;
    Pitch pitch;
};

struct SynthParams
{
    int sample_rate = 44100;
    int channels = 1;

    float duration = 1.0f;

    std::vector<SynthOscillator> oscillators;

    Adsr envelope;


    unsigned int noise_seed = 0x12345678;
};

/**
 * Generate signed 16-bit PCM audio.
 *
 * Samples are interleaved when channels > 1.
 */
std::vector<std::int16_t>
generate_audio(const SynthParams &params);

/**
 * Generate a Sample suitable for play_sample().
 */
Sample *generate_sample(const SynthParams &params);

} // namespace sl