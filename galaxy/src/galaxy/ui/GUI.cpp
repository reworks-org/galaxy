///
/// GUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/Renderer.hpp"

#include "GUI.hpp"

namespace galaxy
{
	namespace ui
	{
		GUI::GUI()
		    : m_state {ConstructionState::DEFAULT}, m_id_counter {0}, m_theme {nullptr}, m_sb {50}
		{
		}

		GUI::~GUI()
		{
			if (m_state != ConstructionState::DEFAULT)
			{
				destroy();
			}
		}

		void GUI::set_theme(UITheme* theme)
		{
			m_theme = theme;
			m_state = ConstructionState::THEME_SET;

			auto* atlas = m_theme->m_atlas.get_atlas();
			auto* cast  = dynamic_cast<graphics::BaseTexture*>(atlas);
			m_sb.set_texture(cast);
		}

		void GUI::remove(const unsigned int id)
		{
			if (id >= m_widgets.size())
			{
				GALAXY_LOG(GALAXY_ERROR, "Invalid widget id.");
			}
			else
			{
				// Don't erase because that will mess up ordering.
				m_widgets[id].reset();
				m_widgets[id] = nullptr;

				m_free.push_back(id);
			}
		}

		void GUI::destroy()
		{
			for (auto& widget : m_widgets)
			{
				widget.reset();
			}

			m_widgets.clear();

			m_state = ConstructionState::DEFAULT;
		}

		void GUI::update(const double dt)
		{
			for (const auto& widget : m_widgets)
			{
				widget->update(dt);
			}
		}

		void GUI::render(graphics::Camera& camera)
		{
			m_sb.update();

			auto shader = m_theme->m_shaders->get("spritebatch");
			shader->bind();
			shader->set_uniform<glm::mat4>("u_cameraProj", camera.get_proj());
			shader->set_uniform<glm::mat4>("u_cameraView", camera.get_transform());

			m_theme->m_renderer->draw_sprite_batch(m_sb, *shader);

			for (const auto& widget : m_widgets)
			{
				widget->render(camera);
			}
		}
	} // namespace ui
} // namespace galaxy