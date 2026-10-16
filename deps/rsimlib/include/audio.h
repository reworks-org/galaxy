#pragma once

#include <cstdint>
#include <string>

namespace sl
{

class Archive;

struct Sample;

/** Sound effects use Allegro 4's 0-255 volume and pan conventions. */
/** Initialize the sound effects mixer. */
bool audio_fx_init();
/** Shut down sound effects and release mixer resources. */
void audio_fx_shutdown();

/** Load a sample from an audio file. */
Sample *load_sample(const std::string &path);
/** Load a sound effect from memory. */
Sample *load_sample_from_memory(const std::uint8_t *data, std::size_t size);
/**
 * Create a sound effect from signed 16-bit PCM data.
 *
 * The PCM data is copied, so the caller may release its buffer
 * after this function returns.
 */
Sample *create_sample_from_pcm(const std::int16_t *data, std::size_t frames, int sample_rate, int channels);

/** Load a sound-effect entry from a ZIP archive. */
Sample *load_sample(const Archive &archive, const std::string &name);
/** Destroy a sample returned by load_sample(). */
void destroy_sample(Sample *sample);

/** Play a sample and return its mixer voice identifier. */
std::uint64_t play_sample(Sample *sample, int volume = 255, int pan = 128, int frequency = 1000, int loops = 0);
/** Stop one mixer voice. */
void stop_voice(std::uint64_t voice);
/** Stop every voice currently playing the sample. */
void stop_sample(Sample *sample);
/** Stop all active sound-effect voices. */
void stop_all_samples();
/** Return whether a voice is currently playing. */
bool voice_is_playing(std::uint64_t voice);
/** Set a voice's pan using the 0-255 Allegro convention. */
void set_pan(std::uint64_t voice, int pan);
/** Set the global sound effects volume. */
void audio_fx_set_volume(int volume);

struct Stream;

/** Initialize the music mixer. */
bool music_init();
/** Shut down music playback and release mixer resources. */
void music_shutdown();

/** Load a music stream from a file. */
Stream *load_stream(const std::string &path);
/** Load a music stream from memory. */
Stream *load_stream_from_memory(const std::uint8_t *data, std::size_t size);
/** Load a music-stream entry from a ZIP archive. */
Stream *load_stream(const Archive &archive, const std::string &name);
/** Destroy a stream returned by load_stream(). */
void destroy_stream(Stream *stream);

/** Start playback, looping forever by default. */
void play_stream(Stream *stream, int loops = -1);
/** Stop the current music stream. */
void stop_stream();
/** Pause the current music stream. */
void pause_stream();
/** Resume paused music playback. */
void resume_stream();
/** Set the global music volume. */
void music_set_volume(int volume);

} // namespace sl
