///
/// BufferStream.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <AL/alext.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/ALError.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "BufferStream.hpp"

namespace galaxy
{
	namespace audio
	{
		BufferStream::BufferStream()
			: m_buffers {0, 0}
			, m_data {nullptr}
			, m_stream {nullptr}
			, m_format {0}
		{
			alGenBuffers(2, &m_buffers[0]);

			const auto error = alGetError();
			if (error != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_parse_error("Unable to gen audio buffer(s).", error));
			}
		}

		BufferStream::~BufferStream()
		{
			if (m_buffers[0] > 0 || m_buffers[1] > 0)
			{
				if (m_stream != nullptr)
				{
					stb_vorbis_close(m_stream);
					m_stream = nullptr;
				}

				if (m_data != nullptr)
				{
					delete[] m_data;
					m_data = nullptr;
				}

				m_info   = {};
				m_format = 0;

				alDeleteBuffers(2, &m_buffers[0]);

				const auto error = alGetError();
				if (error != AL_NO_ERROR)
				{
					GALAXY_LOG(GALAXY_ERROR, error::al_parse_error("Unable to delete audio buffer(s).", error));
				}

				m_buffers = {0, 0};
			}
		}

		const bool BufferStream::internal_load(std::string_view file)
		{
			bool       result = true;
			const auto path   = SL_HANDLE.vfs()->absolute(file);

			if (path == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find file to load into bufferstream: {0}.", file);
				result = false;
			}
			else
			{
				const auto path_str = path.value();
				if (std::filesystem::path(path_str).extension() != ".ogg")
				{
					GALAXY_LOG(GALAXY_ERROR, "Music must be ogg vorbis and have extension of .ogg!");
					result = false;
				}
				else
				{
					m_stream = stb_vorbis_open_filename(path_str.c_str(), nullptr, nullptr);
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
			}

			return result;
		}
	} // namespace audio
} // namespace galaxy