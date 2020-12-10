///
/// GUI.hpp
/// redshift
///
/// Refer to LICENSE.txt for more details.
///

#ifndef REDSHIFT_WORLD_HPP_
#define REDSHIFT_WORLD_HPP_

#include <protostar/async/ThreadPool.hpp>
#include <starlight/Dispatcher.hpp>

#include "redshift/Meta.hpp"
#include "redshift/collision/DynamicTree.hpp"

///
/// Core namespace.
///
namespace rs
{
	class World final
	{
	public:
		World();
		World(const glm::vec2& gravity);
		~World();

		void step();

		template<is_body DerivedBody, typename... Args>
		[[maybe_unused]] std::shared_ptr<DerivedBody> create(Args&&... args);

		template<is_body DerivedBody>
		void remove(std::shared_ptr<DerivedBody> body);

		void set_gravity(glm::vec2& gravity);

	private:
		///
		/// Resolve the collision between two bodies.
		///
		void resolve_collision(std::shared_ptr<Body> a, std::shared_ptr<Body> b);

		sl::Dispatcher m_dispatcher;
		pr::ThreadPool<4> m_pool;

		glm::vec2 m_gravity;
		std::vector<std::shared_ptr<Body>> m_bodies;
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
		std::erase_if
	}
} // namespace rs

#endif