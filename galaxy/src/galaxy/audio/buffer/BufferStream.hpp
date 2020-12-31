///
/// BufferStream.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_BUFFER_BUFFERSTREAM_HPP_
#define GALAXY_AUDIO_BUFFER_BUFFERSTREAM_HPP_

#include <array>

#include <AL/al.h>
#include <AL/alc.h>
#include <stb/stb_vorbis.h>

namespace galaxy
{
	namespace audio
	{
		///
		/// Uses multiple Buffer objects to stream from disk.
		///
		class BufferStream
		{
			friend class Source;

		public:
			///
			/// Size of each buffer used by the stream.
			///
			static inline const constexpr std::size_t CHUNK = 65536;

			///
			/// Constructor.
			///
			BufferStream();

			///
			/// Copy constructor.
			///
			BufferStream(const BufferStream&) = delete;

			///
			/// Move constructor.
			///
			BufferStream(BufferStream&&);

			///
			/// Copy assignment operator.
			///
			BufferStream& operator=(const BufferStream&) = delete;

			///
			/// Move assignment operator.
			///
			BufferStream& operator=(BufferStream&&);

			///
			/// \brief Destructor.
			///
			/// Destroys BufferStream BufferStream(s).
			///
			virtual ~BufferStream();

		protected:
			///
			/// Load a file to stream from disk.
			///
			/// \param file File to load from disk. Can only load ogg vorbis.
			///
			/// \return False if load failed.
			///
			bool internal_load(std::string_view file);

			///
			/// OpenAL data buffers.
			///
			std::array<ALuint, 2> m_buffers;

			///
			/// STB data buffer.
			///
			short* m_data;

			///
			/// STB data info buffer.
			///
			stb_vorbis_info m_info;

			///
			/// Streaming info.
			///
			stb_vorbis* m_stream;

			///
			/// Audio format.
			///
			ALenum m_format;
		};
	} // namespace audio
} // namespace galaxy

#endif