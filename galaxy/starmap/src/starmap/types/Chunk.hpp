///
/// Chunk.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_CHUNK_HPP_
#define STARMAP_CHUNK_HPP_

#include <variant>

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// Chunks are used to store the tile layer data for infinite maps.
	///
	class Chunk final
	{
	public:
		///
		/// Default constructor.
		///
		Chunk();

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
		~Chunk() = default;

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
		[[nodiscard]] const auto& get_data() const;

		///
		/// Get height.
		///
		/// \return In tiles as int.
		///
		[[nodiscard]] const int get_height() const;

		///
		/// Get width.
		///
		/// \return In tiles as int.
		///
		[[nodiscard]] const int get_width() const;

		///
		/// Get x coord.
		///
		/// \return In tiles as int.
		///
		[[nodiscard]] const int get_x() const;

		///
		/// Get y coord.
		///
		/// \return In tiles as int.
		///
		[[nodiscard]] const int get_y() const;

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
} // namespace starmap

#endif