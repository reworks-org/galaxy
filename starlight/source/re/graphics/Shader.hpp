///
///  Shader.hpp
///  starlight
///
///  Created by reworks on 14/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SHADER_HPP_
#define STARLIGHT_SHADER_HPP_

#include <allegro5/shader.h>

#include "sol2/sol_forward.hpp"

namespace sl
{
	class Shader
	{
	public:
		///
		/// Constructor.
		///
		/// \param table sol::table containing data.
		///
		Shader(const sol::table& table);

		///
		/// Copy Constructor.
		///
		Shader(const Shader&) = default;

		///
		/// Move Constructor.
		///
		Shader(Shader&&) = default;

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

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Shader() = delete;
	};
}

#endif