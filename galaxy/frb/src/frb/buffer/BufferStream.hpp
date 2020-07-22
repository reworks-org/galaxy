///
/// BufferStream.hpp
/// context
///
/// Refer to LICENSE.txt for more details.
///

#ifndef FRB_BUFFERSTREAM_HPP_
#define FRB_BUFFERSTREAM_HPP_

#include <array>
#include <filesystem>
#include <fstream>
#include <string_view>

#include <AL/al.h>
#include <AL/alc.h>
#include <vorbis/vorbisfile.h>

///
/// Core namespace.
///
namespace frb
{
	///
	/// Uses multiple Buffer objects to stream from disk.
	///
	class BufferStream
	{
	public:
		struct Data
		{
			///
			/// Default constructor.
			///
			Data() noexcept = default;

			///
			/// Default destructor.
			///
			~Data() noexcept = default;

			///
			///
			///
			std::array<ALuint, frb::BufferStream::buffer_count> m_buffers;

			///
			///
			///
			std::filesystem::path m_file_path = {""};

			///
			///
			///
			std::ifstream m_file_handle;

			///
			///
			///
			std::uint8_t m_channels = {0};

			///
			///
			///
			std::int32_t m_frequency = {0};

			///
			///
			///
			std::uint8_t m_bits = {0};

			///
			///
			///
			ALsizei m_size = {0};

			///
			///
			///
			ALsizei m_consumed = {0};

			///
			///
			///
			ALenum m_format = {0};

			///
			///
			///
			OggVorbis_File m_ogg_handle;

			///
			///
			///
			std::int_fast32_t m_ogg_pos = {0};

			///
			///
			///
			std::size_t m_duration = {0};
		};

		///
		/// Number of buffers being used by the stream.
		///
		constexpr static inline ALsizei buffer_count = 4;

		///
		/// Size of each buffer used by the stream.
		///
		constexpr static inline std::size_t buffer_size = 65536;

		///
		/// Constructor.
		///
		BufferStream();

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
		Data* get_data() noexcept;

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
		/// Destroy all memory and OpenAL data.
		///
		void destroy_stream() noexcept;

		///
		/// All member vars of Buffer Stream.
		///
		Data m_data;
	};
} // namespace frb

#endif