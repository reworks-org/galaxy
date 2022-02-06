///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LAYER_HPP_
#define GALAXY_CORE_LAYER_HPP_

#include "galaxy/core/SceneStack.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Represents an "Layer" of the game, encapsulating things like events and rendering.
		///
		class Layer : public fs::Serializable
		{
		public:
			///
			/// Virtual destructor.
			///
			virtual ~Layer();

			///
			/// Process Layer events.
			///
			virtual void events() = 0;

			///
			/// Process update (fixed timestep) logic.
			///
			virtual void update() = 0;

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			virtual void pre_render() = 0;

			///
			/// Render Layer.
			///
			virtual void render() = 0;

			///
			/// Set Layer name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(std::string_view name) noexcept;

			///
			/// Get Layer name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get scene stack.
			///
			/// \return Reference to scene stack.
			///
			[[nodiscard]] SceneStack& get_stack() noexcept;

			///
			/// Serializes object.
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] nlohmann::json serialize() override;

			///
			/// Deserializes from object.
			///
			/// \param json Json object to retrieve data from.
			///
			void deserialize(const nlohmann::json& json) override;

		protected:
			///
			/// Constructor.
			///
			Layer() noexcept;

		protected:
			///
			/// Layer name for debug purposes.
			///
			std::string m_name;

			///
			/// Controls current application layer.
			///
			SceneStack m_scene_stack;
		};
	} // namespace core
} // namespace galaxy

#endif