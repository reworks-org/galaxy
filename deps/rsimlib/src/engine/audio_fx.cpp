
/** @file
 * @brief Implements threaded sound-effect loading and playback.
 */

#include "audio.h"

#include "audio_backend.h"
#include "error.h"
#include "resource.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <cstring>
#include <limits>
#include <vector>

namespace sl
{

/** Owns the SDL_mixer chunk backing one sound effect. */
struct Sample
{
    Mix_Chunk *chunk = nullptr;
};

namespace
{

/** Serializes SDL_mixer sound-effect operations on a worker thread. */
class FxWorker
{
  public:
    /** Start the worker and acquire the shared mixer. */
    bool start()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (thread_.joinable())
        {
            return true;
        }
        if (!audio_detail::acquire_mixer())
        {
            return false;
        }
        stopping_ = false;
        thread_ = std::thread([this] { run(); });
        return true;
    }

    /** Stop the worker after draining queued commands. */
    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!thread_.joinable())
            {
                return;
            }
            stopping_ = true;
        }
        condition_.notify_one();
        thread_.join();
        audio_detail::release_mixer();
    }

    /** Run a command synchronously on the worker thread. */
    template <typename Function> auto call(Function &&function) -> decltype(function())
    {
        using Result = decltype(function());
        auto result = std::make_shared<std::promise<Result>>();
        auto future = result->get_future();
        enqueue([function = std::forward<Function>(function), result]() mutable {
            if constexpr (std::is_void_v<Result>)
            {
                function();
                result->set_value();
            }
            else
            {
                result->set_value(function());
            }
        });
        return future.get();
    }

    /** Queue a command for asynchronous execution. */
    void enqueue(std::function<void()> command)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            commands_.push_back(std::move(command));
        }
        condition_.notify_one();
    }

  private:
    /** Process queued commands until shutdown is requested. */
    void run()
    {
        for (;;)
        {
            std::function<void()> command;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                condition_.wait(lock, [this] { return stopping_ || !commands_.empty(); });
                if (commands_.empty() && stopping_)
                {
                    return;
                }
                command = std::move(commands_.front());
                commands_.pop_front();
            }
            command();
        }
    }

    std::mutex mutex_;
    std::condition_variable condition_;
    std::deque<std::function<void()>> commands_;
    std::thread thread_;
    bool stopping_ = false;
};

FxWorker worker;
std::atomic<std::uint64_t> next_voice{1};
std::unordered_map<std::uint64_t, int> voices;
int master_volume = 255;

/** Convert the public 0-255 volume range to SDL_mixer's range. */
int mixer_volume(int volume)
{
    return std::clamp(volume, 0, 255) * MIX_MAX_VOLUME / 255;
}

/** Apply the public 0-255 pan range to one mixer channel. */
void apply_pan(int channel, int pan)
{
    pan = std::clamp(pan, 0, 255);
    const Uint8 left = static_cast<Uint8>(pan <= 128 ? 255 : 255 - ((pan - 128) * 255 / 127));
    const Uint8 right = static_cast<Uint8>(pan >= 128 ? 255 : pan * 255 / 128);
    Mix_SetPanning(channel, left, right);
}

} // namespace

bool audio_fx_init()
{
    const bool started = worker.start();
    if (!started)
        sl::detail::set_error(Mix_GetError());
    return started;
}

void audio_fx_shutdown()
{
    worker.stop();
}

Sample *load_sample(const std::string &path)
{
    if (!audio_fx_init())
    {
        return nullptr;
    }
    return worker.call([path] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        Mix_Chunk *chunk = Mix_LoadWAV(path.c_str());
        if (!chunk)
            sl::detail::set_error(Mix_GetError());
        return chunk ? new Sample{chunk} : nullptr;
    });
}

Sample *load_sample_from_memory(const std::uint8_t *data, std::size_t size)
{
    if (!data || size == 0 || size > std::numeric_limits<int>::max() || !audio_fx_init())
        return nullptr;
    std::vector<std::uint8_t> bytes(data, data + size);
    return worker.call([bytes = std::move(bytes)] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        SDL_RWops *rw = SDL_RWFromConstMem(bytes.data(), static_cast<int>(bytes.size()));
        Mix_Chunk *chunk = rw ? Mix_LoadWAV_RW(rw, 1) : nullptr;
        if (!chunk)
            sl::detail::set_error(Mix_GetError());
        return chunk ? new Sample{chunk} : nullptr;
    });
}
Sample *create_sample_from_pcm(const std::int16_t *data, std::size_t frames, int sample_rate, int channels)
{
    if (!data || frames == 0 || sample_rate <= 0 || (channels != 1 && channels != 2) ||
        frames > std::numeric_limits<std::size_t>::max() / (sizeof(std::int16_t) * static_cast<std::size_t>(channels)))
    {
        return nullptr;
    }

    if (!audio_fx_init())
        return nullptr;

    const std::size_t size = frames * sizeof(std::int16_t) * static_cast<std::size_t>(channels);

    if (size > std::numeric_limits<std::uint32_t>::max())
        return nullptr;

    std::vector<std::uint8_t> bytes(size);
    std::memcpy(bytes.data(), data, size);

    return worker.call([bytes = std::move(bytes), sample_rate, channels]() -> Sample * {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());

        int device_rate = 0;
        Uint16 device_format = 0;
        int device_channels = 0;
        if (!Mix_QuerySpec(&device_rate, &device_format, &device_channels))
        {
            sl::detail::set_error(Mix_GetError());
            return nullptr;
        }

        SDL_AudioCVT cvt;
        const int needs_conversion =
            SDL_BuildAudioCVT(&cvt, AUDIO_S16SYS, static_cast<Uint8>(channels), sample_rate, device_format,
                              static_cast<Uint8>(device_channels), device_rate);
        if (needs_conversion < 0)
        {
            sl::detail::set_error(SDL_GetError());
            return nullptr;
        }

        const std::size_t capacity = bytes.size() * static_cast<std::size_t>(needs_conversion ? cvt.len_mult : 1);
        // Mix_FreeChunk releases abuf with SDL_free, so it must come from SDL_malloc.
        auto *buffer = static_cast<std::uint8_t *>(SDL_malloc(capacity));
        if (!buffer)
        {
            sl::detail::set_error("Out of memory");
            return nullptr;
        }
        std::memcpy(buffer, bytes.data(), bytes.size());

        Uint32 length = static_cast<Uint32>(bytes.size());
        if (needs_conversion)
        {
            cvt.buf = buffer;
            cvt.len = static_cast<int>(bytes.size());
            if (SDL_ConvertAudio(&cvt) < 0)
            {
                sl::detail::set_error(SDL_GetError());
                SDL_free(buffer);
                return nullptr;
            }
            length = static_cast<Uint32>(cvt.len_cvt);
        }

        auto *chunk = static_cast<Mix_Chunk *>(SDL_calloc(1, sizeof(Mix_Chunk)));
        if (!chunk)
        {
            SDL_free(buffer);
            sl::detail::set_error("Out of memory");
            return nullptr;
        }
        chunk->allocated = 1;
        chunk->abuf = buffer;
        chunk->alen = length;
        chunk->volume = MIX_MAX_VOLUME;
        return new Sample{chunk};
    });
}
Sample *load_sample(const Archive &archive, const std::string &name)
{
    const auto bytes = archive.read(name);
    return load_sample_from_memory(bytes.data(), bytes.size());
}

void destroy_sample(Sample *sample)
{
    if (!sample || !audio_fx_init())
    {
        return;
    }
    worker.call([sample] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        for (auto voice = voices.begin(); voice != voices.end();)
        {
            if (Mix_GetChunk(voice->second) == sample->chunk)
            {
                Mix_HaltChannel(voice->second);
                voice = voices.erase(voice);
            }
            else
            {
                ++voice;
            }
        }
        Mix_FreeChunk(sample->chunk);
        delete sample;
    });
}

std::uint64_t play_sample(Sample *sample, int volume, int pan, int frequency, int loops)
{
    if (!sample || !audio_fx_init())
    {
        return 0;
    }
    (void)frequency;
    const std::uint64_t voice = next_voice++;
    worker.enqueue([sample, volume, pan, loops, voice] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        const int channel = Mix_PlayChannel(-1, sample->chunk, loops);
        if (channel < 0)
        {
            return;
        }
        voices[voice] = channel;
        Mix_Volume(channel, mixer_volume(volume) * master_volume / 255);
        apply_pan(channel, pan);
    });
    return voice;
}

void stop_voice(std::uint64_t voice)
{
    if (!audio_fx_init())
    {
        return;
    }
    worker.enqueue([voice] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        const auto found = voices.find(voice);
        if (found != voices.end())
        {
            Mix_HaltChannel(found->second);
            voices.erase(found);
        }
    });
}

void stop_sample(Sample *sample)
{
    if (!sample || !audio_fx_init())
    {
        return;
    }
    worker.enqueue([sample] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        for (auto voice = voices.begin(); voice != voices.end();)
        {
            if (Mix_GetChunk(voice->second) == sample->chunk)
            {
                Mix_HaltChannel(voice->second);
                voice = voices.erase(voice);
            }
            else
            {
                ++voice;
            }
        }
    });
}

void stop_all_samples()
{
    if (!audio_fx_init())
        return;
    worker.enqueue([] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        Mix_HaltChannel(-1);
        voices.clear();
    });
}

bool voice_is_playing(std::uint64_t voice)
{
    if (!audio_fx_init())
        return false;
    return worker.call([voice] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        const auto found = voices.find(voice);
        return found != voices.end() && Mix_Playing(found->second) != 0;
    });
}

void set_pan(std::uint64_t voice, int pan)
{
    if (!audio_fx_init())
        return;
    worker.enqueue([voice, pan] {
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        const auto found = voices.find(voice);
        if (found != voices.end())
            apply_pan(found->second, pan);
    });
}

void audio_fx_set_volume(int volume)
{
    if (!audio_fx_init())
    {
        return;
    }
    worker.enqueue([volume] {
        master_volume = std::clamp(volume, 0, 255);
        std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
        Mix_Volume(-1, mixer_volume(master_volume));
    });
}

} // namespace sl
