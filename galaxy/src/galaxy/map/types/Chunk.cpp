///
/// Chunk.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/error/Log.hpp"
#include "galaxy/math/Base64.hpp"
#include "galaxy/math/GZip.hpp"
#include "galaxy/math/ZLib.hpp"

#include "Chunk.hpp"

namespace galaxy
{
	namespace map
	{
		Chunk::Chunk() noexcept
		    : m_height {0}, m_width {0}, m_x {0}, m_y {0}
		{
		}

		Chunk::Chunk(const nlohmann::json& json, std::string_view encoding, std::string_view compression)
		    : m_height {0}, m_width {0}, m_x {0}, m_y {0}
		{
			parse(json, encoding, compression);
		}

		void Chunk::parse(const nlohmann::json& json, std::string_view encoding, std::string_view compression)
		{
			if (json.count("height") > 0)
			{
				m_height = json.at("height");
			}

			if (json.count("width") > 0)
			{
				m_width = json.at("width");
			}

			if (json.count("x") > 0)
			{
				m_x = json.at("x");
			}

			if (json.count("y") > 0)
			{
				m_y = json.at("y");
			}

			if (json.count("data") > 0)
			{
				if (encoding == "csv")
				{
					for (const auto& data : json.at("data"))
					{
						m_data.emplace_back(data.get<unsigned int>());
					}
				}
				else
				{
					std::string data_str = json.at("data");
					data_str             = math::decode_base64(data_str);

					if (compression == "zlib")
					{
						data_str = math::decode_zlib(data_str);
					}
					else if (compression == "gzip")
					{
						data_str = math::decode_gzip(data_str);
					}
					else if (compression == "zstd")
					{
						GALAXY_LOG(GALAXY_FATAL, "Unsupported compression format: zstd.");
					}

					if (data_str.empty())
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to parse decompressed data string.");
					}
					else
					{
						std::size_t expected_size = m_width * m_height * 4;

						std::vector<unsigned char> buffer;
						buffer.reserve(expected_size);
						buffer.insert(buffer.end(), data_str.begin(), data_str.end());

						m_data.reserve(m_width * m_height);
						for (auto i = 0u; i < expected_size - 3u; i += 4u)
						{
							const unsigned int id = buffer[i] | buffer[i + 1] << 8 | buffer[i + 2] << 16 | buffer[i + 3] << 24;
							m_data.push_back(id);
						}
					}
				}
			}
		}

		const std::vector<unsigned int>& Chunk::get_data() const noexcept
		{
			return m_data;
		}

		const int Chunk::get_height() const noexcept
		{
			return m_height;
		}

		const int Chunk::get_width() const noexcept
		{
			return m_width;
		}

		const int Chunk::get_x() const noexcept
		{
			return m_x;
		}

		const int Chunk::get_y() const noexcept
		{
			return m_y;
		}
	} // namespace map
} // namespace galaxy