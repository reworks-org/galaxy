///
/// UniqueID.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_UNIQUEID_HPP_
#define SOLAR_UNIQUEID_HPP_

#include "solar/Config.hpp"

///
/// Core namespace.
///
namespace sr
{
	///
	/// Generates a unique id for a type for each type of specialization.
	/// And the id is kept as a compile time constant.
	///
	template<typename Specialization>
	class UniqueID final
	{
		///
		/// Specialization should be a struct or class.
		///
		static_assert(std::is_class<Specialization>::value);

	public:
		///
		/// Use this function to retrieve the ID.
		/// Will generate a new id if it is called for the first time.
		///
		/// \return SR_INTEGER id.
		///
		template<typename Type>
		static const SR_INTEGER get();

	private:
		///
		/// Internal counter to keep track of allocated ids.
		///
		static inline SR_INTEGER s_counter = 0;
	};

	template<typename Specialization>
	template<typename Type>
	const SR_INTEGER sr::UniqueID<Specialization>::get()
	{
		static SR_INTEGER id = s_counter++;
		return id;
	}

	///
	/// Predefinition of unique id structure for components.
	///
	using cUniqueID = UniqueID<struct ComponentUniqueID>;

	///
	/// Predefinition of unique id structure for systems.
	///
	using sUniqueID = UniqueID<struct SystemUniqueID>;
}

#endif