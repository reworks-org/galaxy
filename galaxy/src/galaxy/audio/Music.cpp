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
		    : m_running {false}
		{
			set_source_to_manipulate(m_source.handle());
		}

		Music::Music(std::string_view file)
		    : m_running {false}
		{
			if (!load(file))
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to load file: {0}.", file);
			}
		}

		Music::Music(const nlohmann::json& json)
		    : m_running {false}
		{
			if (load(json.at("file")))
			{
				set_looping(json.at("loop"));
				set_pitch(json.at("pitch"));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Unable to load file: {0}.", std::string {json.at("file")});
			}
		}

		Music::~Music()
		{
			m_running = false;

			stop();
			m_thread.request_stop();
			m_thread.join();
		}

		const bool Music::load(std::string_view file)
		{
			const auto res = internal_load(file);
			if (res)
			{
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