///
/// System.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_SYSTEM_HPP_
#define SOLAR_SYSTEM_HPP_

///
/// Core namespace.
///
namespace sr
{
	class Manager;

	///
	/// Represents a system that operates on sets of components.
	///
	class System
	{
	public:
		///
		/// Default virtual destructor.
		///
		virtual ~System() noexcept = default;

		///
		/// Abstract implementation for processing events.
		///
		virtual void events() = 0;

		///
		/// Abstract implementation for updating the system. Use the manager to retreive your components.
		///
		/// \param dt "Lag" from gameloop.
		///
		virtual void update(const double dt) = 0;

	protected:
		///
		/// Default constructor.
		///
		System() = default;

		///
		/// Move constructor.
		///
		System(System&&) noexcept = default;

		///
		/// Copy constructor.
		///
		System(const System&) noexcept = default;
	};
} // namespace sr

#endif