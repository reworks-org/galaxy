///
/// Animation.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_ANIMATION_HPP_
#define QUASAR_ANIMATION_HPP_

#include <span>
#include <string>
#include <vector>

#include <qs/anim/Frame.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Animation.
	///
	class Animation final
	{
	public:
		///
		/// Default animation.
		///
		Animation();

		///
		/// Argument constructor.
		///
		/// \param name ID of animation.
		/// \param looping Does animation repeat.
		/// \param speed Multiplier. Speed of animation.
		/// \param frames Frames in the animation.
		///
		Animation(std::string_view name, bool looping, const double speed, std::span<qs::Frame> frames);

		///
		/// Copy constructor.
		///
		Animation(const Animation&);

		///
		/// Move constructor.
		///
		Animation(Animation&&);

		///
		/// Copy assignment operator.
		///
		Animation& operator=(const Animation&);

		///
		/// Move assignment operator.
		///
		Animation& operator=(Animation&&);

		///
		/// Destructor.
		///
		~Animation() noexcept;

		///
		/// Restart animation from beginning.
		///
		void restart() noexcept;

		///
		/// Advance to the next frame.
		///
		/// \return Also returns a pointer to the next frame, saving you an extra call to get_current_frame().
		///
		[[maybe_unused]] qs::Frame* next_frame();

		///
		/// Get animation identifier.
		///
		/// \return Const std::string.
		///
		[[nodiscard]] const std::string& get_name() noexcept;

		///
		/// Is animation looping.
		///
		/// \return Const bool.
		///
		[[nodiscard]] const bool is_looping() const noexcept;

		///
		/// Get animation speed.
		///
		/// \return Const float.
		///
		[[nodiscard]] const double get_speed() const noexcept;

		///
		/// Get total frames in animation.
		///
		/// \return Const std::size_t.
		///
		[[nodiscard]] const std::size_t get_total_frames() const noexcept;

		///
		/// Get current active animation frame.
		///
		/// \return Pointer to current active frame.
		///
		[[nodiscard]] const qs::Frame* get_current_frame() const noexcept;

		///
		/// Get current frame.
		///
		/// \return Const std::vector of qs::Frames.
		///
		[[nodiscard]] const auto& get_frames() noexcept;

	private:
		///
		/// The current frame the animation is on.
		///
		qs::Frame* m_active_frame;

		///
		/// Name of the animation.
		///
		std::string m_name;

		///
		/// Does the animation repeat.
		///
		bool m_looping;

		///
		/// Speed of the animation.
		/// Multiplier, so 1.0f is regular speed.
		///
		double m_speed;

		///
		/// Total number of frames in animation.
		///
		std::size_t m_total_frames;

		///
		/// Index of the current active frame.
		///
		std::size_t m_current_frame_index;

		///
		/// The frames that make up the animation.
		/// Each frame contains a duration and a textureID.
		///
		std::vector<qs::Frame> m_frames;
	};
} // namespace qs

#endif