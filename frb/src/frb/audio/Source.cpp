///
/// Source.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <stdexcept>

#include "frb/detail/Error.hpp"

#include "Source.hpp"

///
/// Core namespace.
///
namespace frb
{
	Source::Source()
		:m_source(0)
	{
		// Create source and check for error. Only create 1 source since source is being managed per object.
		alGenSources(1, &m_source);
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to gen source"));
		}
	}

	Source::~Source() noexcept
	{
		alDeleteSources(1, &m_source);
	}

	void Source::setPitch(const float pitch)
	{
		alSourcef(m_source, AL_PITCH, std::abs(pitch));
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to set source pitch."));
		}
	}

	void Source::setGain(const float gain)
	{
		alSourcef(m_source, AL_GAIN, std::abs(gain));
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to set source gain."));
		}
	}

	void Source::setPosition(const float x, const float y, const float z)
	{
		alSource3f(m_source, AL_POSITION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to set source position."));
		}
	}

	void Source::setVelocity(const float x, const float y, const float z)
	{
		alSource3f(m_source, AL_VELOCITY, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to set source velocity."));
		}
	}

	void Source::setDirection(const float x, const float y, const float z)
	{
		alSource3f(m_source, AL_DIRECTION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to set source direction."));
		}
	}

	void Source::setLooping(bool looping)
	{
		alSourcei(m_source, AL_LOOPING, looping);
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to set source looping."));
		}
	}

	void Source::bind(const frb::Buffer& buffer)
	{
		alSourcei(m_source, AL_BUFFER, buffer.handle());
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to bind buffer: " + std::to_string(buffer.handle()) + " to source: " + std::to_string(m_source) + "."));
		}
	}

	const ALuint Source::handle() const noexcept
	{
		return m_source;
	}
}