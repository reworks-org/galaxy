///
/// NuklearUI.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/Config.hpp"
#include "galaxy/core/Window.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

#include "NuklearUI.hpp"

namespace galaxy
{
	namespace ui
	{
		NuklearUI::NuklearUI()
		{
			m_context = nk_glfw3_init(core::ServiceLocator<core::Window>::ref().handle(), GALAXY_NUKLEAR_MAX_VERTEX_BUFFER, GALAXY_NUKLEAR_MAX_ELEMENT_BUFFER);
			if (!m_context)
			{
				GALAXY_LOG(GALAXY_FATAL, "Failed to create nuklear ui context.");
			}
			else
			{
				const auto dir      = core::ServiceLocator<core::Config>::ref().get<std::string>("font_folder", "resource_folders");
				const auto contents = core::ServiceLocator<fs::VirtualFileSystem>::ref().list_directory(dir);

				if (!contents.empty())
				{
					nk_font_atlas* atlas;
					nk_glfw3_font_stash_begin(&atlas);

					for (const auto& file : contents)
					{
						const auto path               = std::filesystem::path(file);
						m_fonts[path.stem().string()] = nk_font_atlas_add_from_file(atlas, path.string().c_str(), GALAXY_NUKLEAR_DEFAULT_FONT_SIZE, nullptr);
					}

					nk_glfw3_font_stash_end();
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Found no fonts to load into RmlUi at '{0}'.", dir);
				}
			}
		}

		NuklearUI::~NuklearUI() noexcept
		{
			nk_glfw3_shutdown();
			m_context = nullptr; // Freed by nk_glfw3_shutdown().
		}

		void NuklearUI::new_frame() noexcept
		{
			nk_glfw3_new_frame();
		}

		void NuklearUI::render() noexcept
		{
			nk_glfw3_render(NK_ANTI_ALIASING_ON);
		}

		void NuklearUI::set_font(const std::string& font) noexcept
		{
			if (m_fonts.contains(font))
			{
				nk_style_set_font(m_context, &m_fonts[font]->handle);
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Attempted to set font not loaded into nuklear: {0}.", font);
			}
		}

		nk_context* NuklearUI::handle() noexcept
		{
			return m_context;
		}

		nk_glfw* NuklearUI::data() noexcept
		{
			return nk_glfw3_data();
		}
	} // namespace ui
} // namespace galaxy