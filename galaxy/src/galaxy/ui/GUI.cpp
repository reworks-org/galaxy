///
/// GUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <execution>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/res/TextureAtlas.hpp"

#include "GUI.hpp"

namespace galaxy
{
	namespace ui
	{
		GUI::GUI() noexcept
		    : m_state {ConstructionState::DEFAULT}, m_id_counter {0}, m_theme {nullptr}, m_sb {100}
		{
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

			m_sb.set_texture(SL_HANDLE.atlas()->get_atlas());
		}

		void GUI::update(const double dt)
		{
			std::for_each(std::execution::par, m_widgets.begin(), m_widgets.end(), [&](const auto& widget) {
				widget->update(dt);
			});
		}

		void GUI::render(graphics::Camera& camera)
		{
			if (!m_sb.empty())
			{
				m_sb.calculate();
				graphics::Renderer::draw_batch(&m_sb, camera);
			}

			for (const auto& widget : m_widgets)
			{
				widget->render(camera);
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
			m_state = ConstructionState::DEFAULT;
		}

	} // namespace ui
} // namespace galaxy