///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENES_SCENE_HPP_
#define GALAXY_SCENES_SCENE_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/ui/GUI.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// Represents a scene within an instance.
		///
		class Scene : public fs::Serializable
		{
		public:
			///
			/// Destructor.
			///
			virtual ~Scene() noexcept = default;

			///
			/// On push of scene to stack.
			///
			virtual void on_push() = 0;

			///
			/// On pop of scene off stack.
			///
			virtual void on_pop() = 0;

			///
			/// Process events.
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
			/// Render to screen.
			///
			virtual void render() = 0;

		protected:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the scene.
			///
			Scene(std::string_view name) noexcept;

		private:
			///
			/// Deleted default constructor.
			///
			Scene() = delete;

		public:
			///
			/// Scene name.
			///
			std::string m_name;

			///
			/// Entity/System manager.
			///
			core::World m_world;

			///
			/// Event dispatcher.
			///
			events::Dispatcher m_dispatcher;

			///
			/// GUI theme.
			///
			ui::Theme m_gui_theme;

			///
			/// GUI for this scene.
			///
			ui::GUI m_gui;
		};
	} // namespace scene
} // namespace galaxy

#endif