///
/// Sound.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "Sound.hpp"

///
/// Core namespace.
///
namespace frb
{
	Sound::Sound() noexcept
	    : Buffer {}, SourceManipulator {}, m_source {}
	{
		set_source_to_manipulate(m_source.handle());
	}

	Sound::Sound(std::string_view file)
	{
		if (!load(file))
		{
			PL_LOG(PL_FATAL, "Failed to load sound file: {0}.", file);
		}
	}

	Sound::~Sound() noexcept
	{
		destroy();
	}

	bool Sound::load(std::string_view file)
	{
		auto res = internal_load(file);

		if (res)
		{
			m_source.queue(dynamic_cast<Buffer*>(this));
		}

		return res;
	}

	void Sound::destroy() noexcept
	{
		m_source.destroy_source();
		destroy_buffer();
	}
} // namespace frb