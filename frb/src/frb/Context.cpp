///
/// Context.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <stdexcept>

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

	void Context::initialize()
	{
		// This section explains itself kinda. Just init OpenAL and make sure there arent any issues.
		// Similar to OpenGL if your familiar.

		// Make sure error code is default flagged.
		alGetError();

		m_device = alcOpenDevice(alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER));
		if (!m_device)
		{
			throw std::runtime_error(frb::parseError("Failed to create OpenAL device."));
		}

		m_context = alcCreateContext(m_device, nullptr);
		if (!m_context)
		{
			throw std::runtime_error(frb::parseError("Failed to create OpenAL context."));
		}

		if (!alcMakeContextCurrent(m_context))
		{
			throw std::runtime_error(frb::parseError("Failed to make OpenAL context current."));
		}
	}

	void Context::setDopplerFactor(const float factor) noexcept
	{
		alDopplerFactor(factor);
	}

	void Context::setSpeedOfSound(const float speed) noexcept
	{
		alSpeedOfSound(speed);
	}
}