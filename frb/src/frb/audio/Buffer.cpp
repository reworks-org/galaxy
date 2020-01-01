///
/// Buffer.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <stdexcept>

#include "frb/detail/Error.hpp"

#include "Buffer.hpp"

namespace frb
{
	Buffer::Buffer()
		:m_buffer(0)
	{
		// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
		alGenBuffers(1, &m_buffer);
		if (alGetError() != AL_NO_ERROR)
		{
			throw std::runtime_error(frb::parseError("Unable to gen audio buffer."));
		}
	}

	Buffer::~Buffer() noexcept
	{
	}

	void Buffer::load(const std::string& file)
	{
		// alBufferData(buffer, format, data, size, freq);
		// check for errors
	}

	const ALuint Buffer::handle() const noexcept
	{
		return m_buffer;
	}
}