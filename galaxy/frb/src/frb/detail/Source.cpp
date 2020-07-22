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

	Source::~Source() noexcept
	{
		destroy();
	}

	void Source::queue(frb::Buffer* buffer) noexcept
	{
		alSourcei(m_source, AL_BUFFER, buffer->handle());
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to bind buffer: " + std::to_string(buffer->handle()) + " to source: " + std::to_string(m_source)));
		}
	}

	void Source::queue(BufferStream* stream_buffer)
	{
		alSourceQueueBuffers(m_source, BufferStream::buffer_count, &stream_buffer->get_data()->m_buffers[0]);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to queue stream buffer: " + stream_buffer->get_data()->m_file_path.filename().string()));
		}
	}

	void Source::queue(const std::vector<frb::Buffer>& buffers)
	{
		std::vector<ALuint> handles;
		handles.reserve(buffers.size());

		std::transform(buffers.begin(), buffers.end(), std::back_inserter(handles), [](frb::Buffer& buffer) {
			return buffer.handle();
		});

		alSourceQueueBuffers(m_source, static_cast<ALsizei>(handles.size()), handles.data());
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to queue buffer(s)"));
		}
	}

	void Source::queue(const ALuint* buffer_array, const size_t size) noexcept
	{
		alSourceQueueBuffers(m_source, static_cast<ALsizei>(size), buffer_array);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to queue buffer(s)"));
		}
	}

	void Source::play() noexcept
	{
		alSourcePlay(m_source);
	}

	void Source::pause() noexcept
	{
		alSourcePause(m_source);
	}

	void Source::stop() noexcept
	{
		alSourceStop(m_source);
	}

	void Source::rewind() noexcept
	{
		alSourceRewind(m_source);
	}

	void Source::set_pitch(pr::not_negative auto pitch) noexcept
	{
		alSourcef(m_source, AL_PITCH, std::abs(pitch));
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source pitch."));
		}
	}

	void Source::set_gain(pr::not_negative auto gain) noexcept
	{
		alSourcef(m_source, AL_GAIN, std::abs(gain));
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source gain."));
		}
	}

	void Source::set_rolloff_factor(pr::not_negative auto factor) noexcept
	{
		alSourcef(m_source, AL_ROLLOFF_FACTOR, factor);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source rolloff."));
		}
	}

	void Source::set_max_distance(const float distance) noexcept
	{
		alSourcef(m_source, AL_MAX_DISTANCE, distance);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source max distance."));
		}
	}

	void Source::set_cone(const float outer_gain, const float inner_angle, const float outer_angle) noexcept
	{
		alSourcef(m_source, AL_CONE_OUTER_GAIN, outerGain);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set AL_CONE_OUTER_GAIN."));
		}

		alSourcef(m_source, AL_CONE_INNER_ANGLE, innerAngle);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set AL_CONE_INNER_ANGLE."));
		}

		alSourcef(m_source, AL_CONE_OUTER_ANGLE, outerAngle);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set AL_CONE_OUTER_ANGLE."));
		}
	}

	void Source::set_position(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_POSITION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source position."));
		}
	}

	void Source::set_velocity(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_VELOCITY, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source velocity."));
		}
	}

	void Source::set_direction(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_DIRECTION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source direction."));
		}
	}

	void Source::set_looping(bool looping) noexcept
	{
		alSourcei(m_source, AL_LOOPING, looping);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parse_error("Unable to set source looping."));
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

	void Source::destroy() noexcept
	{
		stop();
		alDeleteSources(1, &m_source);
	}
} // namespace frb