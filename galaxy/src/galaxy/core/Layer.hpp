///
/// Layer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_LAYER_HPP_
#define GALAXY_CORE_LAYER_HPP_

#include "galaxy/core/Scene.hpp"

namespace galaxy
{
	namespace core
	{
		///
		/// Represents a "layer" within a scene, encapsulating things like events and rendering.
		///
		class Layer
		{
		public:
			///
			/// Default virtual destructor.
			///
			virtual ~Layer() noexcept = default;

			///
			/// On push of layer to stack.
			///
			virtual void on_push() = 0;

			///
			/// On pop of layer off stack.
			///
			virtual void on_pop() = 0;

			///
			/// Process layer events.
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
			/// Render layer.
			///
			virtual void render() = 0;

			///
			/// Set layer name.
			///
			/// \param name String name for debug purposes.
			///
			void set_name(std::string_view name) noexcept;

			///
			/// Get layer name.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

		protected:
			///
			/// Default constructor.
			///
			Layer() noexcept;

		protected:
			///
			/// Layer name for debug purposes.
			///
			std::string m_name;

			///
			/// Active scene.
			///
			std::unique_ptr<Scene> m_active_scene;
		};
	} // namespace core
} // namespace galaxy

#endif