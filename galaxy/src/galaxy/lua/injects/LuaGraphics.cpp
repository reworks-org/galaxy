///
/// LuaGraphics.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>
#include <sol/sol.hpp>

#include "galaxy/graphics/gl/GLEnums.hpp"
#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/gl/ShaderStorageBuffer.hpp"
#include "galaxy/graphics/gl/Texture.hpp"
#include "galaxy/graphics/gl/Vertex.hpp"
#include "galaxy/graphics/gl/VertexBuffer.hpp"
#include "galaxy/graphics/gl/VertexArray.hpp"

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/Enums.hpp"

#include "../Lua.hpp"

namespace galaxy
{
	void Lua::inject_graphics() noexcept
	{
		auto& lua = entt::locator<sol::state>::value();

		lua.set_function("gen_quad_vertices", &graphics::gen_quad_vertices);
		lua.set_function("gen_default_indices", &graphics::gen_default_indices);
		lua.set_function("map_x_texel", &graphics::map_x_texel<float>);
		lua.set_function("map_y_texel", &graphics::map_y_texel<float>);

		// clang-format off
		lua.new_enum<GLDrawHint>("GLDrawHint",
		{
			{"STATIC_DRAW", GLDrawHint::STATIC_DRAW},
			{"DYNAMIC_DRAW", GLDrawHint::DYNAMIC_DRAW},
			{"STREAM_DRAW", GLDrawHint::STREAM_DRAW}
		});

		lua.new_enum<GLAttributeBinding>("GLAttributeBinding",
		{
			{"POSITION_POINT", GLAttributeBinding::POSITION_POINT},
			{"TEXEL_POINT", GLAttributeBinding::TEXEL_POINT},
			{"OFFSET_POINT", GLAttributeBinding::OFFSET_POINT}
		});

		lua.new_enum<GLBufferBinding>("GLBufferBinding",
		{
			{"VERTEX_BUFFER_POINT", GLBufferBinding::VERTEX_BUFFER_POINT},
			{"INSTANCE_BUFFER_POINT", GLBufferBinding::INSTANCE_BUFFER_POINT}
		});

		lua.new_enum<GLTextureMode>("GLTextureMode",
		{
			{"REPEAT", GLTextureMode::REPEAT},
			{"MIRRORED_REPEAT", GLTextureMode::MIRRORED_REPEAT},
			{"CLAMP_TO_EDGE", GLTextureMode::CLAMP_TO_EDGE},
			{"CLAMP_TO_BORDER", GLTextureMode::CLAMP_TO_BORDER}
		});

		lua.new_enum<GLTextureFilter>("GLTextureFilter",
		{
			{"NEAREST", GLTextureFilter::NEAREST},
			{"TRILINEAR", GLTextureFilter::TRILINEAR}
		});
		// clang-format on

		auto colour_type =
			lua.new_usertype<Colour>("Colour", sol::constructors<Colour(), Colour(const std::uint8_t, const std::uint8_t, const std::uint8_t, const std::uint8_t)>());
		colour_type["array"]         = &Colour::array;
		colour_type["normalize"]     = &Colour::normalize;
		colour_type["set_from_norm"] = &Colour::set_from_norm;
		// colour_type["OPAQUE"]        = &Colour::OPAQUE;
		// colour_type["TRANSPARENT"]   = &Colour::TRANSPARENT;
		colour_type["set_r"] = sol::resolve<void(const std::uint8_t)>(&Colour::r);
		colour_type["set_g"] = sol::resolve<void(const std::uint8_t)>(&Colour::g);
		colour_type["set_b"] = sol::resolve<void(const std::uint8_t)>(&Colour::b);
		colour_type["set_a"] = sol::resolve<void(const std::uint8_t)>(&Colour::a);
		colour_type["get_r"] = sol::resolve<std::uint8_t() const>(&Colour::r);
		colour_type["get_g"] = sol::resolve<std::uint8_t() const>(&Colour::g);
		colour_type["get_b"] = sol::resolve<std::uint8_t() const>(&Colour::b);
		colour_type["get_a"] = sol::resolve<std::uint8_t() const>(&Colour::a);

		auto shader_type              = lua.new_usertype<Shader>("Shader", sol::constructors<Shader()>());
		shader_type["bind"]           = &Shader::bind;
		shader_type["compile"]        = &Shader::compile;
		shader_type["destroy"]        = &Shader::destroy;
		shader_type["id"]             = &Shader::id;
		shader_type["load"]           = sol::resolve<bool(const std::string&, const std::string&)>(&Shader::load);
		shader_type["load_combined"]  = sol::resolve<bool(const std::string&)>(&Shader::load);
		shader_type["parse"]          = sol::resolve<bool(const std::string&, const std::string&)>(&Shader::parse);
		shader_type["parse_combined"] = sol::resolve<bool(const std::string&)>(&Shader::parse);
		shader_type["unbind"]         = &Shader::unbind;

		auto vertex_type      = lua.new_usertype<Vertex>("Vertex", sol::constructors<Vertex()>());
		vertex_type["pos"]    = &Vertex::m_pos;
		vertex_type["texels"] = &Vertex::m_texels;

		auto vertexbuffer_type                = lua.new_usertype<VertexBuffer>("VertexBuffer", sol::constructors<VertexBuffer()>());
		vertexbuffer_type["buffer"]           = sol::resolve<void(std::span<Vertex>, std::span<unsigned int>)>(&VertexBuffer::buffer);
		vertexbuffer_type["buffer_no_upload"] = sol::resolve<void(const int, std::span<unsigned int>)>(&VertexBuffer::buffer);
		vertexbuffer_type["clear"]            = &VertexBuffer::clear;
		vertexbuffer_type["count"]            = &VertexBuffer::count;
		vertexbuffer_type["id"]               = &VertexBuffer::id;
		vertexbuffer_type["offset"]           = &VertexBuffer::offset;
		vertexbuffer_type["sub_buffer"]       = &VertexBuffer::sub_buffer;

		auto vertexarray_type                = lua.new_usertype<VertexArray>("VertexArray", sol::constructors<VertexArray()>());
		vertexarray_type["bind"]             = &VertexArray::bind;
		vertexarray_type["buffer"]           = sol::resolve<void(std::span<Vertex>, std::span<unsigned int>)>(&VertexArray::buffer);
		vertexarray_type["buffer_no_upload"] = sol::resolve<void(const int, std::span<unsigned int>)>(&VertexArray::buffer);
		vertexarray_type["count"]            = &VertexArray::count;
		vertexarray_type["id"]               = &VertexArray::id;
		vertexarray_type["offset"]           = &VertexArray::offset;
		vertexarray_type["sub_buffer"]       = &VertexArray::sub_buffer;
		vertexarray_type["unbind"]           = &VertexArray::unbind;
		vertexarray_type["vbo"]              = &VertexArray::vbo;

		auto texture_type        = lua.new_usertype<Texture>("Texture", sol::constructors<Texture()>());
		texture_type["bind"]     = &Texture::bind;
		texture_type["destroy"]  = &Texture::destroy;
		texture_type["handle"]   = &Texture::handle;
		texture_type["height"]   = &Texture::height;
		texture_type["id"]       = &Texture::id;
		texture_type["load"]     = &Texture::load;
		texture_type["load_mem"] = &Texture::load_mem;
		texture_type["recreate"] = &Texture::recreate;
		texture_type["save"]     = &Texture::save;
		texture_type["unbind"]   = &Texture::unbind;
		texture_type["width"]    = &Texture::width;

		auto ssbo_type = lua.new_usertype<ShaderStorageBuffer>("ShaderStorageBuffer", sol::constructors<ShaderStorageBuffer(const int)>());
		// ssbo_type[""]  = &ShaderStorageBuffer::buffer;
		// ssbo_type[""]  = &ShaderStorageBuffer::sub_buffer;
		ssbo_type[""] = &ShaderStorageBuffer::bind;
		ssbo_type[""] = &ShaderStorageBuffer::clear;
		ssbo_type[""] = &ShaderStorageBuffer::destroy;
		ssbo_type[""] = &ShaderStorageBuffer::id;
		ssbo_type[""] = &ShaderStorageBuffer::unbind;

		// auto animation_type       = lua.new_usertype<graphics::Animation>("Animation", sol::constructors<graphics::Animation()>());
		// animation_type["current"] = &graphics::Animation::current;
		// animation_type["frames"]  = &graphics::Animation::frames;
		// animation_type["index"]   = &graphics::Animation::index;
		// animation_type["load"]    = &graphics::Animation::load;
		// animation_type["name"]    = &graphics::Animation::m_name;
		// animation_type["speed"]   = &graphics::Animation::m_speed;
		// animation_type["next"]    = &graphics::Animation::next;
		// animation_type["prev"]    = &graphics::Animation::prev;
		// animation_type["restart"] = &graphics::Animation::restart;
		//// animation_type["set_frames"] = &graphics::Animation::set_frames;
		// animation_type["total"] = &graphics::Animation::total;

		// auto frame_type        = lua.new_usertype<graphics::Frame>("Frame", sol::constructors<graphics::Frame()>());
		// frame_type["bounds"]   = &graphics::Frame::m_bounds;
		// frame_type["duration"] = &graphics::Frame::m_duration;

		// auto cameradata_type          = lua.new_usertype<graphics::Camera::Data>("CameraData", sol::no_constructor);
		// cameradata_type["model_view"] = &graphics::Camera::Data::m_model_view;
		// cameradata_type["projection"] = &graphics::Camera::Data::m_projection;

		// auto camera_type                    = lua.new_usertype<graphics::Camera>("Camera", sol::constructors<graphics::Camera()>());
		// camera_type["get_data"]             = &graphics::Camera::get_data;
		// camera_type["get_model_view"]       = &graphics::Camera::get_model_view;
		// camera_type["get_origin"]           = &graphics::Camera::get_origin;
		// camera_type["get_pos"]              = &graphics::Camera::get_pos;
		// camera_type["get_proj"]             = &graphics::Camera::get_proj;
		// camera_type["get_rotation"]         = &graphics::Camera::get_rotation;
		// camera_type["get_scale"]            = &graphics::Camera::get_scale;
		// camera_type["get_transform"]        = &graphics::Camera::get_transform;
		// camera_type["get_viewport"]         = &graphics::Camera::get_viewport;
		// camera_type["rotation_speed"]       = &graphics::Camera::m_rotation_speed;
		// camera_type["translation_speed"]    = &graphics::Camera::m_translation_speed;
		// camera_type["allow_rotation"]       = &graphics::Camera::m_allow_rotation;
		// camera_type["reset"]                = &graphics::Camera::reset;
		// camera_type["rotate"]               = &graphics::Camera::rotate;
		// camera_type["scale"]                = &graphics::Camera::scale;
		// camera_type["set_viewport"]         = &graphics::Camera::set_viewport;
		// camera_type["set_pos"]              = &graphics::Camera::set_pos;
		// camera_type["set_rotation"]         = &graphics::Camera::set_rotation;
		// camera_type["set_origin"]           = &graphics::Camera::set_origin;
		// camera_type["set_scale_horizontal"] = &graphics::Camera::set_scale_horizontal;
		// camera_type["set_scale_vertical"]   = &graphics::Camera::set_scale_vertical;
		// camera_type["translate"]            = &graphics::Camera::translate;

		// lua.set("galaxy_colour_opaque", graphics::Colour::OPAQUE);
		// lua.set("galaxy_colour_transparent", graphics::Colour::TRANSPARENT);

		// auto rendercommand_type         = lua.new_usertype<graphics::RenderCommand>("RenderCommand", sol::constructors<graphics::RenderCommand()>());
		// rendercommand_type["count"]     = &graphics::RenderCommand::count;
		// rendercommand_type["instances"] = &graphics::RenderCommand::instances;
		// rendercommand_type["layer"]     = &graphics::RenderCommand::layer;
		// rendercommand_type["mode"]      = &graphics::RenderCommand::mode;
		// rendercommand_type["offset"]    = &graphics::RenderCommand::offset;
		// rendercommand_type["uniforms"]  = &graphics::RenderCommand::uniforms;
		// rendercommand_type["vao"]       = &graphics::RenderCommand::vao;
		// rendercommand_type["texture"]   = &graphics::RenderCommand::texture;

		// auto renderdata_type         = lua.new_usertype<graphics::RenderData>("RenderData", sol::constructors<graphics::RenderData()>());
		// renderdata_type["colour"]    = &graphics::RenderData::colour;
		// renderdata_type["entity"]    = &graphics::RenderData::entity;
		// renderdata_type["point"]     = &graphics::RenderData::point;
		// renderdata_type["textured"]  = &graphics::RenderData::textured;
		// renderdata_type["transform"] = &graphics::RenderData::transform;

		// auto transform_type                    = lua.new_usertype<graphics::Transform>("Transform", sol::constructors<graphics::Transform()>());
		// transform_type["get_origin"]           = &graphics::Transform::get_origin;
		// transform_type["get_pos"]              = &graphics::Transform::get_pos;
		// transform_type["get_rotation"]         = &graphics::Transform::get_rotation;
		// transform_type["get_scale"]            = &graphics::Transform::get_scale;
		// transform_type["get_transform"]        = &graphics::Transform::get_transform;
		// transform_type["reset"]                = &graphics::Transform::reset;
		// transform_type["rotate"]               = &graphics::Transform::rotate;
		// transform_type["scale"]                = &graphics::Transform::scale;
		// transform_type["set_origin"]           = &graphics::Transform::set_origin;
		// transform_type["set_pos"]              = &graphics::Transform::set_pos;
		// transform_type["set_rotation"]         = &graphics::Transform::set_rotation;
		// transform_type["set_scale_horizontal"] = &graphics::Transform::set_scale_horizontal;
		// transform_type["set_scale_vertical"]   = &graphics::Transform::set_scale_vertical;
		// transform_type["translate"]            = &graphics::Transform::translate;

		//// clang-format off
		// lua.new_enum<graphics::Text::Alignment>("TextAlignment",
		//{
		//	{"CENTER", graphics::Text::Alignment::CENTER},
		//	{"LEFT", graphics::Text::Alignment::LEFT},
		//	{"RIGHT", graphics::Text::Alignment::RIGHT}
		// });
		//// clang-format on

		// auto font_type                = lua.new_usertype<graphics::Font>("Font", sol::constructors<graphics::Font()>());
		// font_type["build"]            = &graphics::Font::build;
		// font_type["get_text_size"]    = &graphics::Font::get_text_size;
		// font_type["load_file"]        = sol::resolve<bool(const std::string&)>(&graphics::Font::load);
		// font_type["load_mem"]         = sol::resolve<bool(unsigned char*, const unsigned int)>(&graphics::Font::load);
		// font_type["vertical_advance"] = &graphics::Font::vertical_advance;

		// auto fc_type       = lua.new_usertype<graphics::FontContext>("Font", sol::no_constructor);
		// fc_type["set_dpi"] = &graphics::FontContext::set_dpi;

		// auto text_type                = lua.new_usertype<graphics::Text>("Text", sol::constructors<graphics::Font()>());
		// text_type["create"]           = &graphics::Text::create;
		// text_type["get_alignment"]    = &graphics::Text::get_alignment;
		// text_type["get_font"]         = &graphics::Text::get_font;
		// text_type["get_size"]         = &graphics::Text::get_size;
		// text_type["get_text"]         = &graphics::Text::get_text;
		// text_type["height"]           = &graphics::Text::height;
		// text_type["colour"]           = &graphics::Text::m_colour;
		// text_type["update"]           = sol::resolve<void(std::string_view)>(&graphics::Text::update);
		// text_type["update_size"]      = sol::resolve<void(const float)>(&graphics::Text::update);
		// text_type["update_alignment"] = sol::resolve<void(const graphics::Text::Alignment)>(&graphics::Text::update);
		// text_type["set_font"]         = &graphics::Text::set_font;
		// text_type["width"]            = &graphics::Text::width;

		// auto circle_type         = lua.new_usertype<graphics::Circle>("Circle", sol::constructors<graphics::Circle()>());
		// circle_type["create"]    = &graphics::Circle::create;
		// circle_type["fragments"] = &graphics::Circle::fragments;
		// circle_type["height"]    = &graphics::Circle::height;
		// circle_type["mode"]      = &graphics::Circle::mode;
		// circle_type["colour"]    = &graphics::Circle::m_colour;
		// circle_type["radius"]    = &graphics::Circle::radius;
		// circle_type["width"]     = &graphics::Circle::width;

		// auto ellipse_type         = lua.new_usertype<graphics::Ellipse>("Ellipse", sol::constructors<graphics::Ellipse()>());
		// ellipse_type["create"]    = &graphics::Ellipse::create;
		// ellipse_type["fragments"] = &graphics::Ellipse::fragments;
		// ellipse_type["height"]    = &graphics::Ellipse::height;
		// ellipse_type["mode"]      = &graphics::Ellipse::mode;
		// ellipse_type["colour"]    = &graphics::Ellipse::m_colour;
		// ellipse_type["radii"]     = &graphics::Ellipse::radii;
		// ellipse_type["width"]     = &graphics::Ellipse::width;

		// auto point_type      = lua.new_usertype<graphics::Point>("Point", sol::constructors<graphics::Point()>());
		// point_type["create"] = &graphics::Point::create;
		// point_type["height"] = &graphics::Point::height;
		// point_type["mode"]   = &graphics::Point::mode;
		// point_type["colour"] = &graphics::Point::m_colour;
		// point_type["pos"]    = &graphics::Point::pos;
		// point_type["width"]  = &graphics::Point::width;

		// auto polygon_type      = lua.new_usertype<graphics::Polygon>("Polygon", sol::constructors<graphics::Polygon()>());
		// polygon_type["create"] = &graphics::Polygon::create;
		// polygon_type["height"] = &graphics::Polygon::height;
		// polygon_type["mode"]   = &graphics::Polygon::mode;
		// polygon_type["colour"] = &graphics::Polygon::m_colour;
		// polygon_type["points"] = &graphics::Polygon::points;
		// polygon_type["width"]  = &graphics::Polygon::width;

		// auto polyline_type      = lua.new_usertype<graphics::Polyline>("Polyline", sol::constructors<graphics::Polyline()>());
		// polyline_type["create"] = &graphics::Polyline::create;
		// polyline_type["height"] = &graphics::Polyline::height;
		// polyline_type["mode"]   = &graphics::Polyline::mode;
		// polyline_type["colour"] = &graphics::Polyline::m_colour;
		// polyline_type["points"] = &graphics::Polyline::points;
		// polyline_type["width"]  = &graphics::Polyline::width;
	}
} // namespace galaxy
