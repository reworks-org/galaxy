///
/// TileLayer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_MAP_LAYER_TILELAYER_HPP_
#define GALAXY_MAP_LAYER_TILELAYER_HPP_

#include "galaxy/map/layer/Layer.hpp"
#include "galaxy/map/types/Chunk.hpp"

namespace galaxy
{
	namespace map
	{
		///
		/// Specialized type of layer for tiles.
		///
		class TileLayer final : public Layer
		{
		public:
			///
			/// Parse constructor.
			///
			/// \param json JSON structure containing layer json from root.
			/// \param zlevel Rendering level of this layer.
			///
			explicit TileLayer(const nlohmann::json& json, const int zlevel);

			///
			/// Destructor.
			///
			virtual ~TileLayer() noexcept;

			///
			/// Get compression.
			///
			/// \return Compression as std::string. Can be empty.
			///
			[[nodiscard]] const std::string& get_compression() const noexcept;

			///
			/// Retrieve data encoding.
			///
			/// \return Const reference to a std::string.
			///
			[[nodiscard]] const std::string& get_encoding() const noexcept;

			///
			/// Get chunks array.
			///
			/// \return Const reference to an array of Chunks.
			///
			[[nodiscard]] const std::vector<Chunk>& get_chunks() const noexcept;

			///
			/// Retrieve tile id grid.
			///
			/// \return Const reference to a std::vector of unsigned integers.
			///
			[[nodiscard]] const std::vector<unsigned int>& get_data() const noexcept;

		private:
			///
			/// Deleted constructor.
			///
			TileLayer() = delete;

		private:
			///
			/// zlib, gzip or empty (default). tilelayer only.
			///
			std::string m_compression;

			///
			/// csv (default) or base64. tilelayer only.
			///
			std::string m_encoding;

			///
			/// Array of chunks (optional). tilelayer only.
			///
			std::vector<Chunk> m_chunks;

			///
			/// Array of unsigned int (GIDs) or base64-encoded data.
			///
			std::vector<unsigned int> m_data;
		};
	} // namespace map
} // namespace galaxy

#endif