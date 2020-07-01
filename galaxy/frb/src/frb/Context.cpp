///
/// Context.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "frb/Error.hpp"

#include "Context.hpp"

///
/// Core namespace.
///
namespace frb
{
	Context::Context() noexcept
		:m_device(nullptr), m_context(nullptr)
	{
	}

	Context::~Context() noexcept
	{
		// Cleanup.
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}

	void Context::initialize() noexcept
	{
		// This section explains itself kinda. Just init OpenAL and make sure there arent any issues.
		// Similar to OpenGL if your familiar.

		// Make sure error code is default flagged.
		alGetError();

		m_device = alcOpenDevice(alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER));
		if (!m_device)
		{
			PL_LOG(PL_FATAL, frb::parseError("Failed to create OpenAL device."));
		}

		m_context = alcCreateContext(m_device, nullptr);
		if (!m_context)
		{
			PL_LOG(PL_FATAL, frb::parseError("Failed to create OpenAL context."));
		}

		if (!alcMakeContextCurrent(m_context))
		{
			PL_LOG(PL_FATAL, frb::parseError("Failed to make OpenAL context current."));
		}
	}

	void Context::setDopplerFactor(const float factor) noexcept
	{
		alDopplerFactor(factor);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set context doppler factor."));
		}
	}

	void Context::setSpeedOfSound(const float speed) noexcept
	{
		alSpeedOfSound(speed);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set context sound speed."));
		}
	}

	void Context::setListenerGain(const float gain) noexcept
	{
		if (gain < 0.0f)
		{
			return;
		}

		alListenerf(AL_GAIN, gain);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set listener AL_GAIN."));
		}
	}

	void Context::setListenerPosition(const float x, const float y, const float z) noexcept
	{
		alListener3f(AL_POSITION, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set listener AL_POSITION."));
		}
	}

	void Context::setListenerVelocity(const float x, const float y, const float z) noexcept
	{
		alListener3f(AL_VELOCITY, x, y, z);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set listener AL_VELOCITY."));
		}
	}

	void Context::setListenerOrientation(const float atX, const float atY, const float atZ, const float upX, const float upY, const float upZ) noexcept
	{
		const float asArray[6] = {atX, atY, atZ, upX, upY, upZ};
		alListenerfv(AL_ORIENTATION, asArray);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_ERROR, frb::parseError("Unable to set listener AL_ORIENTATION."));
		}
	}
}