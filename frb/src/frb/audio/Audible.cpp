///
/// Audible.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include "Audible.hpp"

///
/// Core namespace.
///
namespace frb
{
	Audible::Audible()
		:m_buffer(), m_source()
	{
	}

	Audible::~Audible() noexcept
	{
		m_source.destroy();
		m_buffer.destroy();
	}

	void Audible::loadFromFile(const std::string& file)
	{
		m_buffer.loadFromFile(file);
		m_source.bind(m_buffer);
	}

	void Audible::loadFromMemory(const unsigned char* mem, const int size)
	{
		m_buffer.loadFromMemory(mem, size);
		m_source.bind(m_buffer);
	}

	void Audible::setLooping(bool looping)
	{
		m_source.setLooping(looping);
	}

	void Audible::play() noexcept
	{
		m_source.play();
	}

	void Audible::pause() noexcept
	{
		m_source.pause();
	}

	void Audible::stop() noexcept
	{
		m_source.stop();
	}

	void Audible::rewind() noexcept
	{
		m_source.rewind();
	}

	frb::Buffer& Audible::buffer() noexcept
	{
		return m_buffer;
	}

	frb::Source& Audible::source() noexcept
	{
		return m_source;
	}
}