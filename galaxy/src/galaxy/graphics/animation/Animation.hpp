///
///
/// Animation.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_ANIMATION_HPP_
#define GALAXY_GRAPHICS_ANIMATION_HPP_

#include <span>
#include <vector>

#include "galaxy/graphics/animation/Frame.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Basic keyframe animation.
		///
		class Animation final : public fs::Serializable
		{
		public:
			///
			/// Constructor.
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
			Animation(std::string_view name, const bool looping, const double speed, std::span<Frame> frames);

			///
			/// JSON constructor.
			///
			/// \param json JSON object.
			///
			Animation(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Animation(Animation&&);

			///
			/// Move assignment operator.
			///
			Animation& operator=(Animation&&);

			///
			/// Copy constructor.
			///
			Animation(const Animation&);

			///
			/// Copy assignment operator.
			///
			Animation& operator=(const Animation&);

			///
			/// Destructor.
			///
			virtual ~Animation();

			///
			/// Advance to the next frame.
			///
			/// \return Also returns a pointer to the next frame, saving you an extra call to get_current_frame().
			///
			[[maybe_unused]] Frame* next_frame();

			///
			/// Restart animation from beginning.
			///
			void restart();

			///
			/// Get currently active frame.
			///
			/// \return Pointer to active frame.
			///
			[[nodiscard]] Frame* current_frame();

			///
			/// Get current frame index.
			///
			/// \return std::size_t.
			///
			[[nodiscard]] std::size_t current_frame_index();

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to be serialized.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		public:
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
			/// The frames that make up the animation.
			/// Each frame contains a duration and a textureID.
			///
			meta::vector<Frame> m_frames;

		private:
			///
			/// The current frame the animation is on.
			///
			Frame* m_active_frame;

			///
			/// Index of the current active frame.
			///
			std::size_t m_current_frame_index;
		};
	} // namespace graphics
} // namespace galaxy

#endif