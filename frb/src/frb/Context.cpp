///
/// Context.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <stdexcept>

#include "frb/detail/Error.hpp"

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

	bool Context::initialize()
	{
		// This section explains itself kinda. Just init OpenAL and make sure there arent any issues.
		// Similar to OpenGL if your familiar.

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

	Context::~Context() noexcept
	{
		// Cleanup.
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_context);
		alcCloseDevice(m_device);
	}
}