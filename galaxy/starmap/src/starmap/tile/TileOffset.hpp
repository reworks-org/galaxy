///
/// TileOffset.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TILEOFFSET_HPP_
#define STARMAP_TILEOFFSET_HPP_

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// This element is used to specify an offset in pixels, to be applied when drawing a tile from the related tileset. When not present, no offset is applied.
	///
	class TileOffset final
	{
	public:
		///
		/// Default constructor.
		///
		TileOffset() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing tileoffset json.
		///
		explicit TileOffset(const nlohmann::json& json);

		///
		/// Default destructor.
		///
		~TileOffset() noexcept = default;

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing tileoffset json.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get horizontal offset.
		///
		/// \return Const int. In pixels.
		///
		[[nodiscard]] const int get_x() const noexcept;

		///
		/// Get vertical offset.
		///
		/// \return Const int. In pixels, positive is down.
		///
		[[nodiscard]] const int get_y() const noexcept;

	private:
		///
		/// Horizontal offset in pixels.
		///
		int m_x;

		///
		/// Vertical offset in pixels (positive is down).
		///
		int m_y;
	};
} // namespace starmap

#endif