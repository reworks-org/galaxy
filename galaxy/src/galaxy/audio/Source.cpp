///
/// Source.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"

#include "Source.hpp"

namespace galaxy
{
	namespace audio
	{
		Source::Source()
		    : m_source {0}
		{
			// Create source and check for error. Only create 1 source since source is being managed per object.
			alGenSources(1, &m_source);

			const auto error = alGetError();
			if (error != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Unable to gen source.", error));
			}
		}

		Source::~Source()
		{
			if (m_source > 0)
			{
				alDeleteSources(1, &m_source);
				m_source = 0;
			}
		}

		void Source::queue(Buffer* buffer)
		{
			if (buffer == nullptr)
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to pass nullptr to source to be queued.");
			}
			else
			{
				alSourcei(m_source, AL_BUFFER, buffer->handle());

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to bind buffer.", error));
				}
			}
		}

		void Source::queue(BufferStream* stream_buffer)
		{
			if (stream_buffer == nullptr)
			{
				GALAXY_LOG(GALAXY_WARNING, "Attemped to pass nullptr stream_buffer to source to be queued.");
			}
			else
			{
				alSourceQueueBuffers(m_source, 2, &stream_buffer->m_buffers[0]);

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to queue stream buffer.", error));
				}
			}
		}

		void Source::queue(std::span<Buffer> buffers)
		{
			std::vector<ALuint> handles;
			handles.reserve(buffers.size());

			std::transform(buffers.begin(), buffers.end(), std::back_inserter(handles), [](Buffer& buffer) {
				return buffer.handle();
			});

			alSourceQueueBuffers(m_source, static_cast<ALsizei>(handles.size()), handles.data());

			const auto error = alGetError();
			if (error != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to queue buffer(s).", error));
			}
		}

		void Source::queue(std::span<ALuint> buffer_array)
		{
			if (buffer_array.empty())
			{
				GALAXY_LOG(GALAXY_WARNING, "Source recieved an empty buffer_array to queue.");
			}
			else
			{
				alSourceQueueBuffers(m_source, static_cast<ALsizei>(buffer_array.size()), buffer_array.data());

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to queue buffer(s).", error));
				}
			}
		}

		ALint Source::get_state()
		{
			ALint val = 0;
			alGetSourcei(m_source, AL_SOURCE_STATE, &val);

			const auto error = alGetError();
			if (error != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get AL_SOURCE_STATE.", error));
			}

			return val;
		}

		const ALuint Source::handle() const noexcept
		{
			return m_source;
		}
	} // namespace audio
} // namespace galaxy