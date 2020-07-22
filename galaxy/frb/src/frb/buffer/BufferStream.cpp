///
/// BufferStream.cpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#include <AL/alext.h>
#include <pulsar/Log.hpp>

#include "frb/detail/Error.hpp"

#include "BufferStream.hpp"

///
/// Read ogg file callback.
///
/// \param dest Destination.
/// \param size_a Size A.
/// \param size_b Size B.
/// \param handle Handle to ogg file.
///
/// \return Length read.
///
std::size_t read_ogg_callback(void* dest, std::size_t size_a, std::size_t size_b, void* handle)
{
	ALsizei length = size_a * size_b;
	auto* stream   = reinterpret_cast<frb::BufferStream*>(handle);
	auto* data     = stream->get_data();

	if (data->m_consumed + length > data->m_size)
	{
		length = data->m_size - data->m_consumed;
	}

	if (!data->m_file_handle.is_open())
	{
		data->m_file_handle.open(data->m_file_path.string(), std::ios::binary);
		if (!data->m_file_handle.is_open())
		{
			PL_LOG(PL_ERROR, "Could not re-open streaming file: " + data->m_file_path.string());

			return 0;
		}
	}

	char* buff = new char[length];

	data->m_file_handle.clear();
	data->m_file_handle.seekg(data->m_consumed);
	if (!data->m_file_handle.read(&buff[0], length))
	{
		if (data->m_file_handle.eof())
		{
			data->m_file_handle.clear();
		}
		else if (data->m_file_handle.fail())
		{
			PL_LOG(PL_ERROR, "OGG stream has fail bit set: " + data->m_file_path.string());
			data->m_file_handle.clear();

			return 0;
		}
		else if (data->m_file_handle.bad())
		{
			PL_LOG(PL_ERROR, "OGG stream has bad bit set: " + data->m_file_path.string());
			data->m_file_handle.clear();

			return 0;
		}
	}

	data->m_consumed += length;

	std::memcpy(dest, &buff[0], length);
	delete[] buff;

	data->m_file_handle.clear();

	return length;
}

///
/// Seek ogg pos callback.
///
/// \param handle Handle to ogg file.
/// \param to Pos to seek to.
/// \param type Type.
///
/// \return Pos seek'd to.
///
std::int32_t seek_ogg_callback(void* handle, ogg_int64_t to, std::int32_t type)
{
	auto* stream = reinterpret_cast<frb::BufferStream*>(handle);
	auto* data   = stream->get_data();

	if (type == SEEK_CUR)
	{
		data->m_consumed += to;
	}
	else if (type == SEEK_END)
	{
		data->m_consumed = data->m_size - to;
	}
	else if (type == SEEK_SET)
	{
		data->m_consumed = to;
	}
	else
	{
		return -1;
	}

	if (data->m_consumed < 0)
	{
		data->m_consumed = 0;
		return -1;
	}

	if (data->m_consumed > data->m_size)
	{
		data->m_consumed = data->m_size;
		return -1;
	}

	return 0;
}

///
/// Tell for ogg files callback.
///
/// \param handle Handle to ogg file.
///
/// \return Tell pos.
///
long int tell_ogg_callback(void* handle)
{
	auto* stream = reinterpret_cast<frb::BufferStream*>(handle);
	auto* data   = stream->get_data();

	return data->m_consumed;
}

///
/// Core namespace.
///
namespace frb
{
	BufferStream::BufferStream()
	    : m_data {}
	{
		alGenBuffers(buffer_count, &m_data.m_buffers[0]);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_FATAL, frb::parse_error("Unable to gen audio buffer(s)."));
		}
	}

	BufferStream::~BufferStream()
	{
		if (!(
			m_data.m_buffers[0] == 0 &&
			m_data.m_buffers[1] == 0 &&
			m_data.m_buffers[2] == 0 &&
			m_data.m_buffers[3] == 0))
		{
			destroy_stream();
		}
	}

	BufferStream::Data* BufferStream::get_data() noexcept
	{
		return &m_data;
	}

	bool BufferStream::internal_load(std::string_view file)
	{
		bool result = true;

		// Process filepath properly.
		m_data.m_file_path = std::filesystem::path {file};
		if (m_data.m_file_path.extension() != ".ogg")
		{
			PL_LOG(PL_ERROR, "File must be ogg vorbis and have extension of .ogg");
			result = false;
		}
		else
		{
			m_data.m_file_handle.open(m_data.m_file_path.string(), std::ios::binary);
			if (!m_data.m_file_handle.is_open())
			{
				PL_LOG(PL_ERROR, "Failed to open file: " + m_data.m_file_path.string());
				result = false;
			}
			else
			{
				m_data.m_file_handle.seekg(0, std::ios_base::beg);
				m_data.m_file_handle.seekg(0, std::ios_base::beg);
				m_data.m_file_handle.ignore(std::numeric_limits<std::streamsize>::max());
				m_data.m_size = m_data.m_file_handle.gcount();
				m_data.m_file_handle.clear();
				m_data.m_file_handle.seekg(0, std::ios_base::beg);
				m_data.m_consumed = 0;

				ov_callbacks ogg_callbacks;
				ogg_callbacks.read_func  = read_ogg_callback;
				ogg_callbacks.close_func = nullptr;
				ogg_callbacks.seek_func  = seek_ogg_callback;
				ogg_callbacks.tell_func  = tell_ogg_callback;

				if (ov_open_callbacks(reinterpret_cast<void*>(this), &m_data.m_ogg_handle, nullptr, -1, ogg_callbacks) < 0)
				{
					PL_LOG(PL_ERROR, "ov_open_callbacks failed");
					result = false;
				}
				else
				{
					vorbis_info* info = ov_info(&m_data.m_ogg_handle, -1);

					m_data.m_channels  = info->channels;
					m_data.m_bits      = 16;
					m_data.m_frequency = info->rate;
					m_data.m_duration  = ov_time_total(&m_data.m_ogg_handle, -1);

					if (m_data.m_file_handle.eof())
					{
						PL_LOG(PL_ERROR, "Already reached EOF without loading data: " + m_data.m_file_path.string());
						result = false;
					}
					else if (m_data.m_file_handle.fail())
					{
						PL_LOG(PL_ERROR, "fail bit set: " + m_data.m_file_path.string());
						result = false;
					}
					else if (!m_data.m_file_handle)
					{
						PL_LOG(PL_ERROR, "file is false: " + m_data.m_file_path.string());
						result = false;
					}
					else
					{
						char* buff = new char[buffer_size];
						for (std::uint8_t i = 0; i < buffer_count; ++i)
						{
							std::int32_t processed = 0;
							while (processed < buffer_size)
							{
								std::int32_t read_result = ov_read(&m_data.m_ogg_handle, &buff[processed], buffer_size - processed, 0, 2, 1, &m_data.m_ogg_pos);

								if (read_result == OV_HOLE)
								{
									PL_LOG(PL_ERROR, "OV_HOLE found in initial read of buffer: " + std::to_string(i));
									result = false;

									break;
								}
								else if (read_result == OV_EBADLINK)
								{
									PL_LOG(PL_ERROR, "OV_EBADLINK found in initial read of buffer: " + std::to_string(i));
									result = false;

									break;
								}
								else if (read_result == OV_EINVAL)
								{
									PL_LOG(PL_ERROR, "OV_EINVAL found in initial read of buffer: " + std::to_string(i));
									result = false;

									break;
								}
								else if (read_result == 0)
								{
									PL_LOG(PL_ERROR, "EOF found in initial read of buffer: " + std::to_string(i));
									result = false;

									break;
								}

								processed += result;
							}

							if (result)
							{
								if (m_data.m_channels >= 1 && m_data.m_channels < 4)
								{
									m_data.m_format = AL_FORMAT_STEREO16;
								}
								else if (m_data.m_channels == 4 || m_data.m_channels == 5)
								{
									m_data.m_format = AL_FORMAT_QUAD16;
								}
								else if (m_data.m_channels == 6)
								{
									m_data.m_format = AL_FORMAT_51CHN16;
								}
								else if (m_data.m_channels == 7)
								{
									m_data.m_format = AL_FORMAT_61CHN16;
								}
								else if (m_data.m_channels == 8)
								{
									m_data.m_format = AL_FORMAT_71CHN16;
								}
								else
								{
									m_data.m_format = AL_FORMAT_MONO16;
								}

								alBufferData(m_data.m_buffers[i], m_data.m_format, buff, processed, m_data.m_frequency);
							}
							else
							{
								break;
							}
						}

						delete[] buff;
					}
				}
			}
		}

		return result;
	}

	void BufferStream::destroy_stream() noexcept
	{
		m_data.m_file_handle.close();
		ov_clear(&m_data.m_ogg_handle);

		alDeleteBuffers(buffer_count, &m_data.m_buffers[0]);

		m_data.m_buffers[0] = 0;
		m_data.m_buffers[1] = 0;
		m_data.m_buffers[2] = 0;
		m_data.m_buffers[3] = 0;
	}
} // namespace frb