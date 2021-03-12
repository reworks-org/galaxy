///
/// Scene.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_SCENES_SCENE_HPP_
#define GALAXY_SCENES_SCENE_HPP_

#include "galaxy/core/World.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/scenes/SceneTypes.hpp"

namespace galaxy
{
	namespace scenes
	{
		///
		/// Represents a scene within the layer.
		///
		class Scene : public fs::Serializable
		{
		public:
			///
			/// Type constructor.
			///
			/// \param name Name of the scene.
			/// \param type Scene type as an enum.
			///
			Scene(std::string_view name, Types type) noexcept;

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

			///
			/// Get scene name.
			///
			/// \return Const std::string reference to the name.
			///
			[[nodiscard]] const std::string& get_name() const noexcept;

			///
			/// Get scene type.
			///
			/// \return Enum type.
			///
			[[nodiscard]] const Types get_type() const noexcept;

			///
			/// Get scene world.
			///
			/// \return Reference to internal scene world.
			///
			[[nodiscard]] core::World& world() noexcept;

			///
			/// Get scene camera.
			///
			/// \return Reference to internal scene camera.
			///
			[[nodiscard]] graphics::Camera& camera() noexcept;

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

			///
			/// \brief Serialize function to be called by child class.
			///
			/// Called after world and camera serialization by Scene::serialize().
			///
			/// \return JSON object containing data to write out.
			///
			[[nodiscard]] virtual nlohmann::json sub_serialize() = 0;

			///
			/// \brief Deserialize function to be called by child class.
			///
			/// Called after world and camera deserialization by Scene::deserialize().
			///
			/// \param json Json object to retrieve data from.
			///
			virtual void sub_deserialize(const nlohmann::json& json) = 0;

		private:
			///
			/// Deleted default constructor.
			///
			Scene() = delete;

		protected:
			///
			/// Scene type.
			///
			Types m_type;

			///
			/// Scene name.
			///
			std::string m_name;

			///
			/// Entity/System manager.
			///
			core::World m_world;

			///
			/// Camera.
			///
			graphics::Camera m_camera;
		};
	} // namespace scenes

	namespace meta
	{
		template<typename Type>
		concept is_scene = std::derived_from<Type, scenes::Scene>;
	} // namespace meta
} // namespace galaxy

#endif