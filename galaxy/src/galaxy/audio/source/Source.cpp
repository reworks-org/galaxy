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
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Unable to gen source."));
			}
		}

		Source::Source(Source&& s)
		{
			this->m_source = s.m_source;
			s.m_source     = 0;
		}

		Source& Source::operator=(Source&& s)
		{
			if (this != &s)
			{
				this->m_source = s.m_source;
				s.m_source     = 0;
			}

			return *this;
		}

		Source::~Source()
		{
			alDeleteSources(1, &m_source);
			m_source = 0;
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
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to bind buffer."));
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
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to queue stream buffer."));
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
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to queue buffer(s)."));
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
				if (alGetError() != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to queue buffer(s)."));
				}
			}
		}

		ALint Source::get_state()
		{
			ALint val = 0;
			alGetSourcei(m_source, AL_SOURCE_STATE, &val);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to get AL_SOURCE_STATE."));
			}

			return std::move(val);
		}

		const ALuint Source::handle() const
		{
			return m_source;
		}
	} // namespace audio
} // namespace galaxy