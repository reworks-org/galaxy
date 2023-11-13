///
/// ZLib.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MATH_ZLIB_HPP_
#define GALAXY_MATH_ZLIB_HPP_

#include <zlib.h>

#include "galaxy/utils/Globals.hpp"

namespace galaxy
{
	namespace math
	{
		///
		/// \brief Compressor and Decompressor.
		///
		/// Do not create on stack due to class size.
		///
		class ZLib final
		{
		  public:
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
			/// Constructor.
			///
			/// \param mode ZLib mode to start in.
			///
			ZLib(const Mode mode);

			///
			/// Destructor.
			///
			~ZLib();

			///
			/// Compresses string.
			///
			/// \param input Data to compress.
			///
			/// \return Compressed string.
			///
			std::string compress(const std::string& input);

			///
			/// Completes the compression.
			///
			/// \return Compressed string containing termination block.
			///
			std::string finish();

			///
			/// Decompresses a zlib string.
			///
			/// \param input Data to decompress.
			///
			/// \return String containing decompressed data.
			///
			std::string decompress(const std::string& input);

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
			char m_in[GALAXY_ZLIB_COMPLETE_CHUNK];

			///
			/// Output buffer.
			///
			char m_out[GALAXY_ZLIB_COMPLETE_CHUNK];
		};

		///
		/// Compresses string into ZLib.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string encode_zlib(const std::string& input);

		///
		/// Decompresses string into ZLib.
		///
		/// \param input Input data to convert.
		///
		/// \return Returns output data if successful, std::nullopt otherwise.
		///
		[[nodiscard]] std::string decode_zlib(const std::string& input);
	} // namespace math
} // namespace galaxy

#endif
