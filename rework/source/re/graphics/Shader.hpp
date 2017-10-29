//
//  Shader.hpp
//  rework
//
//  Created by reworks on 14/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef REWORK_SHADER_HPP_
#define REWORK_SHADER_HPP_

#include <allegro5/shader.h>

#include "sol2/sol.hpp"

namespace re
{
	class Shader
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		Shader(sol::table& table);

		///
		/// Destructor.
		///
		~Shader();

		///
		/// \brief Use the shader.
		///
		/// Uses the shader for subsequent drawing operations on the current target bitmap i.e window.
		///
		void use();

		///
		/// Removes the shader from the current drawing target.
		///
		void disable();

	private:
		ALLEGRO_SHADER* m_shader;
	};
}

#endif