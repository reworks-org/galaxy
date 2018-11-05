///
/// Quasar.hpp
/// Quasar
///
/// Created by reworks on 16/09/2018.
/// MIT LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_QUASAR_HPP_
#define QUASAR_QUASAR_HPP_

namespace qs
{
	class Quasar
	{
	public:
		///
		/// Initializes SDL2 and creates OpenGL context.
		///
		/// \param flags Flags to pass to SDL_Init().
		///
		Quasar(Uint32 flags);

		///
		/// Cleans up SDL2 and OpenGL.
		///
		~Quasar();
	};
}

#endif