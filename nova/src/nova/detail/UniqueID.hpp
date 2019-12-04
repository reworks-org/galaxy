///
/// UniqueID.hpp
/// nova
///
/// Refer to LICENSE.txt for more details.
///

#ifndef NOVA_UNIQUEID_HPP_
#define NOVA_UNIQUEID_HPP_

#include <cstddef>

namespace nova
{
	///
	/// Generates a unique id for a type for each type of specialization.
	/// And the id is kept as a compile time constant.
	///
	template<typename Specialization>
	class UniqueID
	{
	public: 
		///
		/// Use this function to retrieve the ID.
		/// Will generate a new id if it is called for the first time.
		///
		/// \return SR_INTEGER id.
		///
		template<typename Type>
		static inline std::size_t uid()
		{
			static std::size_t id = s_counter++;
			return id;
		}

	private:
		///
		/// Internal counter to keep track of allocated ids.
		///
		static inline std::size_t s_counter = 0;
	};

	///
	/// Predefinition of unique id structure for events.
	///
	using EventUniqueID = UniqueID<struct EUID>;
}

#endif