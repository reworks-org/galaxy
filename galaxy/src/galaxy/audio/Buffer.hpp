///
/// Buffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GAlAXY_AUDIO_BUFFER_HPP_
#define GAlAXY_AUDIO_BUFFER_HPP_

#include <span>

#include <AL/al.h>
#include <AL/alc.h>

namespace galaxy
{
	namespace audio
	{
		///
		/// Holds a single piece of audio.
		///
		class Buffer
		{
		public:
			///
			/// \brief Default constructor.
			///
			/// Generates Buffer buffer(s).
			///
			Buffer();

			///
			/// Move constructor.
			///
			Buffer(Buffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			Buffer& operator=(Buffer&&) noexcept;

			///
			/// \brief Default destructor.
			///
			/// Destroys Buffer buffer(s).
			///
			virtual ~Buffer();

			///
			/// Get frequency of buffer.
			///
			/// \return Frequency as integer.
			///
			[[nodiscard]] const ALint get_frequency();

			///
			/// Get bits of buffer.
			///
			/// \return Bits as integer.
			///
			[[nodiscard]] const ALint get_bits();

			///
			/// Get channels of buffer.
			///
			/// \return Channels as integer.
			///
			[[nodiscard]] const ALint get_channels();

			///
			/// Get size of buffer.
			///
			/// \return Size as integer.
			///
			[[nodiscard]] const ALint get_size();

			///
			/// Get the OpenAL internal int id / handle.
			///
			/// \return Const ALuint handle integer.
			///
			[[nodiscard]] const ALuint handle() const noexcept;

		protected:
			///
			/// Load a file from the virtual file system.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			[[maybe_unused]] const bool internal_load(std::string_view file);

		private:
			///
			/// Copy constructor.
			///
			Buffer(const Buffer&) = delete;

			///
			/// Copy assignment operator.
			///
			Buffer& operator=(const Buffer&) = delete;

		private:
			///
			/// Handle to Buffer.
			///
			ALuint m_buffer;
		};
	} // namespace audio
} // namespace galaxy

#endif