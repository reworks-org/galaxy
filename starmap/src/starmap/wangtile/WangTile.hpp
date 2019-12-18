///
/// WangTile.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_WANGTILE_HPP_
#define STARMAP_WANGTILE_HPP_

#include "nlohmann/json_fwd.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Represents a Wang tile, by referring to a tile in the tileset and associating it with a certain Wang ID.
	///
	class WangTile final
	{
	public:
		///
		/// Default constructor.
		///
		WangTile() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing WangTile json.
		///
		explicit WangTile(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		WangTile(const WangTile&) = default;

		///
		/// Move constructor.
		///
		WangTile(WangTile&&) = default;

		///
		/// Destructor.
		///
		~WangTile();

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing WangTile json.
		///
		void parse(const nlohmann::json& json);

		///
		/// Is flipped diagonally.
		///
		/// \return True if flipped.
		///
		const bool flippedDiagonally() const noexcept;

		///
		/// Is flipped horizontally.
		///
		/// \return True if flipped.
		///
		const bool flippedHorizontally() const noexcept;

		///
		/// Get local tile id.
		///
		/// \return Const int.
		///
		const int getTileID() const noexcept;

		///
		/// Is flipped vertically.
		///
		/// \return True if flipped.
		///
		const bool flippedVertically() const noexcept;

		///
		/// Get wang colour indexes.
		///
		/// \return Std::vector array.
		///
		const auto& getIndexes() const noexcept;

	private:
		///
		/// Tile is flipped diagonally.
		///
		bool m_dFlip;

		///
		/// Tile is flipped horizontally.
		///
		bool m_hFlip;

		///
		/// Local ID of tile.
		///
		int m_tileID;

		///
		/// Tile is flipped vertically.
		///
		bool m_vFlip;

		///
		/// Array of Wang colour indexes.
		///
		std::vector<int> m_wangIndexes;
	};
}

#endif