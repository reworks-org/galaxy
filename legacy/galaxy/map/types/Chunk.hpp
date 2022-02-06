///
/// Chunk.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_TYPES_CHUNK_HPP_
#define GALAXY_MAP_TYPES_CHUNK_HPP_

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
			/// \param json JSON structure containing chunk array from root->layer.
			/// \param encoding Encoding of string. csv or base64.
			/// \param compression zlib, gzip or empty.
			///
			explicit Chunk(const nlohmann::json& json, std::string_view encoding, std::string_view compression);

			///
			/// Destructor.
			///
			~Chunk() noexcept = default;

			///
			/// Parses json structure to member values; etc.
			///
			/// \param json JSON structure containing chunk array from root->layer.
			/// \param encoding Encoding of string. csv or base64.
			/// \param compression zlib, gzip or empty.
			///
			void parse(const nlohmann::json& json, std::string_view encoding, std::string_view compression);

			///
			/// Gets tile GIDs.
			///
			/// \return Const std::vector reference.
			///
			[[nodiscard]] const std::vector<unsigned int>& get_data() const noexcept;

			///
			/// Get height.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// Get width.
			///
			/// \return Const integer.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// Get x coord.
			///
			/// \return X coordinate in tiles as const integer.
			///
			[[nodiscard]] const int get_x() const noexcept;

			///
			/// Get y coord.
			///
			/// \return Y coordinate in tiles as const integer.
			///
			[[nodiscard]] const int get_y() const noexcept;

		private:
			///
			/// Array of unsigned int (GIDs).
			///
			std::vector<unsigned int> m_data;

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