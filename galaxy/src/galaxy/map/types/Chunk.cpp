///
/// Chunk.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <pulsar/Log.hpp>
#include <nlohmann/json.hpp>

#include "starmap/detail/Decoder.hpp"

#include "Chunk.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Chunk::Chunk()
	    : m_height {0}, m_width {0}, m_x {0}, m_y {0}
	{
	}

	Chunk::Chunk(const nlohmann::json& json)
	    : m_height {0}, m_width {0}, m_x {0}, m_y {0}
	{
		parse(json);
	}

	void Chunk::parse(const nlohmann::json& json)
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
				// base64 -> normal
				std::string stage_one = starmap::decoder::base64(data_array.get<std::string>());

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

	const auto& Chunk::get_data() const
	{
		return m_data;
	}

	const int Chunk::get_height() const
	{
		return m_height;
	}

	const int Chunk::get_width() const
	{
		return m_width;
	}

	const int Chunk::get_x() const
	{
		return m_x;
	}

	const int Chunk::get_y() const
	{
		return m_y;
	}
} // namespace starmap