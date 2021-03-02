///
/// GUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>
#include <optional>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer2D.hpp"
#include "galaxy/res/Shaderbook.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "GUI.hpp"

namespace galaxy
{
	namespace ui
	{
		GUI::GUI() noexcept
		    : m_state {ConstructionState::DEFAULT}, m_id_counter {0}, m_theme {nullptr}
		{
			m_batch_shader = SL_HANDLE.shaderbook()->get("spritebatch");
		}

		GUI::~GUI()
		{
			if (m_state != ConstructionState::DEFAULT)
			{
				destroy();
			}
		}

		void GUI::set_theme(Theme* theme)
		{
			m_theme = theme;
			m_state = ConstructionState::THEME_SET;

			m_theme->m_sb.set_texture(SL_HANDLE.atlas()->get_atlas());
		}

		void GUI::update(const double dt)
		{
			std::for_each(/*std::execution::par, */ m_widgets.begin(), m_widgets.end(), [&](const auto& widget) {
				widget->update(dt);
			});
		}

		void GUI::render()
		{
			if (!m_theme->m_sb.empty())
			{
				m_theme->m_sb.calculate();
				m_batch_shader->bind();
				m_batch_shader->set_uniform("u_cameraProj", m_theme->m_camera.get_proj());
				m_batch_shader->set_uniform("u_cameraView", m_theme->m_camera.get_view());
				m_batch_shader->set_uniform("u_width", static_cast<float>(m_theme->m_sb.get_width()));
				m_batch_shader->set_uniform("u_height", static_cast<float>(m_theme->m_sb.get_height()));

				m_theme->m_sb.bind();
				glDrawElements(GL_TRIANGLES, m_theme->m_sb.get_used_index_count(), GL_UNSIGNED_INT, nullptr);
			}

			for (const auto& widget : m_widgets)
			{
				widget->render();
			}
		}

		void GUI::remove(const unsigned int id)
		{
			if (id >= m_widgets.size())
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to remove widget that does not exist: {0}.", id);
			}
			else
			{
				// Don't erase because that will mess up ordering.
				m_widgets[id].reset();
				m_widgets[id] = nullptr;

				m_free.emplace_back(id);
			}
		}

		void GUI::destroy()
		{
			for (auto& widget : m_widgets)
			{
				widget.reset();
			}

			m_widgets.clear();
			m_batch_shader = nullptr;
			m_state        = ConstructionState::DEFAULT;
		}

	} // namespace ui
} // namespace galaxy