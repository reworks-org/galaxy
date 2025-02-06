///
/// RingBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MEM_RINGBUFFER_HPP_
#define GALAXY_MEM_RINGBUFFER_HPP_

namespace galaxy
{
	namespace mem
	{
		///
		/// Memory manager in a ring buffer format.
		///
		class RingBuffer
		{
		public:
			///
			/// Constructor.
			///
			/// \param capacity Usually you want 1 second of audio reserved.
			///
			RingBuffer(const int capacity) noexcept;

			///
			/// Destructor.
			///
			~RingBuffer() noexcept;

			///
			/// Write data to ring buffer.
			///
			/// \param data Pointer to beginning of section to write from.
			/// \param count Amount of data to write.
			///
			/// \return Updated write index.
			///
			[[nodiscard]]
			int write(float* data, const int count) noexcept;

			///
			/// Read data from buffer.
			///
			/// \param data Buffer to read into.
			/// \param count Amount of data to write.
			///
			/// \return Updated read index.
			///
			[[nodiscard]]
			int read(float* data, int count) noexcept;

			///
			/// Read the ring buffer.
			///
			/// \param count Amount of data to read from internal buffer.
			///
			/// \return Section of data read.
			///
			[[nodiscard]]
			float* direct_read_pointer(const int count) noexcept;

			///
			/// Available bytes left in the ring buffer.
			///
			/// \return Amount of space left in bytes.
			///
			[[nodiscard]]
			int available_bytes() const noexcept;

			///
			/// Sets audio volume of the buffer.
			///
			/// \param volume Between 0 and 1.
			///
			void set_volume(const float volume) noexcept;

			///
			/// Gets current buffer volume.
			///
			/// \return Float from 0 to 1.
			///
			[[nodiscard]]
			float get_volume() const noexcept;

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
			/// Data buffer.
			///
			float* m_buffer;

			///
			/// Current index for reading from buffer.
			///
			int m_read_index;

			///
			/// Current index for writing to buffer.
			///
			int m_write_index;

			///
			/// Amount of data buffer can hold in total.
			///
			int m_capacity;

			///
			/// Current volume of audio.
			///
			float m_volume;
		};
	} // namespace mem
} // namespace galaxy

#endif
