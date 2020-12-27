///
/// BufferStream.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_AUDIO_BUFFER_BUFFERSTREAM_HPP_
#define GALAXY_AUDIO_BUFFER_BUFFERSTREAM_HPP_

#include <array>
#include <filesystem>
#include <fstream>
#include <string_view>

#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>

namespace galaxy
{
	namespace audio
	{
		///
		/// Uses multiple Buffer objects to stream from disk.
		///
		class BufferStream
		{
		public:
			///
			/// Number of buffers being used by the stream.
			///
			const constexpr static inline ALsizei BUFFER_COUNT = 4;

			///
			/// Size of each buffer used by the stream.
			///
			const constexpr static inline std::size_t BUFFER_SIZE = 65536;

			///
			/// Holds various member variables for BufferStream.
			///
			class Data final
			{
			public:
				friend class BufferStream;
				friend class Music;

				///
				/// Copy constructor.
				///
				Data(const Data&) = delete;

				///
				/// Move constructor.
				///
				Data(Data&&);

				///
				/// Copy assignment operator.
				///
				Data& operator=(const Data&) = delete;

				///
				/// Move assignment operator.
				///
				Data& operator=(Data&&);

				///
				/// Destructor.
				///
				~Data() = default;

				///
				/// OpenAL buffers.
				///
				std::array<ALuint, BufferStream::BUFFER_COUNT> m_buffers;

				///
				/// Path to the file.
				///
				std::filesystem::path m_file_path = {""};

				///
				/// File read from disk handle.
				///
				std::ifstream m_file_handle;

				///
				/// Number of channels in buffer.
				///
				std::uint8_t m_channels = {0};

				///
				/// Frequency (samples) of buffer.
				///
				std::int32_t m_frequency = {0};

				///
				/// Bits per sample.
				///
				std::uint8_t m_bits = {0};

				///
				/// Size of buffer.
				///
				ALsizei m_size = {0};

				///
				/// Used to keep track of how much buffer has been read.
				///
				ALsizei m_consumed = {0};

				///
				/// Audio format.
				///
				ALenum m_format = {0};

				///
				/// OggVorbis file handle.
				///
				OggVorbis_File m_ogg_handle;

				///
				/// Position in ogg file.
				///
				std::int32_t m_ogg_pos = {0};

				///
				/// Total duration of audio.
				///
				std::size_t m_duration = {0};

			private:
				///
				/// Constructor.
				///
				Data() = default;
			};

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

			///
			/// Get stream data.
			///
			/// \return Data (i.e. all protected members).
			///
			[[nodiscard]] BufferStream::Data* get_data();

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
			/// All member vars of Buffer Stream.
			///
			Data m_data;
		};
	} // namespace audio
} // namespace galaxy

#endif