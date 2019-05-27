///
/// ID.hpp
///
/// solar
/// See LICENSE.txt.
///

#ifndef SOLAR_ID_HPP_
#define SOLAR_ID_HPP_

#include "solar/Config.hpp"

namespace sr
{
	template<typename Specialization>
	class UID
	{
	public:
		template<typename Type>
		static SR_INTEGER uid();

	private:
		static inline SR_INTEGER s_counter = 1;
	};

	template<typename Specialization>
	template<typename Type>
	inline SR_INTEGER UID<Specialization>::uid()
	{
		static SR_INTEGER id = s_counter++;
		return id;
	}

	using cuid = UID<struct ComponentUniqueID>;
	using suid = UID<struct SystemUniqueID>;
	using euid = UID<struct EntityUniqueID>;
}

#endif