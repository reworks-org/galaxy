///
/// Loading.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <BS_thread_pool.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <RmlUi/Core.h>
#include <RmlUi/Core/Context.h>
#include <RmlUi/Debugger.h>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"

#include "Loading.hpp"

namespace galaxy
{
	namespace core
	{
		Loading::Loading(const std::string& doc)
		{
			m_renderer.init();

			auto& window = ServiceLocator<Window>::ref();

			m_context = Rml::CreateContext("loading", Rml::Vector2i(window.get_width(), window.get_height()));
			Rml::Debugger::Initialise(m_context);

			auto rml_doc = m_context->LoadDocument(doc);
			if (rml_doc)
			{
				rml_doc->Show();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load '{0}' for loading screen.", doc);
			}
		}

		Loading::~Loading()
		{
			m_renderer.destroy();
		}

		void Loading::start(const std::function<void(void)>& lambda)
		{
			auto& tp        = ServiceLocator<BS::thread_pool>::ref();
			m_thread_handle = tp.submit(lambda);
		}

		void Loading::display()
		{
			// Use custom rendering interface for load screen.
			auto old = Rml::GetRenderInterface();
			Rml::SetRenderInterface(&m_renderer);

			while (!meta::is_work_done(m_thread_handle))
			{
				glfwPollEvents();
				m_context->Update();

				m_renderer.begin_frame();
				m_context->Render();
				m_renderer.end_frame();
			}

			m_thread_handle.get();
			Rml::SetRenderInterface(old);
		}
	} // namespace core
} // namespace galaxy