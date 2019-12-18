///
/// Frame.hpp
/// starmap
///
/// Refer to LICENSE.txt for more details.
///

#ifndef STARMAP_FRAME_HPP_
#define STARMAP_FRAME_HPP_

#include "nlohmann/json_fwd.hpp"

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
		/// Copy constructor.
		///
		Frame(const Frame&) = default;

		///
		/// Move constructor.
		///
		Frame(Frame&&) = default;

		///
		/// Destructor.
		///
		~Frame() = default;

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
		const int getDuration() const noexcept;

		///
		/// Get tile id of this frame.
		///
		/// \return Const int. Local id scope.
		///
		const int getTileID() const noexcept;

	private:
		///
		/// Frame duration in milliseconds.
		///
		int m_duration;

		///
		/// Local tile ID representing this frame.
		///
		int m_tileID;
	};
}

#endif