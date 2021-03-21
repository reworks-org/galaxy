///
/// Instance.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_INSTANCE_HPP_
#define GALAXY_CORE_INSTANCE_HPP_

#include "galaxy/core/SceneStack.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Represents an "Instance" of the game, encapsulating things like events and rendering.
		///
		class Instance : public fs::Serializable
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~Instance();

			///
			/// Process Instance events.
			///
			virtual void events() = 0;

			///
			/// Process update (fixed timestep) logic.
			///
			/// \param dt Delta-Time from fixed timestep gameloop.
			///
			virtual void update(const double dt) = 0;

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			virtual void pre_render() = 0;

			///
			/// Render instance.
			///
			virtual void render() = 0;

			///
			/// Set instance name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(std::string_view name) noexcept;

			///
			/// Get instance name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get scene stack.
			///
			/// \return Reference to scene stack.
			///
			[[nodiscard]] core::SceneStack& get_stack() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] virtual nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			virtual void deserialize(const nlohmann::json& json) override;

		protected:
			///
			/// Default constructor.
			///
			Instance() noexcept;

		protected:
			///
			/// Instance name for debug purposes.
			///
			std::string m_name;

			///
			/// Controls current application instance.
			///
			core::SceneStack m_scene_stack;
		};
	} // namespace core
} // namespace galaxy

#endif