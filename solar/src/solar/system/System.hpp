///
/// System.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_SYSTEM_HPP_
#define SOLAR_SYSTEM_HPP_

#include "solar/Config.hpp"

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
		/// Virtualized destructor.
		///
		virtual inline ~System() {}

		///
		/// Abstract implementation for processing an event.
		///
		/// \param e Event object to pass to system.
		///
		virtual void event(const sr::Event& e) = 0;

		///
		/// Abstract implementation for updating the system. Use the manager to retreive your components.
		///
		/// \param time DeltaTime from gameloop.
		/// \param manager Entity manager.
		///
		virtual void update(const sr::DeltaTime time, sr::Manager& manager) = 0;
	};
}

#endif