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
	class System
	{
	public:
		System();
		virtual ~System();

		virtual void event() = 0;
		virtual void update() = 0;
		virtual void render() = 0;

	private:

	};
}

#endif