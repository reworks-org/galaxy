///
/// Frame.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_FRAME_HPP_
#define QUASAR_FRAME_HPP_

#include <protostar/system/Concepts.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// A single frame of an animation.
	///
	class Frame final
	{
	public:
		///
		/// Constructor.
		///
		Frame() noexcept;

		///
		/// Argument constructor.
		///
		/// \param x X position on the anim texture.
		/// \param y Y position on the anim texture.
		/// \param time_per_frame Total time to display this frame for.
		///
		Frame(const float x, const float y, const std::size_t time_per_frame) noexcept;

		///
		/// Copy constructor.
		///
		Frame(const Frame&);

		///
		/// Move constructor.
		///
		Frame(Frame&&);

		///
		/// Copy assignment operator.
		///
		Frame& operator=(const Frame&);

		///
		/// Move assignment operator.
		///
		Frame& operator=(Frame&&);

		///
		/// Default destructor.
		///
		~Frame() noexcept = default;

		///
		/// Get x pos.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_x() const noexcept;

		///
		/// Get t pos.
		///
		/// \return Const float.
		///
		[[nodiscard]] const float get_y() const noexcept;

		///
		/// Get time per frame.
		///
		/// \return Const std::size_t.
		///
		[[nodiscard]] const std::size_t get_time_per_frame() const noexcept;

	private:
		///
		/// X position on the texture.
		///
		float m_x;

		///
		/// Y position on the texture.
		///
		float m_y;

		///
		/// Time to spend on this frame in milliseconds.
		///
		std::size_t m_time_per_frame;
	};
} // namespace qs

#endif