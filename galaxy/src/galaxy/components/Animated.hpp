///
/// Animated.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_COMPONENTS_ANIMATED_HPP_
#define GALAXY_COMPONENTS_ANIMATED_HPP_

#include <robin_hood.h>

#include "galaxy/graphics/animation/Animation.hpp"

namespace galaxy
{
	namespace systems
	{
		class AnimationSystem;
	} // namespace systems

	namespace components
	{
		///
		/// Allows for an entity to be animated.
		///
		class Animated final : public fs::Serializable
		{
			friend class systems::AnimationSystem;

		public:
			///
			/// Constructor.
			///
			Animated() noexcept;

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Animated(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Animated(Animated&&) noexcept;

			///
			/// Move assignment operator.
			///
			Animated& operator=(Animated&&) noexcept;

			///
			/// Destructor.
			///
			~Animated() noexcept;

			///
			/// Add an animation to the object.
			///
			/// \param name Name of the animation.
			/// \param args Variadic arguments to construct animation with.
			///
			template<typename... Args>
			void add_animation(std::string_view name, Args&&... args);

			///
			/// Change the current animation.
			///
			/// \param animation The name of the animation type to change to as defined in lua file. E.g. "walking" -> "running"
			///
			void set_animation(std::string_view animation);

			///
			/// Play the animation.
			///
			void play() noexcept;

			///
			/// Play a specific animation.
			///
			/// \param animation Animation to change to to play.
			///
			void play(std::string_view animation) noexcept;

			///
			/// Pause animation.
			///
			void pause() noexcept;

			///
			/// Stop the animation. If you call play() the animation starts from the beginning.
			///
			void stop() noexcept;

			///
			/// Get current animation.
			///
			/// \return Pointer to currently playing animation.
			///
			[[nodiscard]] graphics::Animation* const get_cur_animation() const noexcept;

			///
			/// Get all animations.
			///
			/// \return Map of all animations.
			///
			[[nodiscard]] auto get_all_anims() const noexcept -> const robin_hood::unordered_map<std::string, graphics::Animation>&;

			///
			/// Is the animation paused?
			///
			/// \return Const bool. True if paused.
			///
			[[nodiscard]] const bool is_paused() const noexcept;

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

		private:
			///
			/// Current active animation.
			///
			graphics::Animation* m_active_anim;

			///
			/// Controls if the animation is paused or not.
			///
			bool m_paused;

			///
			/// Current amount of time spent on the frame.
			///
			double m_time_spent_on_frame;

			///
			/// Stored animations.
			///
			robin_hood::unordered_map<std::string, graphics::Animation> m_animations;
		};

		template<typename... Args>
		inline void Animated::add_animation(std::string_view name, Args&&... args)
		{
			m_animations.emplace(name, graphics::Animation {std::forward<Args>(args)...});
		}
	} // namespace components
} // namespace galaxy

#endif