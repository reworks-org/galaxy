#include "audio_synth.h"
#include "audio.h"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace sl
{

namespace
{

constexpr float pi = 3.14159265358979323846f;
constexpr float two_pi = pi * 2.0f;

float oscillator_sample(Waveform waveform, float phase)
{
    switch (waveform)
    {
    case Waveform::sine:
        return std::sin(phase * two_pi);

    case Waveform::square:
        return phase < 0.5f ? 1.0f : -1.0f;

    case Waveform::saw:
        return 2.0f * phase - 1.0f;

    case Waveform::triangle:
        return 1.0f - 4.0f * std::fabs(phase - 0.5f);

    case Waveform::noise:
        return 0.0f;
    }

    return 0.0f;
}

float envelope_level(const Adsr &adsr, std::size_t sample, std::size_t total_samples, int sample_rate)
{
    const float t = static_cast<float>(sample) / static_cast<float>(sample_rate);

    const float total_time = static_cast<float>(total_samples) / static_cast<float>(sample_rate);

    const float attack_end = adsr.attack;

    const float decay_end = adsr.attack + adsr.decay;

    /*
     * Reserve the release portion of the sound.
     */
    const float release_start = std::max(0.0f, total_time - adsr.release);

    if (adsr.attack > 0.0f && t < attack_end)
    {
        return t / adsr.attack;
    }

    if (adsr.decay > 0.0f && t < decay_end)
    {
        const float p = (t - adsr.attack) / adsr.decay;

        return 1.0f + (adsr.sustain - 1.0f) * p;
    }

    if (t < release_start)
    {
        return adsr.sustain;
    }

    if (adsr.release > 0.0f)
    {
        const float p = (t - release_start) / adsr.release;

        return adsr.sustain * (1.0f - p);
    }

    return 0.0f;
}

std::uint32_t xorshift32(std::uint32_t &state)
{
    if (state == 0)
        state = 0x12345678;

    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;

    return state;
}

float random_noise(std::uint32_t &state)
{
    const std::uint32_t r = xorshift32(state);

    const float n = static_cast<float>(r) / static_cast<float>(std::numeric_limits<std::uint32_t>::max());

    return n * 2.0f - 1.0f;
}

} // namespace

std::vector<std::int16_t> generate_audio(const SynthParams &params)
{
    if (params.sample_rate <= 0 || params.channels <= 0 || params.duration <= 0.0f)
    {
        return {};
    }

    const std::size_t frames = static_cast<std::size_t>(params.duration * static_cast<float>(params.sample_rate));

    if (frames == 0)
        return {};

    std::vector<std::int16_t> output(frames * static_cast<std::size_t>(params.channels));

    /*
     * One phase per oscillator.
     *
     * Keeping phase as [0,1) rather than radians makes
     * saw/square/triangle particularly cheap.
     */
    std::vector<float> phases(params.oscillators.size(), 0.0f);

    std::uint32_t noise_state = params.noise_seed;

    for (std::size_t frame = 0; frame < frames; ++frame)
    {
        const float p = frames > 1 ? static_cast<float>(frame) / static_cast<float>(frames - 1) : 0.0f;

        float value = 0.0f;
        for (std::size_t i = 0; i < params.oscillators.size(); ++i)
        {
            const SynthOscillator &osc = params.oscillators[i];

            const float pitch = osc.pitch.start + (osc.pitch.end - osc.pitch.start) * p;

            float sample;

            if (osc.waveform == Waveform::noise)
            {
                sample = random_noise(noise_state);
            }
            else
            {
                sample = oscillator_sample(osc.waveform, phases[i]);
            }

            value += sample * osc.amplitude;

            const float frequency = osc.frequency * pitch;

            const float increment = frequency / static_cast<float>(params.sample_rate);

            phases[i] += increment;

            phases[i] -= std::floor(phases[i]);
        }

        /*
         * Avoid clipping when multiple oscillators
         * are combined.
         */
        if (!params.oscillators.empty())
        {
            float gain = 0.0f;

            for (const auto &osc : params.oscillators)
                gain += std::fabs(osc.amplitude);

            if (gain > 1.0f)
                value /= gain;
        }

        value *= envelope_level(params.envelope, frame, frames, params.sample_rate);

        value = std::clamp(value, -1.0f, 1.0f);

        const auto pcm = static_cast<std::int16_t>(value * 32767.0f);

        for (int channel = 0; channel < params.channels; ++channel)
        {
            output[frame * static_cast<std::size_t>(params.channels) + static_cast<std::size_t>(channel)] = pcm;
        }
    }

    return output;
}

Sample *generate_sample(const SynthParams &params)
{
    const std::vector<std::int16_t> pcm = generate_audio(params);
    if (pcm.empty())
        return nullptr;

    return create_sample_from_pcm(pcm.data(), pcm.size() / static_cast<std::size_t>(params.channels),
                                  params.sample_rate, params.channels);
}

} // namespace sl