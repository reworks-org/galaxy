///
/// ShaderBook.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/fs/FileSystem.hpp"
#include "galaxy/scripting/JSONUtils.hpp"
#include "galaxy/graphics/shader/BlinnPhong.hpp"
#include "galaxy/graphics/shader/DefaultFramebuffer.hpp"
#include "galaxy/graphics/shader/Glyph.hpp"
#include "galaxy/graphics/shader/Line.hpp"
#include "galaxy/graphics/shader/LightObject.hpp"
#include "galaxy/graphics/shader/Model.hpp"
#include "galaxy/graphics/shader/Phong.hpp"
#include "galaxy/graphics/shader/Point.hpp"
#include "galaxy/graphics/shader/RenderToTexture.hpp"
#include "galaxy/graphics/shader/Skybox.hpp"
#include "galaxy/graphics/shader/Sprite.hpp"
#include "galaxy/graphics/shader/SpriteBatch.hpp"
#include "galaxy/graphics/shader/Text.hpp"

#include "ShaderBook.hpp"

namespace galaxy
{
	namespace res
	{
		ShaderBook::ShaderBook()
		    : m_vert_ext {".vs"}, m_frag_ext {".fs"}
		{
			create_default();
			create_post_shaders();
		}

		ShaderBook::ShaderBook(std::string_view file)
		{
			create_from_json(file);
		}

		ShaderBook::~ShaderBook() noexcept
		{
			clear();
		}

		void ShaderBook::create_from_json(std::string_view file)
		{
			const auto json_opt = json::parse_from_disk(file);
			if (json_opt == std::nullopt)
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to create parse/load json file: {0}, for Shaderbook.", file);
			}
			else
			{
				deserialize(json_opt.value());
			}
		}

		void ShaderBook::clear() noexcept
		{
			m_resources.clear();
		}

		nlohmann::json ShaderBook::serialize()
		{
			nlohmann::json json        = "{\"shaderbook\":[]}"_json;
			json["vertex-extension"]   = m_vert_ext;
			json["fragment-extension"] = m_frag_ext;

			for (const auto& [name, shader] : m_resources)
			{
				json["shaderbook"].push_back(name);
			}

			return json;
		}

		void ShaderBook::deserialize(const nlohmann::json& json)
		{
			clear();
			create_default();
			create_post_shaders();

			m_vert_ext = json.at("vertex-extension");
			m_frag_ext = json.at("fragment-extension");

			const auto& arr = json.at("shaderbook");
			for (const std::string& filename : arr)
			{
				create(filename, filename + m_vert_ext, filename + m_frag_ext);
			}
		}

		void ShaderBook::create_default()
		{
			auto* df = create("DefaultFramebuffer");
			df->load_raw(shaders::default_framebuffer_vert, shaders::default_framebuffer_frag);

			auto* glyph = create("glyph");
			glyph->load_raw(shaders::glyph_vert, shaders::glyph_frag);

			auto* line = create("line");
			line->load_raw(shaders::line_vert, shaders::line_frag);

			auto* model = create("model");
			model->load_raw(shaders::model_vert, shaders::model_frag);

			auto* point = create("point");
			point->load_raw(shaders::point_vert, shaders::point_frag);

			auto* render_to_texture = create("render_to_texture");
			render_to_texture->load_raw(shaders::render_to_texture_vert, shaders::render_to_texture_frag);

			auto* skybox = create("skybox");
			skybox->load_raw(shaders::skybox_vert, shaders::skybox_frag);

			auto* sprite = create("sprite");
			sprite->load_raw(shaders::sprite_vert, shaders::sprite_frag);

			auto* spritebatch = create("spritebatch");
			spritebatch->load_raw(shaders::spritebatch_vert, shaders::spritebatch_frag);

			auto* text = create("text");
			text->load_raw(shaders::text_vert, shaders::text_frag);
		}

		void ShaderBook::create_post_shaders()
		{
			auto* phong = create("phong");
			phong->load_raw(shaders::model_vert, shaders::phong_frag);

			auto* light_object = create("light_object");
			light_object->load_raw(shaders::light_object_vert, shaders::light_object_frag);

			auto* blinn_phong = create("blinn_phong");
			blinn_phong->load_raw(shaders::model_vert, shaders::blinnphong_frag);
		}
	} // namespace res
} // namespace galaxy