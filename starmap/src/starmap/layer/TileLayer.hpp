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
	class TileLayer : public starmap::Layer<TileLayer>
	{
	public:
		///
		/// \brief Parse constructor.
		///
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

		void parse(const nlohmann::json& json);

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