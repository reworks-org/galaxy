///
/// BufferStream.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <filesystem>

#include <AL/alext.h>

#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"

#include "BufferStream.hpp"

namespace galaxy
{
	namespace audio
	{
		BufferStream::BufferStream()
		    : m_buffers {0, 0}, m_data {nullptr}, m_stream {nullptr}
		{
			alGenBuffers(2, &m_buffers[0]);
			if (alGetError() != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Unable to gen audio buffer(s)."));
			}
		}

		BufferStream::BufferStream(BufferStream&& bs)
		{
			this->m_buffers = std::move(bs.m_buffers);
			this->m_data    = bs.m_data;
			this->m_format  = bs.m_format;
			this->m_info    = bs.m_info;
			this->m_stream  = bs.m_stream;

			bs.m_buffers = {0, 0};
			bs.m_data    = nullptr;
			bs.m_stream  = nullptr;
		}

		BufferStream& BufferStream::operator=(BufferStream&& bs)
		{
			if (this != &bs)
			{
				this->m_buffers = std::move(bs.m_buffers);
				this->m_data    = bs.m_data;
				this->m_format  = bs.m_format;
				this->m_info    = bs.m_info;
				this->m_stream  = bs.m_stream;

				bs.m_buffers = {0, 0};
				bs.m_data    = nullptr;
				bs.m_stream  = nullptr;
			}

			return *this;
		}

		BufferStream::~BufferStream()
		{
			alDeleteBuffers(2, &m_buffers[0]);

			if (m_stream != nullptr)
			{
				stb_vorbis_close(m_stream);
			}

			if (m_data != nullptr)
			{
				delete[] m_data;
			}
		}

		bool BufferStream::internal_load(std::string_view file)
		{
			bool result = true;

			auto path = std::filesystem::path {file};
			if (path.extension() != ".ogg")
			{
				GALAXY_LOG(GALAXY_ERROR, "Music must be ogg vorbis and have extension of .ogg!");
				result = false;
			}
			else
			{
				m_stream = stb_vorbis_open_filename(path.string().c_str(), nullptr, nullptr);
				if (!m_stream)
				{
					GALAXY_LOG(GALAXY_ERROR, "STB failed to load: {0}.", file);
					result = false;
				}
				else
				{
					m_info = stb_vorbis_get_info(m_stream);

					m_data   = new short[CHUNK];
					m_format = (m_info.channels > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;

					stb_vorbis_get_samples_short_interleaved(m_stream, m_info.channels, m_data, CHUNK);
					alBufferData(m_buffers[0], m_format, m_data, CHUNK * sizeof(short), m_info.sample_rate);

					stb_vorbis_get_samples_short_interleaved(m_stream, m_info.channels, m_data, CHUNK);
					alBufferData(m_buffers[1], m_format, m_data, CHUNK * sizeof(short), m_info.sample_rate);
				}
			}

			return result;
		}
	} // namespace audio
} // namespace galaxy