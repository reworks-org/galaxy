///
/// Source.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "frb/detail/Error.hpp"

#include "Source.hpp"

///
/// Core namespace.
///
namespace frb
{
	Source::Source()
	    : m_source {0}
	{
		// Create source and check for error. Only create 1 source since source is being managed per object.
		alGenSources(1, &m_source);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_FATAL, frb::parse_error("Unable to gen source."));
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
			PL_LOG(PL_WARNING, "Attempted to pass nullptr to source to be queued.");
		}
		else
		{
			alSourcei(m_source, AL_BUFFER, buffer->handle());
			if (alGetError() != AL_NO_ERROR)
			{
				PL_LOG(PL_ERROR, frb::parse_error("Unable to bind buffer."));
			}
		}
	}

	void Source::queue(BufferStream* stream_buffer)
	{
		if (stream_buffer == nullptr)
		{
			PL_LOG(PL_WARNING, "Attemped to pass nullptr stream_buffer to source to be queued.");
		}
		else
		{
			alSourceQueueBuffers(m_source, BufferStream::BUFFER_COUNT, &stream_buffer->get_data()->m_buffers[0]);
			if (alGetError() != AL_NO_ERROR)
			{
				auto msg = frb::parse_error("Unable to queue stream buffer: " + stream_buffer->get_data()->m_file_path.filename().string());
				PL_LOG(PL_ERROR, "{0}.", msg);
			}
		}
	}

	void Source::queue(std::span<frb::Buffer> buffers)
	{
		std::vector<ALuint> handles;
		handles.reserve(buffers.size());

		for (const auto& buff : buffers)
		{
			handles.emplace_back(buff.handle());
		}

		alSourceQueueBuffers(m_source, static_cast<ALsizei>(handles.size()), handles.data());
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to queue buffer(s)."));
		}
	}

	void Source::queue(std::span<ALuint> buffer_array)
	{
		if (buffer_array.empty())
		{
			PL_LOG(PL_WARNING, "Source recieved an empty buffer_array to queue.");
		}
		else
		{
			alSourceQueueBuffers(m_source, buffer_array.size(), buffer_array.data());
			if (alGetError() != AL_NO_ERROR)
			{
				PL_LOG(PL_ERROR, frb::parse_error("Unable to queue buffer(s)."));
			}
		}
	}

	ALint Source::get_state() noexcept
	{
		ALint val = 0;
		alGetSourcei(m_source, AL_SOURCE_STATE, &val);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to get AL_SOURCE_STATE."));
		}

		return std::move(val);
	}

	const ALuint Source::handle() const noexcept
	{
		return m_source;
	}
} // namespace frb