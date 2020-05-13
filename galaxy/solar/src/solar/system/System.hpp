///
/// System.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_SYSTEM_HPP_
#define SOLAR_SYSTEM_HPP_

#include <protostar/async/ProtectedArithmetic.hpp>

///
/// Core namespace.
///
namespace sr
{
	///
	/// Forward dec.
	///
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
		virtual void events() noexcept = 0;

		///
		/// Abstract implementation for updating the system. Use the manager to retreive your components.
		///
		/// \param deltaTime "Lag" from gameloop.
		/// \param manager Entity manager.
		///
		virtual void update(protostar::ProtectedDouble* deltaTime, sr::Manager& manager) noexcept = 0;

	protected:
		///
		/// Default constructor.
		///
		System() noexcept = default;

		///
		/// Move constructor.
		///
		System(System&&) noexcept = default;

		///
		/// Copy constructor.
		///
		System(const System&) noexcept = default;
	};
}

#endif