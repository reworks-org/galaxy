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
	class Animation
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
		Animation(std::string_view name, bool looping, pr::positive_float auto speed, std::span<qs::Frame> frames);

		///
		/// Destructor.
		///
		~Animation() noexcept;

	private:
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
		float m_speed;

		///
		/// Total number of frames in animation.
		///
		std::size_t m_total_frames;

		///
		/// The current frame the animation is on.
		///
		std::size_t m_current_frame;

		///
		/// The frames that make up the animation.
		/// Each frame contains a duration and a textureID.
		///
		std::vector<qs::Frame> m_frames;
	};
} // namespace qs

#endif