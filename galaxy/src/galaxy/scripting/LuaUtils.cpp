///
/// LuaUtils.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <sol/sol.hpp>

#include "galaxy/core/GalaxyConfig.hpp"
#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/core/World.hpp"
#include "galaxy/components/Actions.hpp"
#include "galaxy/components/Animated.hpp"
#include "galaxy/components/BatchSprite.hpp"
#include "galaxy/components/OnCollision.hpp"
#include "galaxy/components/ParticleEffect.hpp"
#include "galaxy/components/Primitive2D.hpp"
#include "galaxy/components/Renderable.hpp"
#include "galaxy/components/RigidBody.hpp"
#include "galaxy/components/Sprite.hpp"
#include "galaxy/components/Tag.hpp"
#include "galaxy/components/Text.hpp"
#include "galaxy/components/Transform2D.hpp"

#include "galaxy/error/Log.hpp"
#include "galaxy/input/Keys.hpp"
#include "galaxy/input/MouseButtons.hpp"

#include "LuaUtils.hpp"

namespace galaxy
{
	void log_wrapper(error::Level error_level, std::string_view message)
	{
		switch (error_level)
		{
			case error::Level::INFO:
				GALAXY_LOG(GALAXY_INFO, "{0}", message);
				break;

			case error::Level::DEBUG:
				GALAXY_LOG(GALAXY_DEBUG, "{0}", message);
				break;

			case error::Level::WARNING:
				GALAXY_LOG(GALAXY_WARNING, "{0}", message);
				break;

			case error::Level::ERROR_:
				GALAXY_LOG(GALAXY_ERROR, "{0}", message);
				break;

			case error::Level::FATAL:
				GALAXY_LOG(GALAXY_FATAL, "{0}", message);
				break;
		}
	}

	namespace scripting
	{
		void register_lua()
		{
			auto lua = SL_HANDLE.lua();

			//
			// Galaxy Config vars.
			// ---------------------------------------------------------------------

			lua->set("GALAXY_DT", GALAXY_DT);

			//
			// Error handling.
			// ---------------------------------------------------------------------

			// clang-format off
			lua->new_enum<error::Level>("gLogLevels",
			{
				{"INFO", error::Level::INFO},
				{"DEBUG", error::Level::DEBUG},
				{"WARNING", error::Level::WARNING},
				{"ERROR", error::Level::ERROR_},
				{"FATAL", error::Level::FATAL}
			});
			// clang-format on

			lua->set_function("galaxy_log", &log_wrapper);

			//
			// Input handling.
			// ---------------------------------------------------------------------

			// clang-format off
			lua->new_enum<input::Keys>("gKeys",
			{
				{"A", input::Keys::A},
				{"B", input::Keys::B},
				{"C", input::Keys::C},
				{"D", input::Keys::D},
				{"E", input::Keys::E},
				{"F", input::Keys::F},
				{"G", input::Keys::G},
				{"H", input::Keys::H},
				{"I", input::Keys::I},
				{"J", input::Keys::J},
				{"K", input::Keys::K},
				{"L", input::Keys::L},
				{"M", input::Keys::M},
				{"N", input::Keys::N},
				{"O", input::Keys::O},
				{"P", input::Keys::P},
				{"Q", input::Keys::Q},
				{"R", input::Keys::R},
				{"S", input::Keys::S},
				{"T", input::Keys::T},
				{"U", input::Keys::U},
				{"V", input::Keys::V},
				{"W", input::Keys::W},
				{"X", input::Keys::X},
				{"Y", input::Keys::Y},
				{"Z", input::Keys::Z},
				{"KEY_1", input::Keys::KEY_1},
				{"KEY_2", input::Keys::KEY_2},
				{"KEY_3", input::Keys::KEY_3},
				{"KEY_4", input::Keys::KEY_4},
				{"KEY_5", input::Keys::KEY_5},
				{"KEY_6", input::Keys::KEY_6},
				{"KEY_7", input::Keys::KEY_7},
				{"KEY_8", input::Keys::KEY_8},
				{"KEY_9", input::Keys::KEY_9},
				{"KEY_0", input::Keys::KEY_0},
				{"MINUS", input::Keys::S},
				{"EQUALS", input::Keys::S},
				{"BACKSPACE", input::Keys::E},
				{"GRAVE", input::Keys::E},
				{"TAB", input::Keys::B},
				{"CAPS", input::Keys::S},
				{"LSHIFT", input::Keys::T},
				{"LCNTRL", input::Keys::L},
				{"LSTART", input::Keys::T},
				{"LALT", input::Keys::T},
				{"SPACE", input::Keys::E},
				{"RALT", input::Keys::T},
				{"RSTART", input::Keys::T},
				{"MENU", input::Keys::U},
				{"RCNTRL", input::Keys::L},
				{"RSHIFT", input::Keys::T},
				{"ENTER", input::Keys::R},
				{"SEMICOLON", input::Keys::N},
				{"APOSTROPHE", input::Keys::E},
				{"SLASH", input::Keys::H},
				{"PERIOD", input::Keys::D},
				{"COMMA", input::Keys::A},
				{"LBRACKET", input::Keys::T},
				{"RBRACKET", input::Keys::T},
				{"BACKSLASH", input::Keys::H},
				{"ESC", input::Keys::C},
				{"F1", input::Keys::F1},
				{"F2", input::Keys::F2},
				{"F3", input::Keys::F3},
				{"F4", input::Keys::F4},
				{"F5", input::Keys::F5},
				{"F6", input::Keys::F6},
				{"F7", input::Keys::F7},
				{"F8", input::Keys::F8},
				{"F9", input::Keys::F9},
				{"F10", input::Keys::F10},
				{"F11", input::Keys::F1},
				{"F12", input::Keys::F2},
				{"PRINTSCREEN", input::Keys::N},
				{"SCROLL_LOCK", input::Keys::K},
				{"PAUSE", input::Keys::E},
				{"INSERT", input::Keys::T},
				{"HOME", input::Keys::E},
				{"PAGEUP", input::Keys::P},
				{"PAGEDOWN", input::Keys::N},
				{"END", input::Keys::D},
				{"DEL", input::Keys::L},
				{"UP", input::Keys::P},
				{"DOWN", input::Keys::N},
				{"LEFT", input::Keys::T},
				{"RIGHT", input::Keys::T},
				{"NUMLOCK", input::Keys::K},
				{"NUMPAD_MULTIPLY", input::Keys::Y},
				{"NUMPAD_DIVIDE", input::Keys::E},
				{"NUMPAD_ADD", input::Keys::D},
				{"NUMPAD_ENTER", input::Keys::R},
				{"NUMPAD_PERIOD", input::Keys::D},
				{"NUMPAD_0", input::Keys::NUMPAD_0},
				{"NUMPAD_1", input::Keys::NUMPAD_1},
				{"NUMPAD_2", input::Keys::NUMPAD_2},
				{"NUMPAD_3", input::Keys::NUMPAD_3},
				{"NUMPAD_4", input::Keys::NUMPAD_4},
				{"NUMPAD_5", input::Keys::NUMPAD_5},
				{"NUMPAD_6", input::Keys::NUMPAD_6},
				{"NUMPAD_7", input::Keys::NUMPAD_7},
				{"NUMPAD_8", input::Keys::NUMPAD_8},
				{"NUMPAD_9",input::Keys::NUMPAD_9}
			});

			lua->new_enum<input::MouseButtons>("gMouseButtons",
			{
				{"BUTTON_1", input::MouseButtons::BUTTON_1},
				{"BUTTON_2", input::MouseButtons::BUTTON_2},
				{"BUTTON_3", input::MouseButtons::BUTTON_3},
				{"BUTTON_4", input::MouseButtons::BUTTON_4},
				{"BUTTON_5", input::MouseButtons::BUTTON_5},
				{"BUTTON_6", input::MouseButtons::BUTTON_6},
				{"BUTTON_7", input::MouseButtons::BUTTON_7},
				{"BUTTON_8", input::MouseButtons::BUTTON_8},
				{"BUTTON_LAST", input::MouseButtons::BUTTON_LAST},
				{"BUTTON_LEFT", input::MouseButtons::BUTTON_LEFT},
				{"BUTTON_RIGHT", input::MouseButtons::BUTTON_RIGHT},
				{"BUTTON_MIDDLE", input::MouseButtons::BUTTON_MIDDLE}
			});
			// clang-format on

			//
			// Data structures.
			// ---------------------------------------------------------------------

			// clang-format off
			lua->new_enum<graphics::Renderables>("gRenderables",
			{
				{"BATCHED", graphics::Renderables::BATCHED},
				{"LINE", graphics::Renderables::LINE},
				{"LINE_LOOP", graphics::Renderables::LINE_LOOP},
				{"PARTICLE", graphics::Renderables::PARTICLE},
				{"POINT", graphics::Renderables::POINT},
				{"SPRITE", graphics::Renderables::SPRITE},
				{"TEXT", graphics::Renderables::TEXT}
			});

			lua->new_enum<physics::BodyType>("gRigidBodyType",
			{
				{"DYNAMIC", physics::BodyType::DYNAMIC},
				{"STATIC",  physics::BodyType::STATIC}
			});
			// clang-format on

			auto vec2_type = lua->new_usertype<glm::vec2>("gVec2f", sol::constructors<glm::vec2()>());
			vec2_type["x"] = &glm::vec2::x;
			vec2_type["y"] = &glm::vec2::y;

			auto vec3_type = lua->new_usertype<glm::vec3>("gVec3f", sol::constructors<glm::vec3()>());
			vec3_type["x"] = &glm::vec3::x;
			vec3_type["y"] = &glm::vec3::y;
			vec3_type["z"] = &glm::vec3::z;

			auto vec4_type = lua->new_usertype<glm::vec4>("gVec4f", sol::constructors<glm::vec4()>());
			vec4_type["x"] = &glm::vec4::x;
			vec4_type["y"] = &glm::vec4::y;
			vec4_type["z"] = &glm::vec4::z;
			vec4_type["w"] = &glm::vec4::w;

			auto frect_type              = lua->new_usertype<math::Rect<float>>("gRect_f", sol::constructors<math::Rect<float>()>());
			frect_type["contains_point"] = sol::resolve<const bool(const float, const float)>(&math::Rect<float>::contains);
			frect_type["contains_rect"]  = sol::resolve<const bool(const math::Rect<float>&)>(&math::Rect<float>::contains);
			frect_type["height"]         = &math::Rect<float>::m_height;
			frect_type["width"]          = &math::Rect<float>::m_width;
			frect_type["y"]              = &math::Rect<float>::m_x;
			frect_type["x"]              = &math::Rect<float>::m_y;
			frect_type["overlaps"]       = &math::Rect<float>::overlaps;

			auto irect_type              = lua->new_usertype<math::Rect<int>>("gRect_i", sol::constructors<math::Rect<int>()>());
			irect_type["contains_point"] = sol::resolve<const bool(const int, const int)>(&math::Rect<int>::contains);
			irect_type["contains_rect"]  = sol::resolve<const bool(const math::Rect<int>&)>(&math::Rect<int>::contains);
			irect_type["height"]         = &math::Rect<int>::m_height;
			irect_type["width"]          = &math::Rect<int>::m_width;
			irect_type["y"]              = &math::Rect<int>::m_x;
			irect_type["x"]              = &math::Rect<int>::m_y;
			irect_type["overlaps"]       = &math::Rect<int>::overlaps;

			auto colour_type          = lua->new_usertype<graphics::Colour>("gColour", sol::constructors<graphics::Colour()>());
			colour_type["a_normal"]   = &graphics::Colour::a_normal;
			colour_type["b_normal"]   = &graphics::Colour::b_normal;
			colour_type["g_normal"]   = &graphics::Colour::g_normal;
			colour_type["alpha"]      = &graphics::Colour::m_alpha;
			colour_type["blue"]       = &graphics::Colour::m_blue;
			colour_type["green"]      = &graphics::Colour::m_green;
			colour_type["red"]        = &graphics::Colour::m_red;
			colour_type["normalized"] = &graphics::Colour::normalized;
			colour_type["r_normal"]   = &graphics::Colour::r_normal;

			auto aabb_type            = lua->new_usertype<math::AABB>("gAABB", sol::constructors<math::AABB()>());
			aabb_type["area"]         = &math::AABB::area;
			aabb_type["compute_area"] = &math::AABB::compute_area;
			aabb_type["contains"]     = &math::AABB::contains;
			aabb_type["fatten"]       = &math::AABB::fatten;
			aabb_type["max"]          = &math::AABB::max;
			aabb_type["min"]          = &math::AABB::min;
			aabb_type["overlaps"]     = &math::AABB::overlaps;
			aabb_type["size"]         = &math::AABB::size;
			aabb_type["update_area"]  = &math::AABB::update_area;

			//
			// Components.
			// ---------------------------------------------------------------------

			auto actions_type                = lua->new_usertype<components::Actions>("gActionsComponent", sol::no_constructor);
			actions_type["set_key_action"]   = &components::Actions::add_key_action;
			actions_type["set_mouse_action"] = &components::Actions::add_mouse_action;

			auto animated_type             = lua->new_usertype<components::Animated>("gAnimatedComponent", sol::no_constructor);
			animated_type["is_paused"]     = &components::Animated::is_paused;
			animated_type["pause"]         = &components::Animated::pause;
			animated_type["play"]          = sol::resolve<void(void)>(&components::Animated::play);
			animated_type["play_specific"] = sol::resolve<void(std::string_view)>(&components::Animated::play);
			animated_type["set_animation"] = &components::Animated::set_animation;
			animated_type["stop"]          = &components::Animated::stop;

			auto batchsprite_type           = lua->new_usertype<components::BatchSprite>("gBatchSpriteComponent", sol::no_constructor);
			batchsprite_type["clip_height"] = &components::BatchSprite::clip_height;
			batchsprite_type["clip_width"]  = &components::BatchSprite::clip_width;
			batchsprite_type["create_from_rect"] =
				sol::resolve<void(const math::Rect<float>&, std::string_view, std::size_t)>(&components::BatchSprite::create);
			batchsprite_type["create_from_atlas"] = sol::resolve<void(std::string_view, std::string_view)>(&components::BatchSprite::create);
			batchsprite_type["get_atlas_index"]   = &components::BatchSprite::get_atlas_index;
			batchsprite_type["get_clip"]          = &components::BatchSprite::get_clip;
			batchsprite_type["get_key"]           = &components::BatchSprite::get_key;
			batchsprite_type["get_layer"]         = &components::BatchSprite::get_layer;
			batchsprite_type["get_opacity"]       = &components::BatchSprite::get_opacity;
			batchsprite_type["get_region"]        = &components::BatchSprite::get_region;
			batchsprite_type["set_layer"]         = &components::BatchSprite::set_layer;
			batchsprite_type["set_opacity"]       = &components::BatchSprite::set_opacity;
			batchsprite_type["update_region"]     = &components::BatchSprite::update_region;

			auto on_col_type      = lua->new_usertype<components::OnCollision>("gOnCollisionComponent", sol::no_constructor);
			on_col_type["script"] = &components::OnCollision::m_script;

			auto pe_type           = lua->new_usertype<components::ParticleEffect>("gParticleEffectComponent", sol::no_constructor);
			pe_type["buffer"]      = &components::ParticleEffect::buffer;
			pe_type["create"]      = &components::ParticleEffect::create;
			pe_type["get_opacity"] = &components::ParticleEffect::get_opacity;
			pe_type["is_loaded"]   = &components::ParticleEffect::is_loaded;
			pe_type["load"]        = &components::ParticleEffect::load;
			pe_type["regen"]       = &components::ParticleEffect::regen;
			pe_type["set_opacity"] = &components::ParticleEffect::set_opacity;

			auto p2d_data_type =
				lua->new_usertype<components::Primitive2D::PrimitiveData>("gPrimitive2DData", sol::constructors<components::Primitive2D::PrimitiveData()>());
			p2d_data_type["fragments"] = &components::Primitive2D::PrimitiveData::m_fragments;
			p2d_data_type["points"]    = &components::Primitive2D::PrimitiveData::m_points;
			p2d_data_type["radii"]     = &components::Primitive2D::PrimitiveData::m_radii;
			p2d_data_type["radius"]    = &components::Primitive2D::PrimitiveData::m_radius;
			p2d_data_type["start_end"] = &components::Primitive2D::PrimitiveData::m_start_end;

			auto p2d_type               = lua->new_usertype<components::Primitive2D>("gPrimitive2DComponent", sol::no_constructor);
			p2d_type["create_circle"]   = &components::Primitive2D::create<graphics::Primitives::CIRCLE>;
			p2d_type["create_ellipse"]  = &components::Primitive2D::create<graphics::Primitives::ELLIPSE>;
			p2d_type["create_line"]     = &components::Primitive2D::create<graphics::Primitives::LINE>;
			p2d_type["create_point"]    = &components::Primitive2D::create<graphics::Primitives::POINT>;
			p2d_type["create_polyline"] = &components::Primitive2D::create<graphics::Primitives::POLYLINE>;
			p2d_type["create_polygon"]  = &components::Primitive2D::create<graphics::Primitives::POLYGON>;
			p2d_type["set_colour"]      = &components::Primitive2D::set_colour;
			p2d_type["set_opacity"]     = &components::Primitive2D::set_opacity;

			auto renderable_type           = lua->new_usertype<components::Renderable>("gRenderableComponent", sol::no_constructor);
			renderable_type["get_aabb"]    = &components::Renderable::get_aabb;
			renderable_type["type"]        = &components::Renderable::m_type;
			renderable_type["update_aabb"] = &components::Renderable::update_aabb;

			auto rigid_body_type    = lua->new_usertype<components::RigidBody>("gRigidBodyComponent", sol::no_constructor);
			rigid_body_type["type"] = &components::RigidBody::m_type;

			auto sprite_type                 = lua->new_usertype<components::Sprite>("gSpriteComponent", sol::no_constructor);
			sprite_type["create"]            = &components::Sprite::create;
			sprite_type["create_clipped_xy"] = sol::resolve<void(std::string_view, const float, const float)>(&components::Sprite::create_clipped);
			sprite_type["create_clipped_rect"] =
				sol::resolve<void(std::string_view, const float, const float, const float, const float)>(&components::Sprite::create_clipped);
			sprite_type["is_loaded"]   = &components::Sprite::is_loaded;
			sprite_type["load"]        = &components::Sprite::load;
			sprite_type["save"]        = &components::Sprite::save;
			sprite_type["set_opacity"] = &components::Sprite::set_opacity;
			sprite_type["get_opacity"] = &components::Sprite::get_opacity;
			sprite_type["get_layer"]   = &components::Sprite::get_layer;
			sprite_type["get_width"]   = &components::Sprite::get_width;
			sprite_type["get_height"]  = &components::Sprite::get_height;

			auto tag_type   = lua->new_usertype<components::Tag>("gTagComponent", sol::no_constructor);
			tag_type["tag"] = &components::Tag::m_tag;

			auto text_type           = lua->new_usertype<components::Text>("gTextComponent", sol::no_constructor);
			text_type["create"]      = &components::Text::create;
			text_type["get_colour"]  = &components::Text::get_colour;
			text_type["get_height"]  = &components::Text::get_height;
			text_type["get_layer"]   = &components::Text::get_layer;
			text_type["get_width"]   = &components::Text::get_width;
			text_type["set_colour"]  = &components::Text::set_colour;
			text_type["set_opacity"] = &components::Text::set_opacity;
			text_type["update"]      = &components::Text::update;

			auto transform_type            = lua->new_usertype<components::Transform2D>("gTransformComponent", sol::no_constructor);
			transform_type["get_origin"]   = &components::Transform2D::get_origin;
			transform_type["get_pos"]      = &components::Transform2D::get_pos;
			transform_type["get_rotation"] = &components::Transform2D::get_rotation;
			transform_type["get_scale"]    = &components::Transform2D::get_scale;
			transform_type["is_dirty"]     = &components::Transform2D::is_dirty;
			transform_type["move"]         = &components::Transform2D::move;
			transform_type["recalculate"]  = &components::Transform2D::recalculate;
			transform_type["reset"]        = &components::Transform2D::reset;
			transform_type["rotate"]       = &components::Transform2D::rotate;
			transform_type["scale"]        = &components::Transform2D::scale;
			transform_type["set_origin"]   = &components::Transform2D::set_origin;
			transform_type["set_pos"]      = &components::Transform2D::set_pos;

			//
			// ECS.
			// ---------------------------------------------------------------------

			auto world_type                = lua->new_usertype<core::World>("gWorld", sol::no_constructor);
			world_type["enable"]           = &core::World::enable;
			world_type["clear"]            = &core::World::clear;
			world_type["assign_child"]     = &core::World::assign_child;
			world_type["clear"]            = &core::World::clear;
			world_type["create"]           = &core::World::create;
			world_type["create_from_json"] = &core::World::create_from_json;
			world_type["destroy"]          = &core::World::destroy;
			world_type["destroy_parent"]   = &core::World::destroy_parent;
			world_type["disable"]          = &core::World::disable;
			world_type["enable"]           = &core::World::enable;
			world_type["has"]              = &core::World::has;
			world_type["is_enabled"]       = &core::World::is_enabled;
			world_type["is_parent"]        = &core::World::is_parent;
			world_type["make_parent"]      = &core::World::make_parent;
			world_type["remove_child"]     = &core::World::remove_child;
			world_type["remove_parent"]    = &core::World::remove_parent;

			world_type["create_actions"]        = &core::World::create_component<components::Actions>;
			world_type["create_animated"]       = &core::World::create_component<components::Animated>;
			world_type["create_batchsprite"]    = &core::World::create_component<components::BatchSprite>;
			world_type["create_oncollision"]    = &core::World::create_component<components::OnCollision>;
			world_type["create_particleeffect"] = &core::World::create_component<components::ParticleEffect>;
			world_type["create_primitive"]      = &core::World::create_component<components::Primitive2D>;
			world_type["create_renderable"]     = &core::World::create_component<components::Renderable>;
			world_type["create_rigidbody"]      = &core::World::create_component<components::RigidBody>;
			world_type["create_sprite"]         = &core::World::create_component<components::Sprite>;
			world_type["create_tag"]            = &core::World::create_component<components::Tag>;
			world_type["create_text"]           = &core::World::create_component<components::Text>;
			world_type["create_transform"]      = &core::World::create_component<components::Transform2D>;

			world_type["remove_actions"]        = &core::World::remove<components::Actions>;
			world_type["remove_animated"]       = &core::World::remove<components::Animated>;
			world_type["remove_batchsprite"]    = &core::World::remove<components::BatchSprite>;
			world_type["remove_oncollision"]    = &core::World::remove<components::OnCollision>;
			world_type["remove_particleeffect"] = &core::World::remove<components::ParticleEffect>;
			world_type["remove_primitive"]      = &core::World::remove<components::Primitive2D>;
			world_type["remove_renderable"]     = &core::World::remove<components::Renderable>;
			world_type["remove_rigidbody"]      = &core::World::remove<components::RigidBody>;
			world_type["remove_sprite"]         = &core::World::remove<components::Sprite>;
			world_type["remove_tag"]            = &core::World::remove<components::Tag>;
			world_type["remove_text"]           = &core::World::remove<components::Text>;
			world_type["remove_transform"]      = &core::World::remove<components::Transform2D>;

			world_type["get_actions"]        = &core::World::get<components::Actions>;
			world_type["get_animated"]       = &core::World::get<components::Animated>;
			world_type["get_batchsprite"]    = &core::World::get<components::BatchSprite>;
			world_type["get_oncollision"]    = &core::World::get<components::OnCollision>;
			world_type["get_particleeffect"] = &core::World::get<components::ParticleEffect>;
			world_type["get_primitive"]      = &core::World::get<components::Primitive2D>;
			world_type["get_renderable"]     = &core::World::get<components::Renderable>;
			world_type["get_rigidbody"]      = &core::World::get<components::RigidBody>;
			world_type["get_sprite"]         = &core::World::get<components::Sprite>;
			world_type["get_tag"]            = &core::World::get<components::Tag>;
			world_type["get_text"]           = &core::World::get<components::Text>;
			world_type["get_transform"]      = &core::World::get<components::Transform2D>;
		}
	} // namespace scripting
} // namespace galaxy