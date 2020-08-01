///
/// Frame.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_FRAME_HPP_
#define STARMAP_FRAME_HPP_

#include <nlohmann/json_fwd.hpp>

///
/// Core namespace.
///
namespace starmap
{
	///
	/// A frame of a tile animation.
	///
	class Frame final
	{
	public:
		///
		/// Default constructor.
		///
		Frame() noexcept;

		///
		/// \brief Parse constructor.
		///
		/// Can throw exceptions.
		///
		/// \param json JSON structure/array containing frame json.
		///
		explicit Frame(const nlohmann::json& json);

		///
		/// Destructor.
		///
		~Frame() noexcept = default;

		///
		/// \brief Parses json structure to member values; etc.
		///
		/// Can throws exceptions.
		///
		/// \param json JSON structure containing frame json.
		///
		void parse(const nlohmann::json& json);

		///
		/// Get frame duration.
		///
		/// \return Const int. In milliseconds.
		///
		[[nodiscard]] const int get_duration() const noexcept;

		///
		/// Get tile id of this frame.
		///
		/// \return Const int. Local id scope.
		///
		[[nodiscard]] const int get_tile_id() const noexcept;

	private:
		///
		/// Frame duration in milliseconds.
		///
		int m_duration;

		///
		/// Local tile ID representing this frame.
		///
		int m_tile_id;
	};
} // namespace starmap

#endif