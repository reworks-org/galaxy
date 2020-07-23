///
/// Buffer.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <filesystem>

#include <AL/alext.h>
#include <pulsar/Log.hpp>
#include <vorbis/vorbisfile.h>

#include "frb/detail/Error.hpp"

#include "Buffer.hpp"

///
/// Core namespace.
///
namespace frb
{
	Buffer::Buffer()
	    : m_buffer {0}, m_channels {0}, m_frequency {0}
	{
		// Create buffer and check for error. Only create 1 bffer since buffer is being managed per object.
		alGenBuffers(1, &m_buffer);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_FATAL, frb::parse_error("Unable to gen audio buffer(s)."));
		}
	}

	Buffer::~Buffer() noexcept
	{
		destroy_buffer();
	}

	const ALint Buffer::get_frequency() const noexcept
	{
		return m_frequency;
	}

	const ALint Buffer::get_channels() const noexcept
	{
		return m_channels;
	}

	const ALuint Buffer::handle() const noexcept
	{
		return m_buffer;
	}

	bool Buffer::internal_load(std::string_view file)
	{
		bool result = true;

		// Process filepath properly.
		auto path = std::filesystem::path {file};
		if (path.extension() != ".ogg")
		{
			PL_LOG(PL_ERROR, "File must be ogg vorbis and have extension of .ogg!");
			result = false;
		}
		else
		{
			std::ifstream ifs;
			ifs.open(path.string(), std::ios::binary | std::ios::ate);
			if (!ifs.is_open())
			{
				PL_LOG(PL_ERROR, "Could not re-open streaming file: " + path.string());
				result = false;
			}
			else
			{
				const auto length = ifs.tellg();
				std::string buff(length, '\0'); // Doesnt like {} initialization.

				ifs.seekg(std::ios::beg);
				ifs.read(&buff[0], length);
				if (ifs.bad())
				{
					PL_LOG(PL_ERROR, "OGG stream has bad bit set: " + path.string());

					ifs.clear();
					ifs.close();

					result = false;
				}
				else
				{
					OggVorbis_File ogg_handle;
					if (ov_open_callbacks(buff.data(), &ogg_handle, nullptr, 0, OV_CALLBACKS_NOCLOSE) < 0)
					{
						PL_LOG(PL_ERROR, "Stream is not a valid OggVorbis stream: " + path.string());

						ov_clear(&ogg_handle);

						ifs.clear();
						ifs.close();

						result = false;
					}
					else
					{
						ALenum format     = 0;
						vorbis_info* info = ov_info(&ogg_handle, -1);

						m_frequency = info->rate;
						m_channels  = info->channels;
						if (m_channels >= 1 && m_channels < 4)
						{
							format = AL_FORMAT_STEREO16;
						}
						else if (m_channels == 4 || m_channels == 5)
						{
							format = AL_FORMAT_QUAD16;
						}
						else if (m_channels == 6)
						{
							format = AL_FORMAT_51CHN16;
						}
						else if (m_channels == 7)
						{
							format = AL_FORMAT_61CHN16;
						}
						else if (m_channels == 8)
						{
							format = AL_FORMAT_71CHN16;
						}
						else
						{
							format = AL_FORMAT_MONO16;
						}

						auto read_length = ov_pcm_total(&ogg_handle, -1) * m_channels * 2;
						std::string read_buff(read_length, '\0'); // Doesnt like {} initialization.
						for (std::size_t _size = 0, _offset = 0, _sel = 0;
						     (_size = ov_read(&ogg_handle, &read_buff[0] + _offset, 4096, 0, 2, 1, (int*)&_sel)) != 0;
						     _offset += _size)
						{
							if (_size < 0)
							{
								PL_LOG(PL_ERROR, "Invalid ogg file: " + path.string());
								result = false;
							}
						}

						if (result)
						{
							alBufferData(m_buffer, format, read_buff.data(), read_length, m_frequency);
							if (alGetError() != AL_NO_ERROR)
							{
								PL_LOG(PL_ERROR, frb::parse_error("Failed to buffer data for: " + path.string()));
							}
						}

						ov_clear(&ogg_handle);

						ifs.clear();
						ifs.close();
					}
				}
			}
		}

		return result;
	}

	void Buffer::destroy_buffer() noexcept
	{
		alDeleteBuffers(1, &m_buffer);
		m_buffer = 0;
	}
} // namespace frb