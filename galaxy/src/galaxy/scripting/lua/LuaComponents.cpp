///
/// LuaComponents.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt_sol/registry.hpp>

#include "galaxy/components/Circle.hpp"
#include "galaxy/components/Ellipse.hpp"
#include "galaxy/components/GUI.hpp"
#include "galaxy/components/Point.hpp"
#include "galaxy/components/Polygon.hpp"
#include "galaxy/components/Polyline.hpp"
#include "galaxy/components/RenderCommand.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Script.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/core/ServiceLocator.hpp"

#include "Lua.hpp"

namespace galaxy
{
	namespace lua
	{
		void inject_components()
		{
			auto& lua = core::ServiceLocator<sol::state>::ref();

			auto circle_type     = lua.new_usertype<components::Circle>("Circle",
                sol::constructors<components::Circle()>(),
                "type_id",
                &entt::type_hash<components::Circle>::value);
			circle_type["shape"] = &components::Circle::m_shape;

			auto ellipse_type     = lua.new_usertype<components::Ellipse>("Ellipse",
                sol::constructors<components::Ellipse()>(),
                "type_id",
                &entt::type_hash<components::Ellipse>::value);
			ellipse_type["shape"] = &components::Ellipse::m_shape;

			auto gui_type =
				lua.new_usertype<components::GUI>("GUI", sol::constructors<components::GUI()>(), "type_id", &entt::type_hash<components::GUI>::value);
			gui_type["file"]   = &components::GUI::file;
			gui_type["self"]   = &components::GUI::m_self;
			gui_type["update"] = &components::GUI::m_update;

			auto point_type =
				lua.new_usertype<components::Point>("Point", sol::constructors<components::Point()>(), "type_id", &entt::type_hash<components::Point>::value);
			point_type["shape"] = &components::Point::m_shape;

			auto polygon_type     = lua.new_usertype<components::Polygon>("Polygon",
                sol::constructors<components::Polygon()>(),
                "type_id",
                &entt::type_hash<components::Polygon>::value);
			polygon_type["shape"] = &components::Polygon::m_shape;

			auto polyline_type     = lua.new_usertype<components::Polyline>("Polyline",
                sol::constructors<components::Polyline()>(),
                "type_id",
                &entt::type_hash<components::Polyline>::value);
			polyline_type["shape"] = &components::Polyline::m_shape;

			auto rendercommand_type       = lua.new_usertype<components::RenderCommand>("RenderCommand",
                sol::constructors<components::RenderCommand()>(),
                "type_id",
                &entt::type_hash<components::RenderCommand>::value);
			rendercommand_type["command"] = &components::RenderCommand::m_command;

			auto rigidbody_type                         = lua.new_usertype<components::RigidBody>("RigidBody",
                sol::constructors<components::RigidBody()>(),
                "type_id",
                &entt::type_hash<components::RigidBody>::value);
			rigidbody_type["get_density"]               = &components::RigidBody::get_density;
			rigidbody_type["get_friction"]              = &components::RigidBody::get_friction;
			rigidbody_type["get_restitution"]           = &components::RigidBody::get_restitution;
			rigidbody_type["get_restitution_threshold"] = &components::RigidBody::get_restitution_threshold;
			rigidbody_type["get_shape"]                 = &components::RigidBody::get_shape;
			rigidbody_type["get_type"]                  = &components::RigidBody::get_type;
			rigidbody_type["is_bullet"]                 = &components::RigidBody::is_bullet;
			rigidbody_type["is_rotation_fixed"]         = &components::RigidBody::is_rotation_fixed;
			rigidbody_type["set_bullet"]                = &components::RigidBody::set_bullet;
			rigidbody_type["set_density"]               = &components::RigidBody::set_density;
			rigidbody_type["set_fixed_rotation"]        = &components::RigidBody::set_fixed_rotation;
			rigidbody_type["set_friction"]              = &components::RigidBody::set_friction;
			rigidbody_type["set_restitution"]           = &components::RigidBody::set_restitution;
			rigidbody_type["set_restitution_threshold"] = &components::RigidBody::set_restitution_threshold;
			rigidbody_type["set_shape"]                 = &components::RigidBody::set_shape;
			rigidbody_type["set_type"]                  = &components::RigidBody::set_type;

			auto script_type      = lua.new_usertype<components::Script>("Script",
                sol::constructors<components::Script()>(),
                "type_id",
                &entt::type_hash<components::Script>::value);
			script_type["file"]   = &components::Script::file;
			script_type["self"]   = &components::Script::m_self;
			script_type["update"] = &components::Script::m_update;

			auto sprite_type                     = lua.new_usertype<components::Sprite>("Sprite",
                sol::constructors<components::Sprite()>(),
                "type_id",
                &entt::type_hash<components::Sprite>::value);
			sprite_type["tint"]                  = &components::Sprite::m_tint;
			sprite_type["set_texture"]           = sol::resolve<void(const std::string&)>(&components::Sprite::set_texture);
			sprite_type["set_texture_with_rect"] = sol::resolve<void(const std::string&, const math::iRect&)>(&components::Sprite::set_texture);

			auto tag_type =
				lua.new_usertype<components::Tag>("Tag", sol::constructors<components::Tag()>(), "type_id", &entt::type_hash<components::Tag>::value);
			tag_type["tag"] = &components::Tag::m_tag;

			auto text_type =
				lua.new_usertype<components::Text>("Text", sol::constructors<components::Text()>(), "type_id", &entt::type_hash<components::Text>::value);
			text_type["text"] = &components::Text::m_text;

			auto transform_type  = lua.new_usertype<components::Transform>("Transform",
                sol::constructors<components::Transform()>(),
                "type_id",
                &entt::type_hash<components::Transform>::value);
			transform_type["tf"] = &components::Transform::m_tf;

			entt_sol::register_meta_component<components::Circle>();
			entt_sol::register_meta_component<components::Ellipse>();
			entt_sol::register_meta_component<components::GUI>();
			entt_sol::register_meta_component<components::Point>();
			entt_sol::register_meta_component<components::Polygon>();
			entt_sol::register_meta_component<components::Polyline>();
			entt_sol::register_meta_component<components::RenderCommand>();
			entt_sol::register_meta_component<components::RigidBody>();
			entt_sol::register_meta_component<components::Script>();
			entt_sol::register_meta_component<components::Sprite>();
			entt_sol::register_meta_component<components::Tag>();
			entt_sol::register_meta_component<components::Text>();
			entt_sol::register_meta_component<components::Transform>();
		}
	} // namespace lua
} // namespace galaxy
