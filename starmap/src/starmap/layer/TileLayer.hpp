///
/// TileLayer.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_TILELAYER_HPP_
#define STARMAP_TILELAYER_HPP_

#include "starmap/layer/Layer.hpp"
#include "starmap/types/Chunk.hpp"

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Specialized type of layer.
	///
	class TileLayer final : public starmap::Layer
	{
	public:
		///
		/// \brief Parse constructor.
		///
		/// Does not call TileLayer::parse() you must call that afterwards.
		/// Parses Layer common json.
		/// Can throw exceptions.
		///
		/// \param json JSON structure containing chunk array from root map.
		///
		explicit TileLayer(const nlohmann::json& json);

		///
		/// Copy constructor.
		///
		TileLayer(const TileLayer&) noexcept = default;

		///
		/// Move constructor.
		///
		TileLayer(TileLayer&&) noexcept = default;

		///
		/// Default destructor.
		///
		~TileLayer() noexcept override;

		///
		/// Get chunks array.
		///
		/// \return const std::vector<starmap::Chunk>
		///
		const auto& getChunks() const noexcept;

		///
		/// Get compression.
		///
		/// \return compression as std::string. Can be empty.
		///
		const std::string& getCompression() const noexcept;

		///
		/// Retrieve variant data.
		///
		/// \return std::variant 0 = string 1 = vector.
		///
		const auto& getData() const noexcept;

	private:
		///
		/// Default constructor.
		///
		TileLayer() = delete;

	private:
		///
		/// Array of chunks (optional). tilelayer only.
		///
		std::vector<starmap::Chunk> m_chunks;

		///
		/// zlib, gzip or empty (default). tilelayer only.
		///
		std::string m_compression;

		///
		/// Array of unsigned int (GIDs) or base64-encoded data.
		///
		std::variant<std::string, std::vector<unsigned int>> m_data;
	};
}

#endif