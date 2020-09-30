///
/// Music.cpp
/// frb
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>

#include "Music.hpp"

///
/// Core namespace.
///
namespace frb
{
	Music::Music()
	    : m_run_loop {false}
	{
		// Gotta love clang-format.
		m_thread = std::jthread {[](Music* music) {
						 if (music->m_run_loop)
						 {
							 music->m_mutex.lock();
							 music->update();
							 music->m_mutex.unlock();
						 }
					 },
					 this};

		set_source_to_manipulate(m_source.handle());
	}

	Music::Music(std::string_view file)
	{
		if (!load(file))
		{
			PL_LOG(PL_FATAL, "Failed to load file: {0}.", file);
		}
	}

	Music::Music(Music&& m)
	{
		m.m_run_loop = false;
		m.m_mutex.lock();

		m.stop();
		m.m_thread.request_stop();
		m.m_thread.join();

		m.m_mutex.unlock();

		this->m_source = std::move(m.m_source);
		this->m_thread = std::jthread {[](Music* music) {
						       if (music->m_run_loop)
						       {
							       music->m_mutex.lock();
							       music->update();
							       music->m_mutex.unlock();
						       }
					       },
					       this};

		this->m_run_loop = true;
	}

	Music& Music::operator=(Music&& m)
	{
		if (this != &m)
		{
			m.m_run_loop = false;
			m.m_mutex.lock();

			m.stop();
			m.m_thread.request_stop();
			m.m_thread.join();

			m.m_mutex.unlock();

			this->m_source = std::move(m.m_source);
			this->m_thread = std::jthread {[](Music* music) {
							       if (music->m_run_loop)
							       {
								       music->m_mutex.lock();
								       music->update();
								       music->m_mutex.unlock();
							       }
						       },
						       this};

			this->m_run_loop = true;
		}

		return *this;
	}

	Music::~Music()
	{
		m_run_loop = false;
		std::lock_guard<std::mutex> lock {m_mutex};

		stop();
		m_thread.request_stop();
		m_thread.join();
	}

	bool Music::load(std::string_view file)
	{
		std::lock_guard<std::mutex> lock {m_mutex};

		auto res = internal_load(file);
		if (res)
		{
			m_source.queue(dynamic_cast<BufferStream*>(this));
			m_run_loop = true;
		}

		return res;
	}

	void Music::update() noexcept
	{
		ALint processed = 0;
		alGetSourcei(m_source.handle(), AL_BUFFERS_PROCESSED, &processed);

		if (!(processed <= 0))
		{
			while ((processed--))
			{
				ALuint buffer_handle;
				alSourceUnqueueBuffers(m_source.handle(), 1, &buffer_handle);

				char* buff = new char[BufferStream::BUFFER_SIZE];
				std::memset(buff, 0, BufferStream::BUFFER_SIZE);

				ALsizei size      = 0;
				std::int32_t read = 0;

				while (read < BufferStream::BUFFER_SIZE)
				{
					std::int32_t result = ov_read(&m_data.m_ogg_handle, &buff[read], BufferStream::BUFFER_SIZE - read, 0, 2, 1, &m_data.m_ogg_pos);
					if (result == OV_HOLE)
					{
						PL_LOG(PL_ERROR, "OV_HOLE found when trying to loop: {0}.", m_source.handle());
						break;
					}
					else if (result == OV_EBADLINK)
					{
						PL_LOG(PL_ERROR, "OV_EBADLINK found when trying to loop: {0}.", m_source.handle());
						break;
					}
					else if (result == OV_EINVAL)
					{
						PL_LOG(PL_ERROR, "OV_EINVAL found when trying to loop: {0}.", m_source.handle());
						break;
					}
					else if (result == 0)
					{
						std::int32_t seek_result = ov_raw_seek(&m_data.m_ogg_handle, 0);

						if (seek_result == OV_ENOSEEK)
						{
							PL_LOG(PL_ERROR, "OV_ENOSEEK found when trying to loop.");
						}
						else if (seek_result == OV_EINVAL)
						{
							PL_LOG(PL_ERROR, "OV_EINVAL found when trying to loop.");
						}
						else if (seek_result == OV_EREAD)
						{
							PL_LOG(PL_ERROR, "OV_EREAD found when trying to loop.");
						}
						else if (seek_result == OV_EFAULT)
						{
							PL_LOG(PL_ERROR, "OV_EFAULT found when trying to loop.");
						}
						else if (seek_result == OV_EOF)
						{
							PL_LOG(PL_ERROR, "OV_EOF found when trying to loop.");
						}
						else if (seek_result == OV_EBADLINK)
						{
							PL_LOG(PL_ERROR, "OV_EBADLINK found when trying to loop.");
						}

						if (seek_result != 0)
						{
							PL_LOG(PL_ERROR, "Unknown error in ov_raw_seek.");
							return;
						}
					}

					read += result;
				}

				size = read;

				if (size > 0)
				{
					alBufferData(buffer_handle, m_data.m_format, buff, size, m_data.m_frequency);
					alSourceQueueBuffers(m_source.handle(), 1, &buffer_handle);
				}

				delete[] buff;

				if (size < BufferStream::BUFFER_SIZE)
				{
					PL_LOG(PL_ERROR, "Missing data in source: {0}.", m_source.handle());
				}

				if (m_source.get_state() != AL_PLAYING)
				{
					stop();
					play();
				}
			}
		}
	}
} // namespace frb