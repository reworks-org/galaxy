///
/// GUIScene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENES_GUISCENE_HPP_
#define GALAXY_SCENES_GUISCENE_HPP_

#include "galaxy/scenes/Scene.hpp"
#include "galaxy/ui/GUI.hpp"

namespace galaxy
{
	namespace scenes
	{
		///
		/// A game scene that contains a GUI.
		///
		class GUIScene final : public scenes::Scene
		{
		public:
			///
			/// Constructor.
			///
			/// \param name Scene name.
			///
			GUIScene(std::string_view name) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~GUIScene() noexcept;

			///
			/// On push of scene to stack.
			///
			void on_push() override;

			///
			/// On pop of scene off stack.
			///
			void on_pop() override;

			///
			/// Process world events.
			///
			void events() override;

			///
			/// Process world systems.
			///
			/// \param dt Delta-Time from fixed timestep gameloop.
			///
			void update(const double dt) override;

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			void pre_render() override;

			///
			/// Render everything.
			///
			void render() override;

			///
			/// \brief Serialize function to be called by child class.
			///
			/// Called after world and camera serialization by Scene::serialize().
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] nlohmann::json sub_serialize() override;

			///
			/// \brief Deserialize function to be called by child class.
			///
			/// Called after world and camera deserialization by Scene::deserialize().
			///
			/// \param json Json object to retrieve data from.
			///
			void sub_deserialize(const nlohmann::json& json) override;

		private:
			///
			/// Default constructor.
			///
			GUIScene() = delete;

		private:
			///
			/// GUI for this scene.
			///
			ui::GUI m_gui;
		};
	} // namespace scenes
} // namespace galaxy

#endif