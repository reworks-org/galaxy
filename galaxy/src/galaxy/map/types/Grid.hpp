///
/// Grid.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TYPES_GRID_HPP_
#define GALAXY_MAP_TYPES_GRID_HPP_

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace map
	{
		///
		/// This element is only used in case of isometric orientation, and determines how tile overlays for terrain and collision information are rendered.
		///
		class Grid final
		{
		public:
			///
			/// Constructor.
			///
			Grid() noexcept;

			///
			/// \brief Parse constructor.
			///
			/// Can throw exceptions.
			///
			/// \param json JSON structure/array containing Grid.
			///
			explicit Grid(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Grid() noexcept = default;

			///
			/// \brief Parses json structure to member values; etc.
			///
			/// Can throws exceptions.
			///
			/// \param json JSON structure containing chunk array from root->layer.
			///
			void parse(const nlohmann::json& json);

			///
			/// Get cell height of tile grid.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get the orientation of the grid.
			///
			/// \return String, format: orthogonal (default) or isometric.
			///
			[[nodiscard]] const std::string& get_orientation() const noexcept;

			///
			/// Get cell width of tile grid.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_width() const noexcept;

		private:
			///
			/// Cell height of tile grid.
			///
			int m_height;

			///
			/// Orientation of the grid for the tiles in this tileset.
			///
			std::string m_orientation;

			///
			/// Cell width of tile grid.
			///
			int m_width;
		};
	} // namespace map
} // namespace galaxy

#endif