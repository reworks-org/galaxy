///
/// PickerSystem.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_SYSTEMS_PICKERSYSTEM_HPP_
#define SUPERCLUSTER_SYSTEMS_PICKERSYSTEM_HPP_

#include <galaxy/ecs/System.hpp>

namespace sc
{
	///
	/// System that handles rendering of entities with a graphics::Sprite, TransformComponent, etc.
	///
	class PickerSystem final : public galaxy::ecs::System
	{
	public:
		///
		/// Constructor.
		///
		PickerSystem() noexcept = default;

		///
		/// Destructor.
		///
		virtual ~PickerSystem() noexcept = default;

	private:
		///
		/// Abstract implementation for updating the system. Use the manager to retreive your components.
		///
		/// \param world Game World containing entities.
		/// \param dt DeltaTime from gameloop.
		///
		void update(core::World& world, const double dt) override;
	};
} // namespace sc

#endif