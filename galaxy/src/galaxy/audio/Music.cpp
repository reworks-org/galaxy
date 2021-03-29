///
/// Music.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"

#include "Music.hpp"

namespace galaxy
{
	namespace audio
	{
		Music::Music() noexcept
		    : m_looping {false}, m_running {false}
		{
		}

		Music::Music(std::string_view file)
		    : m_looping {false}, m_running {false}
		{
			if (!load(file))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load file: {0}.", file);
			}
		}

		Music::Music(const nlohmann::json& json)
		    : m_looping {false}, m_running {false}
		{
			if (load(json.at("file")))
			{
				set_looping(json.at("looping"));
				set_gain(json.at("volume"));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Unable to load file: {0}.", std::string {json.at("file")});
			}
		}

		Music::~Music()
		{
			m_running = false;

			alSourceStop(m_source.handle());
			m_thread.request_stop();
			m_thread.join();
		}

		void Music::play()
		{
			alSourcePlay(m_source.handle());
		}

		void Music::pause()
		{
			alSourcePause(m_source.handle());
		}

		void Music::stop()
		{
			alSourceStop(m_source.handle());

			alSourceUnqueueBuffers(m_source.handle(), 2, m_buffers.data());

			stb_vorbis_seek_start(m_stream);

			stb_vorbis_get_samples_short_interleaved(m_stream, m_info.channels, m_data, CHUNK);
			alBufferData(m_buffers[0], m_format, m_data, CHUNK * sizeof(short), m_info.sample_rate);

			stb_vorbis_get_samples_short_interleaved(m_stream, m_info.channels, m_data, CHUNK);
			alBufferData(m_buffers[1], m_format, m_data, CHUNK * sizeof(short), m_info.sample_rate);

			m_source.queue(this);
		}

		const bool Music::load(std::string_view file)
		{
			const auto res = internal_load(file);
			if (res)
			{
				set_max_distance(1000.0f);

				m_source.queue(this);
				m_running = true;

				// clang-format off
				m_thread = std::jthread([this]()
				{
					while (this->m_running)
					{
						this->update();
					}
				});
				// clang-format on
			}

			return res;
		}

		void Music::set_looping(const bool looping)
		{
			m_looping = looping;
		}

		const bool Music::get_looping()
		{
			return m_looping;
		}

		void Music::update()
		{
			int processed = 0;
			int amount    = 0;
			ALuint which  = 0;

			while (get_state() == AL_PLAYING)
			{
				alGetSourcei(m_source.handle(), AL_BUFFERS_PROCESSED, &processed);
				if (processed)
				{
					alSourceUnqueueBuffers(m_source.handle(), 1, &which);
					amount = stb_vorbis_get_samples_short_interleaved(m_stream, m_info.channels, m_data, CHUNK);
					if (amount == 0 && m_looping)
					{
						stb_vorbis_seek_start(m_stream);
						amount = stb_vorbis_get_samples_short_interleaved(m_stream, m_info.channels, m_data, CHUNK);
					}

					if (amount > 0)
					{
						alBufferData(which, m_format, m_data, amount * 2 * sizeof(short), m_info.sample_rate);
						alSourceQueueBuffers(m_source.handle(), 1, &which);
					}
				}
			}
		}
	} // namespace audio
} // namespace galaxy