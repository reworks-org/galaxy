///
/// Source.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <algorithm>

#include "galaxy/error/Log.hpp"
#include "galaxy/error/OpenALError.hpp"

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
				GALAXY_LOG(GALAXY_FATAL, error::al_handle_error("Unable to gen source.", error));
			}
		}

		Source::~Source()
		{
			if (m_source > 0)
			{
				alDeleteSources(1, &m_source);
				m_source = 0;

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_FATAL, error::al_handle_error("Unable to delete source.", error));
				}
			}
		}

		void Source::queue(Buffer* buffer)
		{
			if (buffer != nullptr)
			{
				alSourcei(m_source, AL_BUFFER, buffer->handle());

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_handle_error("Unable to bind buffer.", error));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to pass a null buffer to an audio source.");
			}
		}

		void Source::queue(BufferStream* stream_buffer)
		{
			if (stream_buffer != nullptr)
			{
				alSourceQueueBuffers(m_source, 2, &stream_buffer->m_buffers[0]);

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_handle_error("Unable to queue stream buffer.", error));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to pass a null buffer to an audio source.");
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
				GALAXY_LOG(GALAXY_ERROR, error::al_handle_error("Unable to queue buffer(s).", error));
			}
		}

		void Source::queue(std::span<ALuint> buffer_array)
		{
			if (!buffer_array.empty())
			{
				alSourceQueueBuffers(m_source, static_cast<ALsizei>(buffer_array.size()), buffer_array.data());

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_handle_error("Unable to queue buffer(s).", error));
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Source recieved an empty buffer to be queue.");
			}
		}

		ALint Source::get_state() noexcept
		{
			ALint val;
			alGetSourcei(m_source, AL_SOURCE_STATE, &val);

			return val;
		}

		ALuint Source::handle() const noexcept
		{
			return m_source;
		}
	} // namespace audio
} // namespace galaxy