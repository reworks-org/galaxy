///
/// Quasar.cpp
/// Quasar
///
/// Created by reworks on 16/09/2018.
/// MIT LICENSE.
/// Refer to LICENSE.txt for more details.
///

//https://learnopengl.com/Getting-started/Hello-Window

#include <SDL2/SDL.h>

#include "Quasar.hpp"

namespace qs
{
	Quasar::Quasar(Uint32 flags)
	{
		if (SDL_Init(flags) != 0)
		{
			// Get the SDL error message.
			std::string message = "SDL_Init failed! SDL: ";
			message += SDL_GetError();

			// Then throw exception.
			throw std::runtime_error(message);
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	}

	Quasar::~Quasar()
	{
	}
}