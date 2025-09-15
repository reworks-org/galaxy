///
/// Registry.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_ENTITY_REGISTRY_HPP_
#define GALAXY_ENTITY_REGISTRY_HPP_

#include <entt/entity/registry.hpp>

namespace galaxy
{
	///
	/// Wrapper around entt::registry to expand functionality.
	///
	class Registry final
	{
		///
		/// Typedef for creating b2 bodies from components.
		///
		// using B2BodyConstruction = meta::vector<std::pair<components::RigidBody*, components::Transform*>>;

	public:
		///
		/// Constructor.
		///
		Registry();

		///
		/// Move constructor.
		///
		Registry(Registry&&);

		///
		/// Move assignment operator.
		///
		Registry& operator=(Registry&&);

		///
		/// Destructor.
		///
		~Registry();

		///
		/// \brief Create an entity with some default components.
		///
		/// \return Created entity, or entt::null if failed.
		///
		//[[maybe_unused]]
		// entt::entity create();

		///
		/// Create an entity from a prefab.
		///
		/// \param name Name of the prefab to load.
		///
		/// \return Created entity, or entt::null if failed.
		///
		//[[maybe_unused]]
		// entt::entity create_from_prefab(const std::string& name);

		///
		/// Validate an entity to make sure all components have met their requirements as defined by register_dependencies().
		///
		/// \param entity Entity to validate.
		///
		/// \return True if entity is valid.
		///
		//[[nodiscard]]
		// bool is_valid(const entt::entity entity);

		///
		/// \brief Updates pending component data.
		///
		/// For example, box2d bodies.
		///
		/// \param b2World Box2d world data.
		///
		// void update(b2World& b2World);

		///
		/// Clear any pending data.
		///
		// void clear();

	private:
		///
		/// Copy constructor.
		///
		Registry(const Registry&) = delete;

		///
		/// Copy assignment operator.
		///
		Registry& operator=(const Registry&) = delete;

		///
		/// Function that integrates a box2d construction with entt.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void construct_rigidbody(entt::registry& registry, entt::entity entity);

		///
		/// Function that integrates a box2d destruction with entt.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void destroy_rigidbody(entt::registry& registry, entt::entity entity);

		///
		/// Function that integrates lua init with entt on construct event.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void construct_script(entt::registry& registry, entt::entity entity);

		///
		/// Function that integrates lua destroy with entt on destruction event.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void destruct_script(entt::registry& registry, entt::entity entity);

		///
		/// Function that integrates nuklear init with entt on construct event.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void construct_nui(entt::registry& registry, entt::entity entity);

		///
		/// Function that integrates nuklear destroy with entt on destruction event.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void destruct_nui(entt::registry& registry, entt::entity entity);

		///
		/// Function that runs when an entity is enabled.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void enable_entity(entt::registry& registry, entt::entity entity);

		///
		/// Function that runs when an entity is disabled.
		///
		/// \param registry Registry component belongs to.
		/// \param entity Entity component belongs to.
		///
		// void disable_entity(entt::registry& registry, entt::entity entity);

	public:
		///
		/// entt registry.
		///
		entt::registry m_entt;

	private:
		///
		/// List of rigid bodies that need to be constructed.
		///
		// B2BodyConstruction m_bodies_to_construct;
	};
} // namespace galaxy

#endif
