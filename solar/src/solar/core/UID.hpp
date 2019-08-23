///
/// UID.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_UID_HPP_
#define SOLAR_UID_HPP_

#include "solar/Config.hpp"

namespace sr
{
	///
	/// Generates a unique id for a type for each type of specialization.
	/// And the id is kept as a compile time constant.
	///
	template<typename Specialization>
	class UID
	{
	public:
		///
		/// Use this function to retrieve the ID.
		/// Will generate a new id if it is called for the first time.
		///
		/// \return SR_INTEGER id.
		///
		template<typename Type>
		static inline SR_INTEGER uid()
		{
			static SR_INTEGER id = s_counter++;
			return id;
		}

	private:
		///
		/// Internal counter to keep track of allocated ids.
		///
		static inline SR_INTEGER s_counter = 0;
	};

	///
	/// Predefinition of unique id structure for components.
	///
	using cuid = UID<struct ComponentUniqueID>;

	///
	/// Predefinition of unique id structure for systems.
	///
	using suid = UID<struct SystemUniqueID>;
}

#endif