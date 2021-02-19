///
/// TileLayer.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/algorithm/Algorithm.hpp"
#include "galaxy/error/Log.hpp"

#include <nlohmann/json.hpp>

#include "TileLayer.hpp"

namespace galaxy
{
	namespace map
	{
		TileLayer::TileLayer()
		{
			GALAXY_LOG(GALAXY_FATAL, "Cannot instantiate a default constructed TileLayer.");
		}

		TileLayer::TileLayer(const nlohmann::json& json)
		    : Layer {json}, m_compression {""}
		{
			if (json.count("chunks") > 0)
			{
				const auto& chunk_array = json.at("chunks");
				for (auto& chunk : chunk_array)
				{
					m_chunks.emplace_back(chunk);
				}
			}

			// only present on tilelayers.
			if (json.count("compression") > 0)
			{
				m_compression = json.at("compression");
			}

			if (json.count("data") > 0)
			{
				const auto& data_array = json.at("data");
				if (json.is_array())
				{
					std::vector<unsigned int> data_vector;
					for (const auto& data : data_array)
					{
						data_vector.emplace_back(data.get<unsigned int>());
					}

					m_data.emplace<std::vector<unsigned int>>(data_vector);
				}
				else
				{
					m_data = data_array.get<std::string>();
				}

				if (std::holds_alternative<std::string>(m_data))
				{
					if (m_compression == "zlib")
					{
						// base64 -> zlib
						const std::string stage_one = algorithm::decode_base64(std::get<0>(m_data));

						// validate
						if (!stage_one.empty())
						{
							// zlib-> normal
							const std::string stage_two = algorithm::decode_zlib(stage_one);

							// validate
							if (!stage_two.empty())
							{
								// update m_data string
								m_data = stage_two;
							}
							else
							{
								GALAXY_LOG(GALAXY_FATAL, "zlib decoded string empty.");
							}
						}
						else
						{
							GALAXY_LOG(GALAXY_FATAL, "base64 decoded string empty.");
						}
					}
					else if (m_compression == "gzip")
					{
						// base64 -> gzip
						const std::string stage_one = algorithm::decode_base64(std::get<0>(m_data));

						// validate
						if (!stage_one.empty())
						{
							// gzip -> normal
							const std::string stage_two = algorithm::decode_gzip(stage_one);

							// validate
							if (!stage_two.empty())
							{
								// update m_data string
								m_data = stage_two;
							}
							else
							{
								GALAXY_LOG(GALAXY_FATAL, "gzip decoded string empty.");
							}
						}
						else
						{
							GALAXY_LOG(GALAXY_FATAL, "base64 decoded string empty.");
						}
					}
					else
					{
						// base64 -> normal
						const std::string stage_one = algorithm::decode_base64(std::get<0>(m_data));

						// validate
						if (!stage_one.empty())
						{
							// update m_data string
							m_data = stage_one;
						}
						else
						{
							GALAXY_LOG(GALAXY_FATAL, "base64 decoded string empty.");
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

		const std::variant<std::string, std::vector<unsigned int>>& TileLayer::get_data() const noexcept
		{
			return m_data;
		}
	} // namespace map
} // namespace galaxy