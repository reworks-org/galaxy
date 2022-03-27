///
/// BufferStream.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <AL/alext.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/error/OpenALError.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

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
			, m_info {}
		{
			alGenBuffers(2, &m_buffers[0]);

			const auto error = alGetError();
			if (error != AL_NO_ERROR)
			{
				GALAXY_LOG(GALAXY_FATAL, error::al_handle_error("Unable to gen audio buffer(s).", error));
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
					GALAXY_LOG(GALAXY_ERROR, error::al_handle_error("Unable to delete audio buffer(s).", error));
				}

				m_buffers = {0, 0};
			}
		}

		bool BufferStream::internal_load(std::string_view file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto file_info = fs.find(file);

			if (file_info.m_code == fs::FileInfo::Code::FOUND)
			{
				if (file_info.m_path.extension() == ".ogg")
				{
					m_stream = stb_vorbis_open_filename(file_info.m_string.c_str(), nullptr, nullptr);

					if (!m_stream)
					{
						GALAXY_LOG(GALAXY_ERROR, "STB failed to load '{0}'.", file);
						return false;
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

						return true;
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Audio file must be ogg vorbis and have extension of .ogg!");
					return false;
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to find '{0}' because '{1}'.", file, magic_enum::enum_name<fs::FileInfo::Code>(file_info.m_code));
				return false;
			}
		}
	} // namespace audio
} // namespace galaxy