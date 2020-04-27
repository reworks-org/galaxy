///
/// WangTile.cpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "WangTile.hpp"

///
/// Core namespace.
///
namespace starmap
{
	WangTile::WangTile() noexcept
		:m_dFlip(false), m_hFlip(false), m_tileID(0), m_vFlip(false)
	{
	}

	WangTile::WangTile(const nlohmann::json& json)
		:m_dFlip(false), m_hFlip(false), m_tileID(0), m_vFlip(false)
	{
		parse(json);
	}

	WangTile::~WangTile()
	{
		m_wangIndexes.clear();
	}

	void WangTile::parse(const nlohmann::json& json)
	{
		if (json.count("dflip") > 0)
		{
			m_dFlip = json.at("dflip");
		}

		if (json.count("hflip") > 0)
		{
			m_hFlip = json.at("hflip");
		}
		
		if (json.count("tileid") > 0)
		{
			m_tileID = json.at("tileid");
		}

		if (json.count("vflip") > 0)
		{
			m_vFlip = json.at("vflip");
		}
		
		if (json.count("wangid") > 0)
		{
			auto ids = json.at("wangid");
			std::for_each(ids.begin(), ids.end(), [&](const int wangid)
			{
				m_wangIndexes.emplace_back(wangid);
			});
		}
	}

	const bool WangTile::flippedDiagonally() const noexcept
	{
		return m_dFlip;
	}

	const bool WangTile::flippedHorizontally() const noexcept
	{
		return m_hFlip;
	}

	const int WangTile::getTileID() const noexcept
	{
		return m_tileID;
	}

	const bool WangTile::flippedVertically() const noexcept
	{
		return m_vFlip;
	}

	const auto& WangTile::getIndexes() const noexcept
	{
		return m_wangIndexes;
	}
}