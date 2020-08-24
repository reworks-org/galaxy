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
			PL_LOG(PL_ERROR, "Could not re-open streaming file: {0}.", data->m_file_path.string());
			return 0;
		}
	}

	std::string buff(length, '\0'); // Doesnt like {} initialization.

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
			PL_LOG(PL_ERROR, "OGG stream has fail bit set: {0}.", data->m_file_path.string());
			data->m_file_handle.clear();

			return 0;
		}
		else if (data->m_file_handle.bad())
		{
			PL_LOG(PL_ERROR, "OGG stream has bad bit set: {0}.", data->m_file_path.string());
			data->m_file_handle.clear();

			return 0;
		}
	}

	data->m_consumed += length;

	std::memcpy(dest, &buff[0], length);

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
		alGenBuffers(BUFFER_COUNT, &m_data.m_buffers[0]);
		if (alGetError() != AL_NO_ERROR)
		{
			PL_LOG(PL_FATAL, frb::parse_error("Unable to gen audio buffer(s)."));
		}
	}

	BufferStream::BufferStream(BufferStream&& bs)
	{
		this->m_data = std::move(bs.m_data);
	}

	BufferStream& BufferStream::operator=(BufferStream&& bs)
	{
		if (this != &bs)
		{
			this->m_data = std::move(bs.m_data);
		}

		return *this;
	}

	BufferStream::~BufferStream()
	{
		m_data.m_file_handle.clear();
		m_data.m_file_handle.close();

		ov_clear(&m_data.m_ogg_handle);

		alDeleteBuffers(BUFFER_COUNT, &m_data.m_buffers[0]);
		for (auto& buffer : m_data.m_buffers)
		{
			buffer = 0;
		}
	}

	frb::BufferStream::Data* BufferStream::get_data() noexcept
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
			PL_LOG(PL_ERROR, "File must be ogg vorbis and have extension of .ogg: {0}.", m_data.m_file_path.string());
			result = false;
		}
		else
		{
			m_data.m_file_handle.open(m_data.m_file_path.string(), std::ios::binary);
			if (!m_data.m_file_handle.is_open())
			{
				PL_LOG(PL_ERROR, "Failed to open file: {0}.", m_data.m_file_path.string());
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
					PL_LOG(PL_ERROR, "ov_open_callbacks failed for: {0}.", m_data.m_file_path.string());
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
						PL_LOG(PL_ERROR, "Already reached EOF without loading data: {0}.", m_data.m_file_path.string());
						result = false;
					}
					else if (m_data.m_file_handle.fail())
					{
						PL_LOG(PL_ERROR, "fail bit set: {0}.", m_data.m_file_path.string());
						result = false;
					}
					else if (!m_data.m_file_handle)
					{
						PL_LOG(PL_ERROR, "file is false: {0}.", m_data.m_file_path.string());
						result = false;
					}
					else
					{
						std::string buff(BUFFER_SIZE, '\0'); // Doesnt like {} initialization.
						for (std::uint8_t i = 0; i < BUFFER_COUNT; ++i)
						{
							std::int32_t processed = 0;
							while (processed < BUFFER_SIZE)
							{
								std::int32_t read_result = ov_read(&m_data.m_ogg_handle, &buff[processed], BUFFER_SIZE - processed, 0, 2, 1, &m_data.m_ogg_pos);

								if (read_result == OV_HOLE)
								{
									PL_LOG(PL_ERROR, "OV_HOLE found in initial read of buffer: {0}.", m_data.m_file_path.string());
									result = false;

									break;
								}
								else if (read_result == OV_EBADLINK)
								{
									PL_LOG(PL_ERROR, "OV_EBADLINK found in initial read of buffer: {0}.", m_data.m_file_path.string());
									result = false;

									break;
								}
								else if (read_result == OV_EINVAL)
								{
									PL_LOG(PL_ERROR, "OV_EINVAL found in initial read of buffer: {0}.", m_data.m_file_path.string());
									result = false;

									break;
								}
								else if (read_result == 0)
								{
									PL_LOG(PL_ERROR, "EOF found in initial read of buffer: {0}.", m_data.m_file_path.string());
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

								alBufferData(m_data.m_buffers[i], m_data.m_format, buff.data(), processed, m_data.m_frequency);
							}
							else
							{
								break;
							}
						}
					}
				}
			}
		}

		return result;
	}

	BufferStream::Data::Data(Data&& d)
	{
		this->m_bits        = d.m_bits;
		this->m_buffers     = std::move(d.m_buffers);
		this->m_channels    = d.m_channels;
		this->m_consumed    = d.m_consumed;
		this->m_duration    = d.m_duration;
		this->m_file_handle = std::move(d.m_file_handle);
		this->m_file_path   = std::move(d.m_file_path);
		this->m_format      = d.m_format;
		this->m_frequency   = d.m_frequency;
		this->m_ogg_handle  = std::move(d.m_ogg_handle);
		this->m_ogg_pos     = d.m_ogg_pos;
		this->m_size        = d.m_size;

		d.m_bits       = 0;
		d.m_buffers[0] = 0;
		d.m_buffers[1] = 0;
		d.m_buffers[2] = 0;
		d.m_buffers[3] = 0;
		d.m_channels   = 0;
		d.m_consumed   = 0;
		d.m_duration   = 0;
		// d.m_file_handle.close(); Shouldnt be needed.
		d.m_file_path  = "";
		d.m_format     = 0;
		d.m_frequency  = 0;
		d.m_ogg_handle = OggVorbis_File();
		d.m_ogg_pos    = 0;
		d.m_size       = 0;
	}

	BufferStream::Data& BufferStream::Data::operator=(Data&& d)
	{
		if (this != &d)
		{
			this->m_bits        = d.m_bits;
			this->m_buffers     = std::move(d.m_buffers);
			this->m_channels    = d.m_channels;
			this->m_consumed    = d.m_consumed;
			this->m_duration    = d.m_duration;
			this->m_file_handle = std::move(d.m_file_handle);
			this->m_file_path   = std::move(d.m_file_path);
			this->m_format      = d.m_format;
			this->m_frequency   = d.m_frequency;
			this->m_ogg_handle  = std::move(d.m_ogg_handle);
			this->m_ogg_pos     = d.m_ogg_pos;
			this->m_size        = d.m_size;

			d.m_bits       = 0;
			d.m_buffers[0] = 0;
			d.m_buffers[1] = 0;
			d.m_buffers[2] = 0;
			d.m_buffers[3] = 0;
			d.m_channels   = 0;
			d.m_consumed   = 0;
			d.m_duration   = 0;
			// d.m_file_handle.close(); Shouldnt be needed.
			d.m_file_path  = "";
			d.m_format     = 0;
			d.m_frequency  = 0;
			d.m_ogg_handle = OggVorbis_File();
			d.m_ogg_pos    = 0;
			d.m_size       = 0;
		}

		return *this;
	}

} // namespace frb