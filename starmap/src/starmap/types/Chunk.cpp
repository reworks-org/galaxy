///
/// Chunk.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>
#include "starmap/detail/Decoder.hpp"

#include "Chunk.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Chunk::Chunk() noexcept
		:m_height(0), m_width(0), m_x(0), m_y(0)
	{
	}

	Chunk::Chunk(const nlohmann::json& json)
		:m_height(0), m_width(0), m_x(0), m_y(0)
	{
		parse(json);
	}

	Chunk::~Chunk() noexcept
	{
	}

	void Chunk::parse(const nlohmann::json& json)
	{
		m_height = json.at("height");
		m_width = json.at("width");
		m_x = json.at("x");
		m_y = json.at("y");

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
			// base64 -> normal
			std::string stageOne = starmap::decoder::base64(data.get<std::string>());

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
		}
	}

	const auto& Chunk::getData() const noexcept
	{
		return m_data;
	}

	const int Chunk::getHeight() const noexcept
	{
		return m_height;
	}

	const int Chunk::getWidth() const noexcept
	{
		return m_width;
	}

	const int Chunk::getX() const noexcept
	{
		return m_x;
	}

	const int Chunk::getY() const noexcept
	{
		return m_y;
	}
}