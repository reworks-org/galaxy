///
/// Scene3D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENES_SCENE3D_HPP_
#define GALAXY_SCENES_SCENE3D_HPP_

#include "galaxy/graphics/Camera3D.hpp"
#include "galaxy/graphics/Skybox.hpp"
#include "galaxy/scenes/Scene.hpp"

namespace galaxy
{
	namespace scene
	{
		///
		/// 3D scene implementation.
		///
		class Scene3D final : public Scene
		{
		public:
			///
			/// Argument Constructor.
			///
			/// \param name Name of the Scene3D.
			///
			Scene3D(std::string_view name) noexcept;

			///
			/// Destructor.
			///
			virtual ~Scene3D() noexcept;

			///
			/// On push of Scene3D to stack.
			///
			void on_push() override;

			///
			/// On pop of Scene3D off stack.
			///
			void on_pop() override;

			///
			/// Process events.
			///
			void events() override;

			///
			/// Process update (fixed timestep) logic.
			///
			/// \param dt Delta-Time from fixed timestep gameloop.
			///
			void update(const double dt) override;

			///
			/// Code to be called before rendering. Outside of any glBegin, window.begin(), etc...
			///
			void pre_render() override;

			///
			/// Render to screen.
			///
			void render() override;

			///
			/// Move camera.
			///
			/// \param x X distance to move.
			/// \param y Y distance to move.
			/// \param z Z distance to move.
			///
			void move(const float x, const float y, const float z) noexcept override;

			///
			/// Get camera.
			///
			/// \return Reference to scene Camera3D.
			///
			[[nodiscard]] graphics::Camera3D& camera() noexcept;

			///
			/// Get skybox.
			///
			/// \return Reference to scene skybox.
			///
			[[nodiscard]] graphics::Skybox& skybox() noexcept;

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

		private:
			///
			/// Deleted default constructor.
			///
			Scene3D() = delete;

		private:
			///
			/// Scene camera.
			///
			graphics::Camera3D m_camera;

			///
			/// Scene skybox.
			///
			graphics::Skybox m_skybox;
		};
	} // namespace scene
} // namespace galaxy

#endif