///
/// TileLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/algorithm/Algorithm.hpp"
#include "galaxy/error/Log.hpp"

#include "TileLayer.hpp"

namespace galaxy
{
	namespace map
	{
		TileLayer::TileLayer(const nlohmann::json& json, const int zlevel)
		    : Layer {json, zlevel}, m_compression {""}, m_encoding {"csv"}
		{
			if (json.count("compression") > 0)
			{
				m_compression = json.at("compression");
			}

			if (json.count("encoding") > 0)
			{
				m_encoding = json.at("encoding");
			}

			if (json.count("chunks") > 0)
			{
				const auto& chunk_array = json.at("chunks");
				for (const auto& chunk : chunk_array)
				{
					m_chunks.emplace_back(chunk, m_compression, m_encoding);
				}
			}

			if (json.count("data") > 0)
			{
				const auto& data_array = json.at("data");
				if (m_encoding == "csv")
				{
					for (const auto& data : json.at("data"))
					{
						m_data.emplace_back(data.get<int>());
					}
				}
				else
				{
					std::string data_str = json.at("data");
					data_str             = algorithm::decode_base64(data_str);

					if (m_compression == "zlib")
					{
						data_str = algorithm::decode_zlib(data_str);
					}
					else if (m_compression == "gzip")
					{
						data_str = algorithm::decode_gzip(data_str);
					}
					else if (m_compression == "zstd")
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
							const int id = buffer[i] | buffer[i + 1] << 8 | buffer[i + 2] << 16 | buffer[i + 3] << 24;
							m_data.push_back(id);
						}
					}
				}
			}
		}

		TileLayer::~TileLayer() noexcept
		{
			m_chunks.clear();
		}

		const std::vector<Chunk>& TileLayer::get_chunks() const noexcept
		{
			return m_chunks;
		}

		const std::string& TileLayer::get_compression() const noexcept
		{
			return m_compression;
		}

		const std::string& TileLayer::get_encoding() const noexcept
		{
			return m_encoding;
		}

		const std::vector<int>& TileLayer::get_data() const noexcept
		{
			return m_data;
		}

	} // namespace map
} // namespace galaxy