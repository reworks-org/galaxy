///
/// ZLib.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_ZLIB_HPP_
#define GALAXY_MATH_ZLIB_HPP_

#define MINIZ_HEADER_FILE_ONLY
#include <miniz.h>

namespace galaxy
{
	///
	/// Zip (de)compressor.
	///
	class ZLib final
	{
	public:
		///
		/// ZLib parsing chunk size.
		///
		static constexpr const auto ChunkSize = 16384;

		///
		/// ZLib mode to start in.
		///
		enum class Mode : int
		{
			///
			/// ZLib deflation.
			///
			COMPRESS = 0,

			///
			/// ZLib inflation.
			///
			DECOMPRESS = 1
		};

		///
		/// Destructor.
		///
		~ZLib();

		///
		/// Compresses string into ZLib.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]]
		static std::string encode(const std::string& input);

		///
		/// Decompresses string into ZLib.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]]
		static std::string decode(const std::string& input);

	private:
		///
		/// Constructor.
		///
		/// \param mode ZLib mode to start in.
		///
		ZLib(const Mode mode);

		///
		/// Compresses string.
		///
		/// \param input Data to compress.
		///
		/// \return Compressed string.
		///
		[[nodiscard]]
		std::string compressor(const std::string& input);

		///
		/// Completes the compression.
		///
		/// \return Compressed string containing termination block.
		///
		[[nodiscard]]
		std::string finish();

		///
		/// Decompresses a zlib string.
		///
		/// \param input Data to decompress.
		///
		/// \return String containing decompressed data.
		///
		[[nodiscard]]
		std::string decompressor(const std::string& input);

	private:
		///
		/// ZLib mode.
		///
		Mode m_mode;

		///
		/// Stream object.
		///
		z_stream m_stream;

		///
		/// Has the compression finished.
		///
		bool m_finished;

		///
		/// Input buffer.
		///
		char m_in[ZLib::ChunkSize];

		///
		/// Output buffer.
		///
		char m_out[ZLib::ChunkSize];
	};
} // namespace galaxy

#endif
