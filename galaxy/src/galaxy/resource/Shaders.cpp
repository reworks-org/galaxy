///
/// Shaders.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/graphics/DefaultShaders.hpp"

#include "Shaders.hpp"

namespace galaxy
{
	namespace resource
	{
		void ShaderLoader::build(robin_hood::unordered_node_map<std::uint64_t, std::shared_ptr<graphics::Shader>>& cache)
		{
			// Load default shaders first.
			cache.emplace(algorithm::fnv1a_64("RenderToTexture"),
				std::make_shared<graphics::Shader>(shaders::render_to_texture_vert, shaders::render_to_texture_frag));

			for (auto&& [key, shader] : cache)
			{
				shader->compile();
			}
		}

		std::shared_ptr<graphics::Shader> ShaderLoader::operator()(const std::string& file)
		{
			auto ptr = std::make_shared<graphics::Shader>();
			ptr->load(file);

			return ptr;
		}
	} // namespace resource
} // namespace galaxy
