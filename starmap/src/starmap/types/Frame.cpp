///
/// Frame.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "Frame.hpp"

///
/// Core namespace.
///
namespace starmap
{
	Frame::Frame() noexcept
		:m_duration(0), m_tileID(0)
	{
	}

	Frame::Frame(const nlohmann::json& json)
		:m_duration(0), m_tileID(0)
	{
		parse(json);
	}

	void Frame::parse(const nlohmann::json& json)
	{
		m_duration = json.at("duration");
		m_tileID = json.at("tileid");
	}

	const int Frame::getDuration() const noexcept
	{
		return m_duration;
	}

	const int Frame::getTileID() const noexcept
	{
		return m_tileID;
	}
}