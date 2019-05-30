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
	template<typename Specialization>
	class UID
	{
	public:
		template<typename Type>
		static inline SR_INTEGER uid()
		{
			static SR_INTEGER id = s_counter++;
			return id;
		}

	private:
		static inline SR_INTEGER s_counter = 0;
	};

	using cuid = UID<struct ComponentUniqueID>;
	using suid = UID<struct SystemUniqueID>;
}

#endif