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
		virtual void event(const Event& e) = 0;

		///
		/// Abstract implementation for updating the system. Use heliosphere to retreive your components.
		///
		virtual void update(const DeltaTime time, Manager& hs) = 0;
	};
}

#endif