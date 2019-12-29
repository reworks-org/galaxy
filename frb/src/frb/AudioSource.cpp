///
/// AudioSource.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <stdexcept>

#include "frb/detail/Error.hpp"

#include "AudioSource.hpp"

///
/// Core namespace.
///
namespace frb
{
	AudioSource::AudioSource()
		:m_source(0)
	{
		// Create source and check for error. Only create 1 source since source is being managed per object.
		alGenSources(1, &m_source);
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to gen AudioSource"));
		}
	}

	AudioSource::~AudioSource() noexcept
	{
	}

	void AudioSource::setPitch(const float pitch)
	{
	}

	void AudioSource::setGain(const float gain)
	{
	}

	void AudioSource::setPosition(const float x, const float y, const float z)
	{
	}

	void AudioSource::setVelocity(const float x, const float y, const float z)
	{
	}

	void AudioSource::setLooping(bool looping)
	{
	}
}