///
/// ZLib.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/logging/Log.hpp"
#include "galaxy/platform/Pragma.hpp"

#include "ZLib.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26403)
GALAXY_DISABLE_WARNING(26407)
GALAXY_DISABLE_WARNING(26409)
GALAXY_DISABLE_WARNING(26414)
GALAXY_DISABLE_WARNING(26477)
#endif

namespace galaxy
{
	namespace math
	{
		ZLib::ZLib(const Mode mode)
			: m_mode {mode}
			, m_finished {false}
			, m_in {0}
			, m_out {0}
		{
			try
			{
				if (m_mode == ZLib::Mode::COMPRESS)
				{
					m_stream.zalloc = Z_NULL;
					m_stream.zfree  = Z_NULL;
					m_stream.opaque = Z_NULL;

					if (deflateInit2(&m_stream, 9, Z_DEFLATED, 15, 9, Z_DEFAULT_STRATEGY) != Z_OK)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to initialize ZLib compressor.");
					}
				}
				else
				{
					m_stream.zalloc   = Z_NULL;
					m_stream.zfree    = Z_NULL;
					m_stream.opaque   = Z_NULL;
					m_stream.next_in  = Z_NULL;
					m_stream.avail_in = 0;

					if (inflateInit2(&m_stream, 15) != Z_OK)
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to initialize ZLib decompressor.");
					}
				}
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}
		}

		ZLib::~ZLib()
		{
			try
			{
				if (m_mode == ZLib::Mode::COMPRESS)
				{
					deflateEnd(&m_stream);
				}
				else
				{
					inflateEnd(&m_stream);
				}
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}
		}

		std::string ZLib::compressor(const std::string& input)
		{
			std::string result;

			try
			{
				if (m_mode == ZLib::Mode::COMPRESS)
				{
					if (!m_finished)
					{
						for (auto i = 0u; i < input.length(); i += GALAXY_ZLIB_COMPLETE_CHUNK)
						{
							const auto left    = static_cast<unsigned int>(input.length() - i);
							const auto end     = left <= GALAXY_ZLIB_COMPLETE_CHUNK;
							const auto current = (left > GALAXY_ZLIB_COMPLETE_CHUNK) ? GALAXY_ZLIB_COMPLETE_CHUNK : left;

							std::memcpy(m_in, input.data() + i, current);

							m_stream.avail_in = current;
							m_stream.next_in  = (Bytef*)m_in;

							do
							{
								m_stream.avail_out = GALAXY_ZLIB_COMPLETE_CHUNK;
								m_stream.next_out  = (Bytef*)m_out;

								if (deflate(&m_stream, end ? Z_SYNC_FLUSH : Z_NO_FLUSH) == Z_STREAM_ERROR)
								{
									GALAXY_LOG(GALAXY_ERROR, "Failed to compress ZLib data stream.");
									return {};
								}

								result += std::string(m_out, GALAXY_ZLIB_COMPLETE_CHUNK - m_stream.avail_out);
							} while (m_stream.avail_out == 0);
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "Dont call ZLib compress after calling zlib finish.");
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Dont call ZLib compress in decompression mode.");
				}
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}

			return result;
		}

		std::string ZLib::finish()
		{
			std::string result;

			try
			{
				if (m_mode == ZLib::Mode::COMPRESS)
				{
					if (!m_finished)
					{
						m_finished        = true;
						m_stream.avail_in = 0;
						m_stream.next_in  = (Bytef*)m_in;

						do
						{
							m_stream.avail_out = GALAXY_ZLIB_COMPLETE_CHUNK;
							m_stream.next_out  = (Bytef*)m_out;

							if (deflate(&m_stream, Z_FINISH) == Z_STREAM_ERROR)
							{
								GALAXY_LOG(GALAXY_ERROR, "Failed to complete ZLib compression.");
								return {};
							}

							result += std::string(m_out, GALAXY_ZLIB_COMPLETE_CHUNK - m_stream.avail_out);
						} while (m_stream.avail_out == 0);
					}
					else
					{
						GALAXY_LOG(GALAXY_WARNING, "ZLib finish already called.");
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Dont call ZLib finish in decompress mode.");
				}
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}

			return result;
		}

		std::string ZLib::decompressor(const std::string& input)
		{
			std::string result;

			try
			{
				if (m_mode == ZLib::Mode::DECOMPRESS)
				{
					for (auto i = 0u; i < input.length(); i += GALAXY_ZLIB_COMPLETE_CHUNK)
					{
						const auto left    = static_cast<unsigned int>(input.length() - i);
						const auto current = (left > GALAXY_ZLIB_COMPLETE_CHUNK) ? GALAXY_ZLIB_COMPLETE_CHUNK : left;

						std::memcpy(m_in, input.data() + i, current);

						m_stream.avail_in = current;
						m_stream.next_in  = (Bytef*)m_in;

						if (m_stream.avail_in == 0)
						{
							break;
						}
						do
						{
							m_stream.avail_out = GALAXY_ZLIB_COMPLETE_CHUNK;
							m_stream.next_out  = (Bytef*)m_out;

							if (inflate(&m_stream, Z_NO_FLUSH) == Z_STREAM_ERROR)
							{
								GALAXY_LOG(GALAXY_ERROR, "Failed to complete ZLib decompression.");
								return {};
							}

							result += std::string(m_out, GALAXY_ZLIB_COMPLETE_CHUNK - m_stream.avail_out);
						} while (m_stream.avail_out == 0);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Dont call ZLib finish in compress mode.");
				}
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}

			return result;
		}

		std::string encode_zlib(const std::string& input)
		{
			auto in = new char[GALAXY_ZLIB_COMPLETE_CHUNK];

			std::string result;

			try
			{
				auto zlib = std::make_unique<ZLib>(ZLib::Mode::COMPRESS);
				std::memset(in, 0, sizeof(char) * GALAXY_ZLIB_COMPLETE_CHUNK);

				unsigned int total_read = 0;

				std::stringstream sstream;
				sstream.str(input);

				while (true)
				{
					sstream.read(in, GALAXY_ZLIB_COMPLETE_CHUNK);
					total_read = static_cast<unsigned int>(sstream.gcount());

					if (total_read != 0)
					{
						result += zlib->compressor({in, total_read});
					}
					else
					{
						break;
					}
				}

				result += zlib->finish();
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}

			delete[] in;
			return result;
		}

		std::string decode_zlib(const std::string& input)
		{
			auto in = new char[GALAXY_ZLIB_COMPLETE_CHUNK];

			std::string result;

			try
			{
				auto zlib = std::make_unique<ZLib>(ZLib::Mode::DECOMPRESS);
				std::memset(in, 0, sizeof(char) * GALAXY_ZLIB_COMPLETE_CHUNK);

				unsigned int total_read = 0;

				std::stringstream sstream;
				sstream.str(input);

				while (true)
				{
					sstream.read(in, GALAXY_ZLIB_COMPLETE_CHUNK);
					total_read = static_cast<unsigned int>(sstream.gcount());

					if (total_read != 0)
					{
						result += zlib->decompressor({in, total_read});
					}
					else
					{
						break;
					}
				}
			}
			catch (const std::exception& e)
			{
				GALAXY_LOG(GALAXY_ERROR, "{0}.", e.what());
			}

			delete[] in;
			return result;
		}
	} // namespace math
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif
