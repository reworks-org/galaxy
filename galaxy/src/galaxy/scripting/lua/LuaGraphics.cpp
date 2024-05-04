///
/// LuaGraphics.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/graphics/anim/Animation.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/shapes/Circle.hpp"
#include "galaxy/graphics/shapes/Ellipse.hpp"
#include "galaxy/graphics/shapes/Point.hpp"
#include "galaxy/graphics/shapes/Polygon.hpp"
#include "galaxy/graphics/shapes/Polyline.hpp"
#include "galaxy/graphics/text/Text.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_graphics()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto animation_type       = lua.new_usertype<graphics::Animation>("Animation", sol::constructors<graphics::Animation()>());
			animation_type["current"] = &graphics::Animation::current;
			animation_type["frames"]  = &graphics::Animation::frames;
			animation_type["index"]   = &graphics::Animation::index;
			animation_type["load"]    = &graphics::Animation::load;
			animation_type["name"]    = &graphics::Animation::m_name;
			animation_type["speed"]   = &graphics::Animation::m_speed;
			animation_type["next"]    = &graphics::Animation::next;
			animation_type["prev"]    = &graphics::Animation::prev;
			animation_type["restart"] = &graphics::Animation::restart;
			// animation_type["set_frames"] = &graphics::Animation::set_frames;
			animation_type["total"] = &graphics::Animation::total;

			auto frame_type        = lua.new_usertype<graphics::Frame>("Frame", sol::constructors<graphics::Frame()>());
			frame_type["bounds"]   = &graphics::Frame::m_bounds;
			frame_type["duration"] = &graphics::Frame::m_duration;

			auto cameradata_type          = lua.new_usertype<graphics::Camera::Data>("CameraData", sol::no_constructor);
			cameradata_type["model_view"] = &graphics::Camera::Data::m_model_view;
			cameradata_type["projection"] = &graphics::Camera::Data::m_projection;

			auto camera_type                    = lua.new_usertype<graphics::Camera>("Camera", sol::constructors<graphics::Camera()>());
			camera_type["get_data"]             = &graphics::Camera::get_data;
			camera_type["get_model_view"]       = &graphics::Camera::get_model_view;
			camera_type["get_origin"]           = &graphics::Camera::get_origin;
			camera_type["get_pos"]              = &graphics::Camera::get_pos;
			camera_type["get_proj"]             = &graphics::Camera::get_proj;
			camera_type["get_rotation"]         = &graphics::Camera::get_rotation;
			camera_type["get_scale"]            = &graphics::Camera::get_scale;
			camera_type["get_transform"]        = &graphics::Camera::get_transform;
			camera_type["get_viewport"]         = &graphics::Camera::get_viewport;
			camera_type["rotation_speed"]       = &graphics::Camera::m_rotation_speed;
			camera_type["translation_speed"]    = &graphics::Camera::m_translation_speed;
			camera_type["allow_rotation"]       = &graphics::Camera::m_allow_rotation;
			camera_type["reset"]                = &graphics::Camera::reset;
			camera_type["rotate"]               = &graphics::Camera::rotate;
			camera_type["scale"]                = &graphics::Camera::scale;
			camera_type["set_viewport"]         = &graphics::Camera::set_viewport;
			camera_type["set_pos"]              = &graphics::Camera::set_pos;
			camera_type["set_rotation"]         = &graphics::Camera::set_rotation;
			camera_type["set_origin"]           = &graphics::Camera::set_origin;
			camera_type["set_scale_horizontal"] = &graphics::Camera::set_scale_horizontal;
			camera_type["set_scale_vertical"]   = &graphics::Camera::set_scale_vertical;
			camera_type["translate"]            = &graphics::Camera::translate;

			lua.set("galaxy_colour_opaque", graphics::Colour::OPAQUE);
			lua.set("galaxy_colour_transparent", graphics::Colour::TRANSPARENT);

			auto colour_type           = lua.new_usertype<graphics::Colour>("Colour", sol::constructors<graphics::Colour()>());
			colour_type["get_red"]     = &graphics::Colour::r<std::uint8_t>;
			colour_type["get_green"]   = &graphics::Colour::g<std::uint8_t>;
			colour_type["get_blue"]    = &graphics::Colour::b<std::uint8_t>;
			colour_type["get_alpha"]   = &graphics::Colour::a<std::uint8_t>;
			colour_type["get_red_f"]   = &graphics::Colour::r<float>;
			colour_type["get_green_f"] = &graphics::Colour::g<float>;
			colour_type["get_blue_f"]  = &graphics::Colour::b<float>;
			colour_type["get_alpha_f"] = &graphics::Colour::a<float>;
			colour_type["set_r"]       = &graphics::Colour::set_r;
			colour_type["set_g"]       = &graphics::Colour::set_g;
			colour_type["set_b"]       = &graphics::Colour::set_b;
			colour_type["set_a"]       = &graphics::Colour::set_a;
			colour_type["set_rgba"]    = &graphics::Colour::set_rgba;

			auto rendercommand_type         = lua.new_usertype<graphics::RenderCommand>("RenderCommand", sol::constructors<graphics::RenderCommand()>());
			rendercommand_type["count"]     = &graphics::RenderCommand::count;
			rendercommand_type["instances"] = &graphics::RenderCommand::instances;
			rendercommand_type["layer"]     = &graphics::RenderCommand::layer;
			rendercommand_type["mode"]      = &graphics::RenderCommand::mode;
			rendercommand_type["offset"]    = &graphics::RenderCommand::offset;
			rendercommand_type["uniforms"]  = &graphics::RenderCommand::uniforms;
			rendercommand_type["vao"]       = &graphics::RenderCommand::vao;

			auto renderdata_type         = lua.new_usertype<graphics::RenderData>("RenderData", sol::constructors<graphics::RenderData()>());
			renderdata_type["colour"]    = &graphics::RenderData::colour;
			renderdata_type["entity"]    = &graphics::RenderData::entity;
			renderdata_type["handle"]    = &graphics::RenderData::handle;
			renderdata_type["point"]     = &graphics::RenderData::point;
			renderdata_type["textured"]  = &graphics::RenderData::textured;
			renderdata_type["transform"] = &graphics::RenderData::transform;

			auto transform_type                    = lua.new_usertype<graphics::Transform>("Transform", sol::constructors<graphics::Transform()>());
			transform_type["get_origin"]           = &graphics::Transform::get_origin;
			transform_type["get_pos"]              = &graphics::Transform::get_pos;
			transform_type["get_rotation"]         = &graphics::Transform::get_rotation;
			transform_type["get_scale"]            = &graphics::Transform::get_scale;
			transform_type["get_transform"]        = &graphics::Transform::get_transform;
			transform_type["reset"]                = &graphics::Transform::reset;
			transform_type["rotate"]               = &graphics::Transform::rotate;
			transform_type["scale"]                = &graphics::Transform::scale;
			transform_type["set_origin"]           = &graphics::Transform::set_origin;
			transform_type["set_pos"]              = &graphics::Transform::set_pos;
			transform_type["set_rotation"]         = &graphics::Transform::set_rotation;
			transform_type["set_scale_horizontal"] = &graphics::Transform::set_scale_horizontal;
			transform_type["set_scale_vertical"]   = &graphics::Transform::set_scale_vertical;
			transform_type["translate"]            = &graphics::Transform::translate;

			// clang-format off
			lua.new_enum<graphics::Text::Alignment>("TextAlignment",
			{
				{"CENTER", graphics::Text::Alignment::CENTER},
				{"LEFT", graphics::Text::Alignment::LEFT},
				{"RIGHT", graphics::Text::Alignment::RIGHT}
			});
			// clang-format on

			auto font_type                = lua.new_usertype<graphics::Font>("Font", sol::constructors<graphics::Font()>());
			font_type["build"]            = &graphics::Font::build;
			font_type["get_text_size"]    = &graphics::Font::get_text_size;
			font_type["load_file"]        = sol::resolve<bool(const std::string&)>(&graphics::Font::load);
			font_type["load_mem"]         = sol::resolve<bool(unsigned char*, const unsigned int)>(&graphics::Font::load);
			font_type["vertical_advance"] = &graphics::Font::vertical_advance;

			auto fc_type       = lua.new_usertype<graphics::FontContext>("Font", sol::no_constructor);
			fc_type["set_dpi"] = &graphics::FontContext::set_dpi;

			auto text_type                = lua.new_usertype<graphics::Text>("Text", sol::constructors<graphics::Font()>());
			text_type["create"]           = &graphics::Text::create;
			text_type["get_alignment"]    = &graphics::Text::get_alignment;
			text_type["get_font"]         = &graphics::Text::get_font;
			text_type["get_size"]         = &graphics::Text::get_size;
			text_type["get_text"]         = &graphics::Text::get_text;
			text_type["height"]           = &graphics::Text::height;
			text_type["colour"]           = &graphics::Text::m_colour;
			text_type["update"]           = sol::resolve<void(std::string_view)>(&graphics::Text::update);
			text_type["update_size"]      = sol::resolve<void(const float)>(&graphics::Text::update);
			text_type["update_alignment"] = sol::resolve<void(const graphics::Text::Alignment)>(&graphics::Text::update);
			text_type["set_font"]         = &graphics::Text::set_font;
			text_type["width"]            = &graphics::Text::width;

			auto circle_type         = lua.new_usertype<graphics::Circle>("Circle", sol::constructors<graphics::Circle()>());
			circle_type["create"]    = &graphics::Circle::create;
			circle_type["fragments"] = &graphics::Circle::fragments;
			circle_type["height"]    = &graphics::Circle::height;
			circle_type["mode"]      = &graphics::Circle::mode;
			circle_type["colour"]    = &graphics::Circle::m_colour;
			circle_type["radius"]    = &graphics::Circle::radius;
			circle_type["width"]     = &graphics::Circle::width;

			auto ellipse_type         = lua.new_usertype<graphics::Ellipse>("Ellipse", sol::constructors<graphics::Ellipse()>());
			ellipse_type["create"]    = &graphics::Ellipse::create;
			ellipse_type["fragments"] = &graphics::Ellipse::fragments;
			ellipse_type["height"]    = &graphics::Ellipse::height;
			ellipse_type["mode"]      = &graphics::Ellipse::mode;
			ellipse_type["colour"]    = &graphics::Ellipse::m_colour;
			ellipse_type["radii"]     = &graphics::Ellipse::radii;
			ellipse_type["width"]     = &graphics::Ellipse::width;

			auto point_type      = lua.new_usertype<graphics::Point>("Point", sol::constructors<graphics::Point()>());
			point_type["create"] = &graphics::Point::create;
			point_type["height"] = &graphics::Point::height;
			point_type["mode"]   = &graphics::Point::mode;
			point_type["colour"] = &graphics::Point::m_colour;
			point_type["pos"]    = &graphics::Point::pos;
			point_type["width"]  = &graphics::Point::width;

			auto polygon_type      = lua.new_usertype<graphics::Polygon>("Polygon", sol::constructors<graphics::Polygon()>());
			polygon_type["create"] = &graphics::Polygon::create;
			polygon_type["height"] = &graphics::Polygon::height;
			polygon_type["mode"]   = &graphics::Polygon::mode;
			polygon_type["colour"] = &graphics::Polygon::m_colour;
			polygon_type["points"] = &graphics::Polygon::points;
			polygon_type["width"]  = &graphics::Polygon::width;

			auto polyline_type      = lua.new_usertype<graphics::Polyline>("Polyline", sol::constructors<graphics::Polyline()>());
			polyline_type["create"] = &graphics::Polyline::create;
			polyline_type["height"] = &graphics::Polyline::height;
			polyline_type["mode"]   = &graphics::Polyline::mode;
			polyline_type["colour"] = &graphics::Polyline::m_colour;
			polyline_type["points"] = &graphics::Polyline::points;
			polyline_type["width"]  = &graphics::Polyline::width;
		}
	} // namespace lua
} // namespace galaxy
