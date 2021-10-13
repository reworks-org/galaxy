///
/// System.hpp
/// galaxy
///
/// See LICENSE.txt.
///

#ifndef GALAXY_ECS_SYSTEM_HPP_
#define GALAXY_ECS_SYSTEM_HPP_

namespace galaxy
{
	namespace core
	{
		class Scene2D;
	} // namespace core

	namespace ecs
	{
		///
		/// Represents a system that operates on sets of components.
		///
		class System
		{
		public:
			///
			/// Virtual destructor.
			///
			virtual ~System() noexcept = default;

			///
			/// Abstract implementation for updating the system. Use the manager to retreive your components.
			///
			/// \param scene Currently active scene.
			///
			virtual void update(core::Scene2D* scene) = 0;

		protected:
			///
			/// Default constructor.
			///
			System() noexcept = default;
		};
	} // namespace ecs
} // namespace galaxy

#endif