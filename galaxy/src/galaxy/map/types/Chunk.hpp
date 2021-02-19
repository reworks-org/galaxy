///
/// Chunk.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TYPES_CHUNK_HPP_
#define GALAXY_MAP_TYPES_CHUNK_HPP_

#include <variant>

#include <nlohmann/json_fwd.hpp>

namespace galaxy
{
	namespace map
	{
		///
		/// Chunks are used to store the tile layer data for infinite maps.
		///
		class Chunk final
		{
		public:
			///
			/// Constructor.
			///
			Chunk() noexcept;

			///
			/// Parse constructor.
			///
			/// Can throw exceptions.
			///
			/// \param json JSON structure containing chunk array from root->layer.
			///
			explicit Chunk(const nlohmann::json& json);

			///
			/// Destructor.
			///
			~Chunk() noexcept = default;

			///
			/// \brief Parses json structure to member values; etc.
			///
			/// Can throws exceptions.
			///
			/// \param json JSON structure containing chunk array from root->layer.
			///
			void parse(const nlohmann::json& json);

			///
			/// Gets data.
			///
			/// \return Const std::variant reference. Either string or array.
			///
			[[nodiscard]] const std::variant<std::string, std::vector<unsigned int>>& get_data() const noexcept;

			///
			/// Get height.
			///
			/// \return In tiles as int.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get width.
			///
			/// \return In tiles as int.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// Get x coord.
			///
			/// \return In tiles as int.
			///
			[[nodiscard]] const int get_x() const noexcept;

			///
			/// Get y coord.
			///
			/// \return In tiles as int.
			///
			[[nodiscard]] const int get_y() const noexcept;

		private:
			///
			/// Array of unsigned int (GIDs) or base64-encoded data.
			///
			std::variant<std::string, std::vector<unsigned int>> m_data;

			///
			/// Height in tiles.
			///
			int m_height;

			///
			/// Width in tiles.
			///
			int m_width;

			///
			/// X coordinate in tiles.
			///
			int m_x;

			///
			/// Y coordinate in tiles.
			///
			int m_y;
		};
	} // namespace map
} // namespace galaxy

#endif