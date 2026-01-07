///
/// LuaGraphics.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/graphics/gl/GLEnums.hpp"
#include "galaxy/graphics/gl/Sampler.hpp"
#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/gl/ShaderBuffer.hpp"
#include "galaxy/graphics/gl/Texture.hpp"
#include "galaxy/graphics/gl/TextureView.hpp"
#include "galaxy/graphics/gl/Vertex.hpp"
#include "galaxy/graphics/gl/VertexBuffer.hpp"
#include "galaxy/graphics/gl/VertexArray.hpp"

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Enums.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/RenderData.hpp"
#include "galaxy/graphics/Renderer.hpp"
#include "galaxy/graphics/RenderPass.hpp"
#include "galaxy/graphics/RenderPipeline.hpp"
#include "galaxy/graphics/VertexBatch.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_graphics() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		// clang-format off
		lua.new_enum<GLDrawHint>(
			"GLDrawHint",
			{
				{"STATIC_DRAW", GLDrawHint::STATIC_DRAW},
				{"DYNAMIC_DRAW", GLDrawHint::DYNAMIC_DRAW},
				{"STREAM_DRAW", GLDrawHint::STREAM_DRAW}
			}
		);
		lua.new_enum<GLTextureMode>(
			"GLTextureMode",
			{
				{"REPEAT", GLTextureMode::REPEAT},
				{"MIRRORED_REPEAT", GLTextureMode::MIRRORED_REPEAT},
				{"CLAMP_TO_EDGE", GLTextureMode::CLAMP_TO_EDGE},
				{"CLAMP_TO_BORDER", GLTextureMode::CLAMP_TO_BORDER}
			}
		);
		lua.new_enum<GLRenderMode>(
			"GLRenderMode",
			{
				{"QUADS", GLRenderMode::QUADS},
				{"TRIANGLES", GLRenderMode::TRIANGLES},
				{"LINES", GLRenderMode::LINES},
				{"LINE_LOOP", GLRenderMode::LINE_LOOP},
				{"POINTS", GLRenderMode::POINTS}
			}
		);
		lua.new_enum<GLAttributeBinding>(
			"GLAttributeBinding",
			{
				{"POSITION_POINT", GLAttributeBinding::POSITION_POINT},
				{"TEXEL_POINT", GLAttributeBinding::TEXEL_POINT},
				{"INDEX_POINT", GLAttributeBinding::INDEX_POINT}
			}
		);
		lua.new_enum<GLBufferLocation>(
			"GLBufferLocation",
			{
				{"VERTEX_BUFFER_POINT", GLBufferLocation::VERTEX_BUFFER_POINT}
			}
		);
		lua.new_enum<GLBufferBinding>(
			"GLBufferBinding",
			{
				{"TEXTURE_HANDLES", GLBufferBinding::TEXTURE_HANDLES},
				{"CAMERA", GLBufferBinding::CAMERA},
				{"UNIFORMS", GLBufferBinding::UNIFORMS}
			}
		);
		lua.new_enum<GLTextureFilter>(
			"GLTextureFilter",
			{
				{"NEAREST", GLTextureFilter::NEAREST},
				{"BILINEAR", GLTextureFilter::BILINEAR},
				{"TRILINEAR", GLTextureFilter::TRILINEAR}
			}
		);
		// clang-format on

		auto sampler_type      = lua.new_usertype<Sampler>("Sampler", sol::constructors<Colour()>());
		sampler_type["set"]    = &Sampler::set;
		sampler_type["setf"]   = &Sampler::setf;
		sampler_type["bind"]   = &Sampler::bind;
		sampler_type["unbind"] = &Sampler::unbind;
		sampler_type["id"]     = &Sampler::id;

		auto shader_type                  = lua.new_usertype<Shader>("Shader", sol::constructors<Shader()>());
		shader_type["bind"]               = &Shader::bind;
		shader_type["compile"]            = &Shader::compile;
		shader_type["destroy"]            = &Shader::destroy;
		shader_type["id"]                 = &Shader::id;
		shader_type["load"]               = sol::resolve<bool(const std::string&, const std::string&)>(&Shader::load);
		shader_type["load_combined"]      = sol::resolve<bool(const std::string&)>(&Shader::load);
		shader_type["parse"]              = sol::resolve<bool(const std::string&, const std::string&)>(&Shader::parse);
		shader_type["parse_combined"]     = sol::resolve<bool(const std::string&)>(&Shader::parse);
		shader_type["unbind"]             = &Shader::unbind;
		shader_type["set_uniform_bool"]   = &Shader::set_uniform<bool>;
		shader_type["set_uniform_int"]    = &Shader::set_uniform<int>;
		shader_type["set_uniform_uint"]   = &Shader::set_uniform<unsigned int>;
		shader_type["set_uniform_float"]  = &Shader::set_uniform<float>;
		shader_type["set_uniform_double"] = &Shader::set_uniform<double>;
		shader_type["set_uniform_mat4"]   = &Shader::set_uniform<glm::mat4>;
		shader_type["set_uniform_vec2"]   = &Shader::set_uniform<glm::vec2>;
		shader_type["set_uniform_vec3"]   = &Shader::set_uniform<glm::vec3>;
		shader_type["set_uniform_vec4"]   = &Shader::set_uniform<glm::vec4>;
		shader_type["set_uniform_colour"] = &Shader::set_uniform<Colour>;

		auto shaderbuffer_type         = lua.new_usertype<ShaderBuffer>("ShaderBuffer", sol::constructors<ShaderBuffer(), ShaderBuffer(const GLBufferBinding)>());
		shaderbuffer_type["bind"]      = &ShaderBuffer::bind;
		shaderbuffer_type["clear"]     = &ShaderBuffer::clear;
		shaderbuffer_type["destroy"]   = &ShaderBuffer::destroy;
		shaderbuffer_type["id"]        = &ShaderBuffer::id;
		shaderbuffer_type["set_index"] = &ShaderBuffer::set_index;
		shaderbuffer_type["unbind"]    = &ShaderBuffer::unbind;
		// shaderbuffer_type[""]          = &ShaderBuffer::buffer;
		// shaderbuffer_type[""]          = &ShaderBuffer::sub_buffer;

		auto texture_type        = lua.new_usertype<Texture>("Texture", sol::constructors<Texture()>());
		texture_type["bind"]     = &Texture::bind;
		texture_type["destroy"]  = &Texture::destroy;
		texture_type["handle"]   = &Texture::handle;
		texture_type["height"]   = &Texture::height;
		texture_type["id"]       = &Texture::id;
		texture_type["load"]     = &Texture::load;
		texture_type["load_mem"] = &Texture::load_mem;
		texture_type["save"]     = &Texture::save;
		texture_type["unbind"]   = &Texture::unbind;
		texture_type["width"]    = &Texture::width;
		texture_type["get_view"] = &Texture::get_view;

		auto textureview_type = lua.new_usertype<TextureView>(
			"TextureView",
			sol::constructors<TextureView(const unsigned int, const unsigned int, const unsigned int, const unsigned int, const unsigned int)>()
		);
		textureview_type["id"] = &TextureView::id;

		auto vertex_type      = lua.new_usertype<Vertex>("Vertex", sol::constructors<Vertex()>());
		vertex_type["pos"]    = &Vertex::m_pos;
		vertex_type["texels"] = &Vertex::m_texels;
		vertex_type["index"]  = &Vertex::m_index;

		lua.set_function("gen_quad_vertices", &graphics::gen_quad_vertices);
		lua.set_function("gen_default_indices", &graphics::gen_default_indices);
		lua.set_function("map_x_texel", &graphics::map_x_texel<int>);
		lua.set_function("map_y_texel", &graphics::map_y_texel<int>);
		lua.set_function("map_x_texelf", &graphics::map_x_texel<float>);
		lua.set_function("map_y_texelf", &graphics::map_y_texel<float>);

		auto vertexbuffer_type                   = lua.new_usertype<VertexBuffer>("VertexBuffer", sol::constructors<VertexBuffer()>());
		vertexbuffer_type["clear"]               = &VertexBuffer::clear;
		vertexbuffer_type["count"]               = &VertexBuffer::count;
		vertexbuffer_type["id"]                  = &VertexBuffer::id;
		vertexbuffer_type["offset"]              = &VertexBuffer::offset;
		vertexbuffer_type["sub_buffer"]          = &VertexBuffer::sub_buffer;
		vertexbuffer_type["buffer"]              = &VertexBuffer::buffer;
		vertexbuffer_type["erase"]               = &VertexBuffer::erase;
		vertexbuffer_type["reserve"]             = &VertexBuffer::reserve;
		vertexbuffer_type["sub_buffer_indices"]  = &VertexBuffer::sub_buffer_indices;
		vertexbuffer_type["sub_buffer_vertices"] = &VertexBuffer::sub_buffer_vertices;

		auto vertexarray_type                   = lua.new_usertype<VertexArray>("VertexArray", sol::constructors<VertexArray()>());
		vertexarray_type["bind"]                = &VertexArray::bind;
		vertexarray_type["count"]               = &VertexArray::count;
		vertexarray_type["id"]                  = &VertexArray::id;
		vertexarray_type["offset"]              = &VertexArray::offset;
		vertexarray_type["sub_buffer"]          = &VertexArray::sub_buffer;
		vertexarray_type["unbind"]              = &VertexArray::unbind;
		vertexarray_type["vbo"]                 = &VertexArray::vbo;
		vertexarray_type["buffer"]              = &VertexArray::buffer;
		vertexarray_type["clear"]               = &VertexArray::clear;
		vertexarray_type["erase"]               = &VertexArray::erase;
		vertexarray_type["reserve"]             = &VertexArray::reserve;
		vertexarray_type["sub_buffer_indices"]  = &VertexArray::sub_buffer_indices;
		vertexarray_type["sub_buffer_vertices"] = &VertexArray::sub_buffer_vertices;

		auto cameradata_type          = lua.new_usertype<Camera::Data>("CameraData", sol::no_constructor);
		cameradata_type["model_view"] = &Camera::Data::m_model_view;
		cameradata_type["projection"] = &Camera::Data::m_projection;

		auto camera_type                      = lua.new_usertype<Camera>("Camera", sol::constructors<Camera()>());
		camera_type["get_data"]               = &Camera::get_data;
		camera_type["get_model_view"]         = &Camera::get_model_view;
		camera_type["get_origin"]             = &Camera::get_origin;
		camera_type["get_pos"]                = &Camera::get_pos;
		camera_type["get_proj"]               = &Camera::get_proj;
		camera_type["get_rotation"]           = &Camera::get_rotation;
		camera_type["get_scale"]              = &Camera::get_scale;
		camera_type["get_transform"]          = &Camera::get_transform;
		camera_type["rotation_speed"]         = &Camera::m_rotation_speed;
		camera_type["translation_speed"]      = &Camera::m_translation_speed;
		camera_type["allow_rotation"]         = &Camera::m_allow_rotation;
		camera_type["reset"]                  = &Camera::reset;
		camera_type["rotate"]                 = &Camera::rotate;
		camera_type["scale"]                  = &Camera::scale;
		camera_type["set_rotation"]           = &Camera::set_rotation;
		camera_type["set_origin"]             = &Camera::set_origin;
		camera_type["set_scale_horizontal"]   = &Camera::set_scale_horizontal;
		camera_type["set_scale_vertical"]     = &Camera::set_scale_vertical;
		camera_type["move"]                   = &Camera::move;
		camera_type["move_x"]                 = &Camera::move_x;
		camera_type["move_y"]                 = &Camera::move_y;
		camera_type["set_positon"]            = &Camera::set_positon;
		camera_type["set_positon_horizontal"] = &Camera::set_positon_horizontal;
		camera_type["set_positon_vertical"]   = &Camera::set_positon_vertical;
		camera_type["set_projection"]         = &Camera::set_projection;

		auto colour_type =
			lua.new_usertype<Colour>("Colour", sol::constructors<Colour(), Colour(const std::uint8_t, const std::uint8_t, const std::uint8_t, const std::uint8_t)>());
		colour_type["array"]         = &Colour::array;
		colour_type["normalize"]     = &Colour::normalize;
		colour_type["set_from_norm"] = &Colour::set_from_norm;
		colour_type["set_r"]         = sol::resolve<void(const std::uint8_t)>(&Colour::r);
		colour_type["set_g"]         = sol::resolve<void(const std::uint8_t)>(&Colour::g);
		colour_type["set_b"]         = sol::resolve<void(const std::uint8_t)>(&Colour::b);
		colour_type["set_a"]         = sol::resolve<void(const std::uint8_t)>(&Colour::a);
		colour_type["get_r"]         = sol::resolve<std::uint8_t() const>(&Colour::r);
		colour_type["get_g"]         = sol::resolve<std::uint8_t() const>(&Colour::g);
		colour_type["get_b"]         = sol::resolve<std::uint8_t() const>(&Colour::b);
		colour_type["get_a"]         = sol::resolve<std::uint8_t() const>(&Colour::a);
		lua.set("GALAXY_COL_OPAQUE", Colour::OPAQUE);
		lua.set("GALAXY_COL_TRANSPARENT", Colour::TRANSPARENT);
		lua.set("GALAXY_COL_OFFSET", Colour::OFFSET);
		lua.set("GALAXY_COL_LOWER", Colour::LOWER);
		lua.set("GALAXY_COL_UPPER", Colour::UPPER);

		auto rendercmd_type        = lua.new_usertype<RenderCmd>("RenderCmd", sol::constructors<RenderCmd()>());
		rendercmd_type["type"]     = &RenderCmd::type;
		rendercmd_type["vertices"] = &RenderCmd::vertices;
		rendercmd_type["vao"]      = &RenderCmd::vao;
		rendercmd_type["data"]     = &RenderCmd::data;

		auto renderdata_type         = lua.new_usertype<RenderData>("RenderData", sol::constructors<RenderData()>());
		renderdata_type["transform"] = &RenderData::transform;
		renderdata_type["colour"]    = &RenderData::colour;
		renderdata_type["entity"]    = &RenderData::entity;
		renderdata_type["point"]     = &RenderData::point;
		renderdata_type["handle"]    = &RenderData::handle;

		auto renderer_type             = lua.new_usertype<Renderer>("Renderer", sol::no_constructor);
		renderer_type["begin_pass"]    = &Renderer::begin_pass;
		renderer_type["bind_pipeline"] = &Renderer::bind_pipeline;
		renderer_type["end_pass"]      = &Renderer::end_pass;
		renderer_type["prepare"]       = &Renderer::prepare;
		renderer_type["push_cmd"]      = [](Renderer& renderer, RenderCmd& cmd) {
            renderer.push_cmd(std::move(cmd));
		};

		auto renderpipe_type              = lua.new_usertype<RenderPipeline>("RenderPipeline", sol::constructors<RenderPipeline()>());
		renderpipe_type["enabled"]        = &RenderPipeline::enabled;
		renderpipe_type["disabled"]       = &RenderPipeline::disabled;
		renderpipe_type["blend_equation"] = &RenderPipeline::blend_equation;
		renderpipe_type["blend_function"] = &RenderPipeline::blend_function;
		renderpipe_type["cull_face"]      = &RenderPipeline::cull_face;
		renderpipe_type["depth_func"]     = &RenderPipeline::depth_func;
		renderpipe_type["shader"]         = &RenderPipeline::shader;

		auto vertexbatch_type       = lua.new_usertype<VertexBatch>("VertexBatch", sol::constructors<VertexBatch(const int, const int, const std::vector<unsigned int>&)>());
		vertexbatch_type["bind"]    = &VertexBatch::bind;
		vertexbatch_type["count"]   = &VertexBatch::count;
		vertexbatch_type["flush"]   = &VertexBatch::flush;
		vertexbatch_type["offset"]  = &VertexBatch::offset;
		vertexbatch_type["prepare"] = &VertexBatch::prepare;
		vertexbatch_type["push"]    = &VertexBatch::push;
		vertexbatch_type["vao"]     = &VertexBatch::vao;
	}
} // namespace galaxy
