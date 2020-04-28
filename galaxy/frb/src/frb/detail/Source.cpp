///
/// Source.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "frb/Error.hpp"

#include "Source.hpp"

///
/// Core namespace.
///
namespace frb
{
	Source::Source() noexcept
		:m_source(0)
	{
		// Create source and check for error. Only create 1 source since source is being managed per object.
		alGenSources(1, &m_source);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_FATAL, frb::parseError("Unable to gen source"));
		}
	}

	Source::~Source() noexcept
	{
		destroy();
	}

	void Source::setPitch(const float pitch) noexcept
	{
		alSourcef(m_source, AL_PITCH, std::abs(pitch));
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source pitch."));
		}
	}

	void Source::setGain(const float gain) noexcept
	{
		alSourcef(m_source, AL_GAIN, std::abs(gain));
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source gain."));
		}
	}

	void Source::setRolloffFactor(const float factor) noexcept
	{
		alSourcef(m_source, AL_ROLLOFF_FACTOR, factor);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source rolloff."));
		}
	}

	void Source::setMaxDistance(const float distance) noexcept
	{
		alSourcef(m_source, AL_MAX_DISTANCE, distance);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source max distance."));
		}
	}

	void Source::configureCone(const float outerGain, const float innerAngle, const float outerAngle) noexcept
	{
		alSourcef(m_source, AL_CONE_OUTER_GAIN, outerGain);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set AL_CONE_OUTER_GAIN."));
		}

		alSourcef(m_source, AL_CONE_INNER_ANGLE, innerAngle);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set AL_CONE_INNER_ANGLE."));
		}

		alSourcef(m_source, AL_CONE_OUTER_ANGLE, outerAngle);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set AL_CONE_OUTER_ANGLE."));
		}
	}

	void Source::setPosition(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_POSITION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source position."));
		}
	}

	void Source::setVelocity(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_VELOCITY, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source velocity."));
		}
	}

	void Source::setDirection(const float x, const float y, const float z) noexcept
	{
		alSource3f(m_source, AL_DIRECTION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source direction."));
		}
	}

	void Source::setLooping(bool looping) noexcept
	{
		alSourcei(m_source, AL_LOOPING, looping);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set source looping."));
		}
	}

	ALint Source::getState() noexcept
	{
		ALint val = 0;
		alGetSourcei(m_source, AL_SOURCE_STATE, &val);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to get AL_SOURCE_STATE."));
		}

		return std::move(val);
	}

	void Source::bind(const frb::Buffer& buffer) noexcept
	{
		alSourcei(m_source, AL_BUFFER, buffer.handle());
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to bind buffer: " + std::to_string(buffer.handle()) + " to source: " + std::to_string(m_source) + "."));
		}
	}

	void Source::queue(const frb::Buffer& buffer) noexcept
	{
		auto handle = buffer.handle();
		alSourceQueueBuffers(m_source, 1, &handle);
	}

	void Source::queue(const std::vector<frb::Buffer*>& buffers) noexcept
	{
		std::vector<ALuint> handles;
		handles.reserve(buffers.size());

		for (frb::Buffer* buff : buffers)
		{
			handles.push_back(buff->handle());
		}

		handles.shrink_to_fit();
		alSourceQueueBuffers(m_source, static_cast<ALsizei>(handles.size()), handles.data());
	}

	void Source::queue(const ALuint* bufferArray, const size_t size) noexcept
	{
		alSourceQueueBuffers(m_source, static_cast<ALsizei>(size), bufferArray);
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

	const ALuint Source::handle() const noexcept
	{
		return m_source;
	}

	void Source::destroy() noexcept
	{
		stop();
		alDeleteSources(1, &m_source);
	}
}