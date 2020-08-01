///
/// TileLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

#include "starmap/detail/Decoder.hpp"

#include "TileLayer.hpp"

///
/// Core namespace.
///
namespace starmap
{
	TileLayer::TileLayer()
	{
		PL_LOG(PL_FATAL, "Cannot instantiate a default constructed TileLayer.");
	}

	TileLayer::TileLayer(const nlohmann::json& json)
	    : Layer {json}, m_compression {""}
	{
		if (json.count("chunks") > 0)
		{
			auto chunk_array = json.at("chunks");
			for (const auto& chunk : chunk_array)
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
			auto data_array = json.at("data");
			if (json.is_array())
			{
				std::vector<unsigned int> data_vector;
				for (const auto& data : data_array)
				{
					data_vector.push_back(data.get<unsigned int>());
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
					std::string stage_one = starmap::decoder::base64(std::get<0>(m_data));

					// validate
					if (!stage_one.empty())
					{
						// zlib-> normal
						std::string stage_two = starmap::decoder::zlib(stage_one);

						// validate
						if (!stage_two.empty())
						{
							// update m_data string
							m_data = stage_two;
						}
						else
						{
							PL_LOG(PL_FATAL, "zlib decoded string empty.");
						}
					}
					else
					{
						PL_LOG(PL_FATAL, "base64 decoded string empty.");
					}
				}
				else if (m_compression == "gzip")
				{
					// base64 -> gzip
					std::string stage_one = starmap::decoder::base64(std::get<0>(m_data));

					// validate
					if (!stage_one.empty())
					{
						// gzip -> normal
						std::string stage_two = starmap::decoder::gzip(stage_one);

						// validate
						if (!stage_two.empty())
						{
							// update m_data string
							m_data = stage_two;
						}
						else
						{
							PL_LOG(PL_FATAL, "gzip decoded string empty.");
						}
					}
					else
					{
						PL_LOG(PL_FATAL, "base64 decoded string empty.");
					}
				}
				else
				{
					// base64 -> normal
					std::string stage_one = starmap::decoder::base64(std::get<0>(m_data));

					// validate
					if (!stage_one.empty())
					{
						// update m_data string
						m_data = stage_one;
					}
					else
					{
						PL_LOG(PL_FATAL, "base64 decoded string empty.");
					}
				}
			}
		}
	}

	TileLayer::~TileLayer() noexcept
	{
		m_chunks.clear();
	}

	const auto& TileLayer::get_chunks() const noexcept
	{
		return m_chunks;
	}

	std::string TileLayer::get_compression() const noexcept
	{
		return m_compression;
	}

	const auto& TileLayer::get_data() const noexcept
	{
		return m_data;
	}
} // namespace starmap