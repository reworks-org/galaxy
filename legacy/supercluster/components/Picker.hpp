///
/// Picker.hpp
/// supercluster
///
/// Refer to LICENSE.txt for more details.
///

#ifndef SUPERCLUSTER_COMPONENTS_PICKER_HPP_
#define SUPERCLUSTER_COMPONENTS_PICKER_HPP_

#include <galaxy/physics/Box2DIntegration.hpp>

namespace sc
{
	namespace core
	{
		class World;
	} // namespace core

	class Picker final
	{
		friend class core::World;

	public:
		///
		/// Constructor.
		///
		Picker() noexcept;

		///
		/// Move constructor.
		///
		Picker(Picker&&) noexcept;

		///
		/// Move assignment operator.
		///
		Picker& operator=(Picker&&) noexcept;

		///
		/// Destructor.
		///
		~Picker();

	private:
		///
		/// Copy constructor.
		///
		Picker(const Picker&) = delete;

		///
		/// Copy assignment operator.
		///
		Picker& operator=(const Picker&) = delete;

	private:
		///
		/// Allows body to interact with world.
		///
		b2World* m_world_pointer;

		///
		/// Box2D body.
		///
		b2Body* m_body;
	};
} // namespace sc

#endif