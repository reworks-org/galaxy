///
/// Shaders.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <ranges>

#include "galaxy/utils/StringUtils.hpp"

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/graphics/DefaultShaders.hpp"

#include "Shaders.hpp"

namespace galaxy
{
	namespace resource
	{
		Shaders::Shaders() noexcept
			: m_folder {""}
		{
		}

		Shaders::~Shaders() noexcept
		{
		}

		void Shaders::load(std::string_view folder)
		{
			m_folder = static_cast<std::string>(folder);

			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto contents = fs.list_directory(m_folder);
			if (!contents.empty())
			{
				for (auto& file : contents)
				{
					file = strutils::replace_first(file, GALAXY_VERTEX_EXT, "");
					file = strutils::replace_first(file, GALAXY_FRAGMENT_EXT, "");
				}

				auto unique_range = std::ranges::unique(contents);
				contents.erase(unique_range.begin(), unique_range.end());

				std::ranges::sort(contents);
				unique_range = std::ranges::unique(contents);
				contents.erase(unique_range.begin(), unique_range.end());

				for (const auto& file : contents)
				{
					const auto name = std::filesystem::path(file).stem().string();
					m_cache[name]   = std::make_shared<graphics::Shader>(file + GALAXY_VERTEX_EXT, file + GALAXY_FRAGMENT_EXT);
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Found no shaders to load in '{0}'.", m_folder);
			}

			// Now load default shaders.
			m_cache["Sprite"] = std::make_shared<graphics::Shader>();
			m_cache["Sprite"]->load_raw(shaders::sprite_vert, shaders::sprite_frag);

			m_cache["Line"] = std::make_shared<graphics::Shader>();
			m_cache["Line"]->load_raw(shaders::line_vert, shaders::line_frag);

			m_cache["Point"] = std::make_shared<graphics::Shader>();
			m_cache["Point"]->load_raw(shaders::point_vert, shaders::point_frag);

			m_cache["Text"] = std::make_shared<graphics::Shader>();
			m_cache["Text"]->load_raw(shaders::text_vert, shaders::text_frag);

			m_cache["RenderToTexture"] = std::make_shared<graphics::Shader>();
			m_cache["RenderToTexture"]->load_raw(shaders::render_to_texture_vert, shaders::render_to_texture_frag);
		}

		void Shaders::reload()
		{
			if (!m_folder.empty())
			{
				clear();
				load(m_folder);
				compile();
			}
		}

		void Shaders::compile()
		{
			for (auto&& [key, shader] : m_cache)
			{
				shader->compile();
			}
		}
	} // namespace resource
} // namespace galaxy