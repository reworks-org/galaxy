///
/// Playlist.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include "Playlist.hpp"

///
/// Core namespace.
///
namespace frb
{
	Playlist::Playlist() noexcept
		:m_source()
	{
	}

	Playlist::~Playlist() noexcept
	{
		m_source.destroy();
		m_buffers.destroy();
	}

	bool Playlist::load(const std::vector<std::string>& files) noexcept
	{
		bool result = true;
		result = m_buffers.load(files);

		if (result)
		{
			m_source.queue(m_buffers.raw().data(), m_buffers.raw().size());
		}

		return result;
	}

	bool Playlist::load(const std::vector<std::pair<unsigned char*, const int>>& data) noexcept
	{
		bool result = true;
		result = m_buffers.load(data);

		if (result)
		{
			m_source.queue(m_buffers.raw().data(), m_buffers.raw().size());
		}

		return result;
	}

	void Playlist::setLooping(bool looping) noexcept
	{
		m_source.setLooping(looping);
	}

	void Playlist::play() noexcept
	{
		m_source.play();
	}

	void Playlist::pause() noexcept
	{
		m_source.pause();
	}

	void Playlist::stop() noexcept
	{
		m_source.stop();
	}

	void Playlist::rewind() noexcept
	{
		m_source.rewind();
	}

	frb::Source& Playlist::source() noexcept
	{
		return m_source;
	}
}