///
/// RingBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEDIA_RINGBUFFER_HPP_
#define GALAXY_MEDIA_RINGBUFFER_HPP_

namespace galaxy
{
	namespace media
	{
		///
		///
		///
		class RingBuffer
		{
		public:
			///
			///
			///
			RingBuffer(const int capacity);

			///
			///
			///
			~RingBuffer();

			///
			///
			///
			int write(const float* data, const int count);

			///
			///
			///
			int read(float* data, int count);

			///
			///
			///
			float* direct_read_pointer(int ACount);

			///
			///
			///
			int available_bytes();

			///
			///
			///
			void set_volume(const float volume);

			///
			///
			///
			float get_volume() const;

		private:
			///
			/// Constructor.
			///
			RingBuffer() = delete;

			///
			/// Copy constructor.
			///
			RingBuffer(const RingBuffer&) = delete;

			///
			/// Move constructor.
			///
			RingBuffer(RingBuffer&&) = delete;

			///
			/// Copy assignment operator.
			///
			RingBuffer& operator=(const RingBuffer&) = delete;

			///
			/// Move assignment operator.
			///
			RingBuffer& operator=(RingBuffer&&) = delete;

		private:
			///
			///
			///
			float* m_buffer;

			///
			///
			///
			int m_read_index;

			///
			///
			///
			int m_write_index;

			///
			///
			///
			int m_capacity;

			///
			/// Current volume of audio.
			///
			float m_volume;
		};
	} // namespace media
} // namespace galaxy

#endif