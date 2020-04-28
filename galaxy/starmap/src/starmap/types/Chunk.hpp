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
		Chunk() noexcept;

		///
		/// Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure containing chunk array from root->layer.
		///
		explicit Chunk(const nlohmann::json& json) noexcept;

		///
		/// Destructor.
		///
		~Chunk() noexcept;

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing chunk array from root->layer.
		///
		void parse(const nlohmann::json& json) noexcept;

		///
		/// Gets data.
		///
		/// \return Const std::variant reference. Either string or array.
		///
		const auto& getData() const noexcept;

		///
		/// Get height.
		///
		/// \return In tiles as int.
		///
		const int getHeight() const noexcept;

		///
		/// Get width.
		///
		/// \return In tiles as int.
		///
		const int getWidth() const noexcept;

		///
		/// Get x coord.
		///
		/// \return In tiles as int.
		///
		const int getX() const noexcept;

		///
		/// Get y coord.
		///
		/// \return In tiles as int.
		///
		const int getY() const noexcept;

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
}

#endif