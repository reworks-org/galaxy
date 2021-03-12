///
/// GUIScene.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "GUIScene.hpp"

namespace galaxy
{
	namespace scenes
	{
		GUIScene::GUIScene(std::string_view name) noexcept
		    : Scene {name, Types::GUI}
		{
			m_camera.create(0.0f, 1024, 1024, 0.0f);
			m_camera.set_speed(100.0f);
		}

		GUIScene::~GUIScene() noexcept
		{
		}

		void GUIScene::on_push()
		{
		}

		void GUIScene::on_pop()
		{
		}

		void GUIScene::events()
		{
		}

		void GUIScene::update(const double dt)
		{
			m_camera.update(dt);
		}

		void GUIScene::pre_render()
		{
		}

		void GUIScene::render()
		{
		}

		nlohmann::json GUIScene::sub_serialize()
		{
			nlohmann::json json = "{}"_json;

			return json;
		}

		void GUIScene::sub_deserialize(const nlohmann::json& json)
		{
		}
	} // namespace scenes
} // namespace galaxy