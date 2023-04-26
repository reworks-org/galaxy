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

#include "galaxy/core/Config.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Loader.hpp"

namespace galaxy
{
	namespace core
	{
		Loader::Loader()
		{
			auto& config = ServiceLocator<Config>::ref();
			auto& fs     = ServiceLocator<fs::VirtualFileSystem>::ref();
			auto& window = ServiceLocator<Window>::ref();

			auto doc = fs.find(config.get<std::string>("load_screen_rml"));

			m_renderer.init();

			m_context = Rml::CreateContext("GalaxyCoreLoader", Rml::Vector2i(window.get_width(), window.get_height()));

			auto rml_doc = m_context->LoadDocument(doc.string);
			if (rml_doc)
			{
				rml_doc->Show();
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load '{0}' for loading screen.", doc.string);
			}
		}

		Loader::~Loader()
		{
			m_renderer.destroy();
		}

		void Loader::load(const std::function<void(void)>& loader_func)
		{
			auto& tp        = ServiceLocator<BS::thread_pool>::ref();
			m_thread_handle = tp.submit(loader_func);

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