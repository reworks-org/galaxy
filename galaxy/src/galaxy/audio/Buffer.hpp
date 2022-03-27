///
/// Buffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_BUFFER_HPP_
#define GALAXY_AUDIO_BUFFER_HPP_

#include <span>
#include <string_view>

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
			[[nodiscard]] ALint get_frequency() noexcept;

			///
			/// Get bits of buffer.
			///
			/// \return Bits as integer.
			///
			[[nodiscard]] ALint get_bits() noexcept;

			///
			/// Get channels of buffer.
			///
			/// \return Channels as integer.
			///
			[[nodiscard]] ALint get_channels() noexcept;

			///
			/// Get size of buffer.
			///
			/// \return Size as integer.
			///
			[[nodiscard]] ALint get_size() noexcept;

			///
			/// Get the OpenAL internal int id / handle.
			///
			/// \return Const ALuint handle integer.
			///
			[[nodiscard]] ALuint handle() const noexcept;

		protected:
			///
			/// Load a file from the virtual file system.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			[[maybe_unused]] bool internal_load(std::string_view file);

		protected:
			///
			/// Handle to Buffer.
			///
			ALuint m_buffer;

		private:
			///
			/// Move constructor.
			///
			Buffer(Buffer&&) = delete;

			///
			/// Move assignment operator.
			///
			Buffer& operator=(Buffer&&) = delete;

			///
			/// Copy constructor.
			///
			Buffer(const Buffer&) = delete;

			///
			/// Copy assignment operator.
			///
			Buffer& operator=(const Buffer&) = delete;
		};
	} // namespace audio
} // namespace galaxy

#endif