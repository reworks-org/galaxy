///
/// World.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_WORLD_HPP_
#define REDSHIFT_WORLD_HPP_

#include <optional>

#include <protostar/async/ThreadPool.hpp>
#include <starlight/Dispatcher.hpp>

#include "redshift/Meta.hpp"
#include "redshift/collision/DynamicTree.hpp"
#include "redshift/collision/Manifold.hpp"

///
/// Core namespace.
///
namespace rs
{
	class World final
	{
	public:
		static inline const constexpr float PENETRATION_PERCENT = 0.2f;
		static inline const constexpr float SLOP_THRESHOLD      = 0.01f;

		World();
		World(const glm::vec2& gravity);
		~World();

		void step(const double dt);

		template<is_body DerivedBody, typename... Args>
		[[maybe_unused]] std::shared_ptr<DerivedBody> create(Args&&... args);

		template<is_body DerivedBody>
		void remove(std::shared_ptr<DerivedBody> body);

		void set_gravity(glm::vec2& gravity);

	private:
		[[nodiscard]] std::optional<Manifold> narrow_phase_collision(Body* a, Body* b);
		void resolve_collision(Body* a, Body* b, const Manifold& manifold);

		sl::Dispatcher m_dispatcher;
		pr::ThreadPool<4> m_pool;

		glm::vec2 m_gravity;
		std::vector<std::shared_ptr<Body>> m_bodies;
		std::vector<std::shared_ptr<Collidable>> m_removed;
		DynamicTree m_collision_tree;
	};

	template<is_body DerivedBody, typename... Args>
	inline std::shared_ptr<DerivedBody> World::create(Args&&... args)
	{
		auto body = std::make_shared<DerivedBody>(std::forward<Args>(args)...);

		m_bodies.push_back(std::static_pointer_cast<Body>(body));
		m_collision_tree.insert(std::static_pointer_cast<Collidable>(body));

		return body;
	}

	template<is_body DerivedBody>
	inline void World::remove(std::shared_ptr<DerivedBody> body)
	{
		// clang-format off
		m_bodies.erase(std::remove_if(m_bodies.begin(), m_bodies.end(), [&](std::shared_ptr<DerivedBody> ptr)
		{
			return ptr == body;
		}),
		m_bodies.end());
		m_collision_tree.remove(std::static_pointer_cast<Collidable>(body));
		// clang-format on
	}
} // namespace rs

#endif