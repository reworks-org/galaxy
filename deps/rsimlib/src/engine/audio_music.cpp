
/** @file
 * @brief Implements threaded music-stream loading and playback.
 */

#include "audio.h"

#include "audio_backend.h"
#include "error.h"
#include "resource.h"

#include <algorithm>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <type_traits>

namespace sl
{

	/** Owns the SDL_mixer music object for one stream. */
	struct Stream
	{
		Mix_Music *music = nullptr;
	};

	namespace
	{

		/** Serializes SDL_mixer music operations on a worker thread. */
		class MusicWorker
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
				thread_ = std::thread([this]
									  { run(); });
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
			template <typename Function>
			auto call(Function &&function) -> decltype(function())
			{
				using Result = decltype(function());
				auto result = std::make_shared<std::promise<Result>>();
				auto future = result->get_future();
				enqueue([function = std::forward<Function>(function), result]() mutable
						{
			if constexpr (std::is_void_v<Result>) {
				function();
				result->set_value();
			} else {
				result->set_value(function());
			} });
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
						condition_.wait(lock, [this]
										{ return stopping_ || !commands_.empty(); });
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

		MusicWorker worker;
		int master_volume = 255;

		/** Convert the public 0-255 volume range to SDL_mixer's range. */
		int mixer_volume(int volume)
		{
			return std::clamp(volume, 0, 255) * MIX_MAX_VOLUME / 255;
		}

	} // namespace

	bool music_init()
	{
		const bool started = worker.start();
		if (!started)
			sl::detail::set_error(Mix_GetError());
		return started;
	}

	void music_shutdown()
	{
		worker.stop();
	}

	Stream *load_stream(const std::string &path)
	{
		if (!music_init())
		{
			return nullptr;
		}
		return worker.call([path]
						   {
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		Mix_Music* music = Mix_LoadMUS(path.c_str());
		if (!music) sl::detail::set_error(Mix_GetError());
		return music ? new Stream{music} : nullptr; });
	}

	Stream *load_stream_from_memory(const std::uint8_t *data, std::size_t size)
	{
		if (!data || size == 0 || size > std::numeric_limits<int>::max() || !music_init())
			return nullptr;
		std::vector<std::uint8_t> bytes(data, data + size);
		return worker.call([bytes = std::move(bytes)]
						   {
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		SDL_RWops* rw = SDL_RWFromConstMem(bytes.data(), static_cast<int>(bytes.size()));
		Mix_Music* music = rw ? Mix_LoadMUS_RW(rw, 1) : nullptr;
		if (!music) sl::detail::set_error(Mix_GetError());
		return music ? new Stream{music} : nullptr; });
	}

	Stream *load_stream(const Archive &archive, const std::string &name)
	{
		const auto bytes = archive.read(name);
		return load_stream_from_memory(bytes.data(), bytes.size());
	}

	void destroy_stream(Stream *stream)
	{
		if (!stream || !music_init())
		{
			return;
		}
		worker.call([stream]
					{
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		Mix_HaltMusic();
		Mix_FreeMusic(stream->music);
		delete stream; });
	}

	void play_stream(Stream *stream, int loops)
	{
		if (!stream || !music_init())
		{
			return;
		}
		worker.enqueue([stream, loops]
					   {
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		Mix_VolumeMusic(mixer_volume(master_volume));
		Mix_PlayMusic(stream->music, loops); });
	}

	void stop_stream()
	{
		if (!music_init())
		{
			return;
		}
		worker.enqueue([]
					   {
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		Mix_HaltMusic(); });
	}

	void pause_stream()
	{
		if (!music_init())
		{
			return;
		}
		worker.enqueue([]
					   {
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		Mix_PauseMusic(); });
	}

	void resume_stream()
	{
		if (!music_init())
		{
			return;
		}
		worker.enqueue([]
					   {
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		Mix_ResumeMusic(); });
	}

	void music_set_volume(int volume)
	{
		if (!music_init())
		{
			return;
		}
		worker.enqueue([volume]
					   {
		master_volume = std::clamp(volume, 0, 255);
		std::lock_guard<std::mutex> lock(audio_detail::mixer_mutex());
		Mix_VolumeMusic(mixer_volume(master_volume)); });
	}

} // namespace sl
