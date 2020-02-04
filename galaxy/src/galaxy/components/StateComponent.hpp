///
/// StateComponent.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_STATECOMPONENT_HPP_
#define GALAXY_STATECOMPONENT_HPP_

///
/// Core namespace.
///
namespace galaxy
{
	///
	/// Flag component used to determine if an entity is "alive" or "dead".
	///
	class StateComponent final
	{
	public:
		///
		/// Default constructor.
		///
		StateComponent() noexcept;

		///
		/// Argument constructor.
		///
		/// \param isAlive Allows to create an entity that is dead initially.
		///
		explicit StateComponent(const bool isAlive) noexcept;

		///
		/// Default destructor.
		///
		~StateComponent() noexcept = default;

	public:
		///
		/// Alive/Dead flag.
		///
		bool m_alive;
	};
}

#endif