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
		/// \brief Default constructor.
		///
		/// Throws a runtime exception if called.
		///
		TileLayer();

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
		/// Destructor.
		///
		~TileLayer();

		///
		/// Get chunks array.
		///
		/// \return Const std::vector<starmap::Chunk>
		///
		[[nodiscard]] const auto& get_chunks() const;

		///
		/// Get compression.
		///
		/// \return Compression as std::string. Can be empty.
		///
		[[nodiscard]] std::string get_compression() const;

		///
		/// Retrieve variant data.
		///
		/// \return Std::variant 0 = string 1 = vector.
		///
		[[nodiscard]] const auto& get_data() const;

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
} // namespace starmap

#endif