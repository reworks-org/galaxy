///
/// RingBuffer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <cmath>
#include <utility>

#include "RingBuffer.hpp"

namespace galaxy
{
	namespace media
	{
		RingBuffer::RingBuffer(const int capacity)
			: m_read_index {0}
			, m_write_index {0}
			, m_capacity {capacity}
		{
			m_buffer = new float[m_capacity];
			set_volume(1.0f);
		}

		RingBuffer::~RingBuffer()
		{
			delete[] m_buffer;
			m_buffer = nullptr;
		}

		int RingBuffer::write(const float* data, const int count)
		{
			for (auto i = 0; i < count; i++)
			{
				const auto pos = (m_write_index + i) % m_capacity;
				m_buffer[pos]  = data[i];
			}

			m_write_index = (m_write_index + count) % m_capacity;

			return count;
		}

		int RingBuffer::read(float* data, int count)
		{
			for (auto i = 0; i < count; i++)
			{
				const auto pos = (m_read_index + i) % m_capacity;
				data[i]        = m_buffer[pos];
			}

			m_read_index = (m_read_index + count) % m_capacity;

			return count;
		}

		float* RingBuffer::direct_read_pointer(const int count)
		{
			float* result = &m_buffer[m_read_index % m_capacity];
			m_read_index  = (m_read_index + count) % m_capacity;
			return result;
		}

		int RingBuffer::available_bytes()
		{
			return (m_capacity + m_write_index - m_read_index) % m_capacity;
		}

		void RingBuffer::set_volume(const float volume)
		{
			const float gain   = (std::max(std::min(volume, 1.0f), 0.0f) * 50) - 50;
			const float factor = std::pow(10, gain * 0.05f);

			m_volume = std::max(std::min(1.0f * factor, 1.0f), 0.0f);
		}

		float RingBuffer::get_volume() const
		{
			return m_volume;
		}
	} // namespace media
} // namespace galaxy