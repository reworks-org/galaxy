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
	Audible::Audible() noexcept
		:m_buffer(), m_source()
	{
	}

	Audible::~Audible() noexcept
	{
		m_source.destroy();
		m_buffer.destroy();
	}

	Audible::Audible(const std::string& file) noexcept
	{
		load(file);
	}

	bool Audible::load(const std::string& file) noexcept
	{
		bool result = true;
		result = m_buffer.load(file);
		
		if (result)
		{
			m_source.bind(m_buffer);
		}

		return result;
	}

	bool Audible::load(const unsigned char* mem, const int size) noexcept
	{
		bool result = true;
		result = m_buffer.load(mem, size);

		if (result)
		{
			m_source.bind(m_buffer);
		}

		return result;
	}

	void Audible::setLooping(bool looping) noexcept
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