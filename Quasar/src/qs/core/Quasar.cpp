///
/// Quasar.cpp
/// Quasar
///
/// Created by reworks on 16/09/2018.
/// MIT LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "Quasar.hpp"

namespace qs
{
	Quasar::Quasar()
	{
		// all temp
		SDL_Init();
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	}
}