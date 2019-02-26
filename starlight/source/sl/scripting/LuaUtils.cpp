///
/// LuaUtils.hpp
/// starlight
///
/// Created by reworks on 02/07/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>

#include "sl/core/World.hpp"
#include "sl/math/Vector2.hpp"
#include "sl/math/Vector3.hpp"
#include "sl/math/Vector4.hpp"
#include "sl/core/StateMachine.hpp"
#include "sl/core/ServiceLocator.hpp"
#include "sl/resources/MusicPlayer.hpp"
#include "sl/scripting/Sol2Interface.hpp"
#include "sl/libs/entt/entity/registry.hpp"
#include "sl/components/PhysicsComponent.hpp"
#include "sl/core/Command.hpp"

#include "LuaUtils.hpp"

// GCC 8 Workaround.
#if defined(__GNUC__) || defined(__GNUG__)

namespace sol
{
	template <>
	struct is_automagical<sl::Rect<float, int>> : std::false_type {};

	template <>
	struct is_automagical<sl::AnimationComponent> : std::false_type {};

	template <>
	struct is_automagical<sl::ParallaxComponent> : std::false_type {};

	template <>
	struct is_automagical<sl::ParticleComponent> : std::false_type {};

	template <>
	struct is_automagical<sl::RenderComponent> : std::false_type {};

	template <>
	struct is_automagical<sl::TransformComponent> : std::false_type {};

	template <>
	struct is_automagical<sl::PhysicsComponent> : std::false_type {};

	template <>
	struct is_automagical<sl::ScrollingBackgroundComponent> : std::false_type {};

	template <>
	struct is_automagical<entt::DefaultRegistry> : std::false_type {};
}

#endif

namespace sl
{
	void LuaUtils::writeTableToFile(const sol::table& table, const std::string& file, const std::string& tableName)
	{
		// Opens a file to write to and prints formatting correctly.
		std::ofstream out(file);
		out << tableName << " = " << "\n";
		out << "{" << "\n";

		// Loops over each pair in table to write out.
		table.for_each([&](std::pair<sol::object, sol::object> pair)
		{
			out << "    " << pair.first.as<std::string>() << " = " << pair.second.as<std::string>() << "," << "\n";
		});

		// Erase last ',' from table.
		out << '\b';
		out << '\b';
		out << " ";

		// Close up table and flush stream.
		out << "}" << std::endl;
		out.close();
	}

	void LuaUtils::registerStarlightUsertypes()
	{
		// Now all the usertypes we want to access from lua are registered.
		Locator::lua->new_usertype<std::uint32_t>("uint32_t");
		Locator::lua->new_usertype<std::uint16_t>("uint16_t");
		Locator::lua->new_usertype<entt::DefaultRegistry::entity_type>("entity");

		// Engine usertypes.
		Locator::lua->new_usertype<Vector2<int>>("Vector2i",
			sol::constructors<Vector2<int>(), Vector2<int>(int, int)>(),
			"transpose", &Vector2<int>::transpose,
			"x", &Vector2<int>::m_x,
			"y", &Vector2<int>::m_y
			);

		Locator::lua->new_usertype<Vector2<float>>("Vector2f",
			sol::constructors<Vector2<float>(), Vector2<float>(float, float)>(),
			"transpose", &Vector2<float>::transpose,
			"x", &Vector2<float>::m_x,
			"y", &Vector2<float>::m_y
			);

		Locator::lua->new_usertype<Vector3<int>>("Vector3i",
			sol::constructors<Vector3<int>(), Vector3<int>(int, int, int)>(),
			"x", &Vector3<int>::m_x,
			"y", &Vector3<int>::m_y,
			"z", &Vector3<int>::m_z
			);

		Locator::lua->new_usertype<Vector3<float>>("Vector3f",
			sol::constructors<Vector3<float>(), Vector3<float>(float, float, float)>(),
			"x", &Vector3<float>::m_x,
			"y", &Vector3<float>::m_y,
			"z", &Vector3<float>::m_z
			);

		Locator::lua->new_usertype<Vector4<int>>("Vector4i",
			sol::constructors<Vector4<int>(), Vector4<int>(int, int, int, int)>(),
			"w", &Vector4<int>::m_w,
			"x", &Vector4<int>::m_x,
			"y", &Vector4<int>::m_y,
			"z", &Vector4<int>::m_z
			);

		Locator::lua->new_usertype<Vector4<float>>("Vector4f",
			sol::constructors<Vector4<float>(), Vector4<float>(float, float, float, float)>(),
			"w", &Vector4<float>::m_w,
			"x", &Vector4<float>::m_x,
			"y", &Vector4<float>::m_y,
			"z", &Vector4<float>::m_z
			);

		Locator::lua->new_usertype<Rect<int>>("iRect",
			sol::constructors<Rect<int>(), Rect<int>(int, int, int, int)>(),
			"contains", sol::overload(sol::resolve<bool(int, int)>(&Rect<int>::contains), sol::resolve<bool(const Rect<int>&)>(&Rect<int>::contains)),
			"x", &Rect<int>::m_x,
			"y", &Rect<int>::m_y,
			"width", &Rect<int>::m_width,
			"height", &Rect<int>::m_height
			);

		Locator::lua->new_usertype<Rect<float>>("fRect",
			sol::constructors<Rect<float>(), Rect<float>(float, float, float, float)>(),
			"contains", sol::overload(sol::resolve<bool(float, float)>(&Rect<float>::contains), sol::resolve<bool(const Rect<float>&)>(&Rect<float>::contains)),
			"x", &Rect<float>::m_x,
			"y", &Rect<float>::m_y,
			"width", &Rect<float>::m_width,
			"height", &Rect<float>::m_height
			);

		Locator::lua->new_usertype<Rect<float, int>>("fiRect",
			sol::constructors<Rect<float, int>(), Rect<float, int>(float, float, int, int)>(),
			"contains", sol::overload(sol::resolve<bool(float, float)>(&Rect<float, int>::contains), sol::resolve<bool(const Rect<float, int>&)>(&Rect<float, int>::contains)),
			"x", &Rect<float, int>::m_x,
			"y", &Rect<float, int>::m_y,
			"width", &Rect<float, int>::m_width,
			"height", &Rect<float, int>::m_height
			);

		Locator::lua->new_usertype<AnimationComponent>("AnimationComponent",
			sol::constructors<AnimationComponent(const sol::table&)>(),
			"changeAnimation", &AnimationComponent::changeAnimation,
			"play", sol::overload(sol::resolve<void(void)>(&AnimationComponent::play), sol::resolve<void(const std::string&)>(&AnimationComponent::play)),
			"pause", &AnimationComponent::pause,
			"stop", &AnimationComponent::stop,
			"m_isPaused", &AnimationComponent::m_isPaused,
			"m_currentFrameTime", &AnimationComponent::m_currentFrameTime,
			"m_activeAnimation", &AnimationComponent::m_activeAnimation,
			"m_animations", &AnimationComponent::m_animations
			);

		Locator::lua->new_usertype<ParallaxComponent>("ParallaxComponent",
			sol::constructors<ParallaxComponent(const sol::table&)>(),
			"verticalSpeed", &ParallaxComponent::m_verticalSpeed,
			"horizontalSpeed", &ParallaxComponent::m_horizontalSpeed
			);

		Locator::lua->new_usertype<ParticleComponent>("ParticleComponent",
			sol::constructors<ParticleComponent(float, float, float)>(),
			"fade", &ParticleComponent::m_fade,
			"direction", &ParticleComponent::m_direction
			);

		Locator::lua->new_usertype<RenderComponent>("RenderComponent",
			sol::constructors<RenderComponent(const sol::table&), RenderComponent(float, const std::string&)>(),
			"opacity", &RenderComponent::m_opacity,
			"textureName", &RenderComponent::m_textureName
			);

		Locator::lua->new_usertype<TransformComponent>("TransformComponent",
			sol::constructors<TransformComponent(const sol::table&), TransformComponent(int, float, const Rect<float, int>&)>(),
			"layer", &TransformComponent::m_layer,
			"angle", &TransformComponent::m_angle,
			"rect", &TransformComponent::m_rect
			);

		Locator::lua->new_usertype<PhysicsComponent>("PhysicsComponent",
			sol::constructors<PhysicsComponent(const sol::table&)>(),
			"setFixtureEntity", &PhysicsComponent::setFixtureEntity
			);

		Locator::lua->new_usertype<ScrollingBackgroundComponent>("ScrollingBackgroundComponent",
			sol::constructors<ScrollingBackgroundComponent(const sol::table&), ScrollingBackgroundComponent(float)>()
			);

		Locator::lua->new_usertype<entt::DefaultRegistry>("Registry",
			sol::constructors<entt::DefaultRegistry()>(),
			"create", sol::resolve<entt::DefaultRegistry::entity_type(void)>(&entt::DefaultRegistry::create),
			"destroy", &Sol2Interface::enttDestroyWorkaround,
			"disable", sol::resolve<void(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::remove<EnabledComponent>),
			"assignEnabledComponent", &Sol2Interface::assignEnabledComponent,
			"assignAnimationComponent", &Sol2Interface::assignAnimationComponent,
			"assignParallaxComponent", &Sol2Interface::assignParallaxComponent,
			"assignParticleComponent", &Sol2Interface::assignParticleComponent,
			"assignRenderComponent", &Sol2Interface::assignRenderComponent,
			"assignSBComponent", &Sol2Interface::assignSBComponent,
			"assignTransformComponent", &Sol2Interface::assignTransformComponent,
			"getAnimationComponent", sol::resolve<AnimationComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<AnimationComponent>),
			"getParallaxComponent", sol::resolve<ParallaxComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<ParallaxComponent>),
			"getParticleComponent", sol::resolve<ParticleComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<ParticleComponent>),
			"getPhysicsComponent", sol::resolve<PhysicsComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<PhysicsComponent>),
			"getRenderComponent", sol::resolve<RenderComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<RenderComponent>),
			"getTransformComponent", sol::resolve<TransformComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<TransformComponent>),
			"getScrollingComponent", sol::resolve<ScrollingBackgroundComponent&(entt::DefaultRegistry::entity_type)>(&entt::DefaultRegistry::get<ScrollingBackgroundComponent>)
			);

		Locator::lua->set("registry", &(Locator::world->m_registry));

		// Including state so we can manipulate it from the debug interface and console.
		Locator::lua->new_usertype<StateMachine>("StateMachine",
			"push", &StateMachine::push,
			"pop", &StateMachine::pop);
		Locator::lua->set("stateMachine", Locator::stateMachine);

		// AudioPlayer
		Locator::lua->new_usertype<MusicPlayer>("MusicPlayer",
			"play", &MusicPlayer::play,
			"stop", &MusicPlayer::stop,
			"resume", &MusicPlayer::resume,
			"changeVolume", &MusicPlayer::changeVolume);
		Locator::lua->set("musicPlayer", Locator::musicPlayer);
	}
}