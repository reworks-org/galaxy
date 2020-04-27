///
/// TileLayer.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

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
		throw std::runtime_error("Cannot instantiate a default constructed TileLayer!");
	}

	TileLayer::TileLayer(const nlohmann::json& json)
		:Layer(json), m_compression("")
	{
		if (json.count("chunks") > 0)
		{
			auto chunk = json.at("chunks");
			std::for_each(chunk.begin(), chunk.end(), [&](const nlohmann::json& item)
				{
					// Constructs in place.
					m_chunks.emplace_back(item);
				});
		}

		// only present on tilelayers.
		if (json.count("compression") > 0)
		{
			m_compression = json.at("compression");
		}

		if (json.count("data") > 0)
		{
			auto data = json.at("data");
			if (json.is_array())
			{
				std::vector<unsigned int> dataAsVector;

				std::for_each(data.begin(), data.end(), [&](const nlohmann::json& item)
					{
						dataAsVector.push_back(item.get<unsigned int>());
					});

				m_data.emplace<std::vector<unsigned int>>(dataAsVector);
			}
			else
			{
				m_data = data.get<std::string>();
			}

			if (std::holds_alternative<std::string>(m_data))
			{
				if (m_compression == "zlib")
				{
					// base64 -> zlib
					std::string stageOne = starmap::decoder::base64(std::get<0>(m_data));

					// validate
					if (!stageOne.empty())
					{
						// zlib-> normal
						std::string stageTwo = starmap::decoder::zlib(stageOne);

						// validate
						if (!stageTwo.empty())
						{
							// update m_data string
							m_data = stageTwo;
						}
						else
						{
							throw std::runtime_error("zlib decoded string empty!");
						}
					}
					else
					{
						throw std::runtime_error("base64 decoded string empty!");
					}
				}
				else if (m_compression == "gzip")
				{
					// base64 -> gzip
					std::string stageOne = starmap::decoder::base64(std::get<0>(m_data));

					// validate
					if (!stageOne.empty())
					{
						// gzip -> normal
						std::string stageTwo = starmap::decoder::gzip(stageOne);

						// validate
						if (!stageTwo.empty())
						{
							// update m_data string
							m_data = stageTwo;
						}
						else
						{
							throw std::runtime_error("gzip decoded string empty!");
						}
					}
					else
					{
						throw std::runtime_error("base64 decoded string empty!");
					}
				}
				else
				{
					// base64 -> normal
					std::string stageOne = starmap::decoder::base64(std::get<0>(m_data));

					// validate
					if (!stageOne.empty())
					{
						// update m_data string
						m_data = stageOne;
					}
					else
					{
						throw std::runtime_error("base64 decoded string empty!");
					}
				} // if-else-if
			} // if std::holds...
		}
	}

	TileLayer::~TileLayer() noexcept
	{
		m_chunks.clear();
	}

	const auto& TileLayer::getChunks() const noexcept
	{
		return m_chunks;
	}

	const std::string& TileLayer::getCompression() const noexcept
	{
		return m_compression;
	}

	const auto& TileLayer::getData() const noexcept
	{
		return m_data;
	}
}