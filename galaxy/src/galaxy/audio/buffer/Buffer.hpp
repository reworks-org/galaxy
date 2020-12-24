///
/// Buffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_BUFFER_BUFFER_HPP_
#define GALAXY_AUDIO_BUFFER_BUFFER_HPP_

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
			/// Copy constructor.
			///
			Buffer(const Buffer&) = delete;

			///
			/// Move constructor.
			///
			Buffer(Buffer&&);

			///
			/// Copy assignment operator.
			///
			Buffer& operator=(const Buffer&) = delete;

			///
			/// Move assignment operator.
			///
			Buffer& operator=(Buffer&&);

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
			[[nodiscard]] const ALint get_frequency() const;

			///
			/// Get channels of buffer.
			///
			/// \return Channels as integer.
			///
			[[nodiscard]] const ALint get_channels() const;

			///
			/// Get the OpenAL internal int id / handle.
			///
			/// \return Const ALuint handle integer.
			///
			[[nodiscard]] const ALuint handle() const;

		protected:
			///
			/// Load a file from disk.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			[[nodiscard]] bool internal_load(std::string_view file);

			///
			/// Handle to Buffer.
			///
			ALuint m_buffer;

			///
			/// Amount of channels in buffer.
			///
			ALint m_channels;

			///
			/// Frequency of audio.
			///
			ALsizei m_frequency;
		};
	} // namespace audio
} // namespace galaxy

#endif