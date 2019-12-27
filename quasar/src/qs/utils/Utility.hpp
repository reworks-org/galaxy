///
/// Utility.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_UTILITY_HPP_
#define QUASAR_UTILITY_HPP_

#include <SDL2/SDL_stdinc.h>

namespace qs
{
	///
	/// \brief Utility functions used by Quasar.
	///
	/// Cannot be instantiated.
	///
	class Utils final
	{
	public:
		///
		/// Converts a float to a unsigned 8 bit integer.
		///
		/// \param f Float to convert.
		///
		/// \return Uint8.
		///
		static Uint8 fromFloat(float f);

	private:
		///
		/// Deleted Constructor.
		///
		Utils() = delete;

		///
		/// Deleted copy constructor.
		///
		Utils(const Utils&) = delete;

		///
		/// Deleted move constructor.
		///
		Utils(Utils&&) = delete;
	};
}

#endif