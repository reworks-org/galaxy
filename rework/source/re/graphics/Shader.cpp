///
///  Shader.cpp
///  rework
///
///  Created by reworks on 14/10/2017.
///  Copyright (c) 2018+ reworks.
///  Refer to LICENSE.txt for more details.
///

#include "re/fs/VFS.hpp"
#include "loguru/loguru.hpp"

#include "Shader.hpp"

namespace re
{
	Shader::Shader(sol::table& table)
	{
		m_shader = al_create_shader(ALLEGRO_SHADER_GLSL);

		// http://liballeg.org/a5docs/trunk/shader.html

		if (!al_attach_shader_source(m_shader, ALLEGRO_VERTEX_SHADER, VFS::get()->openAsString(table.get<std::string>("vertexShader")).c_str()))
		{
			LOG_S(ERROR) << "Could not attach vertex shader. Allegro Log: " << al_get_shader_log(m_shader);
		}

		if (!al_attach_shader_source(m_shader, ALLEGRO_PIXEL_SHADER, VFS::get()->openAsString(table.get<std::string>("pixelShader")).c_str()))
		{
			LOG_S(ERROR) << "Could not attach pixel shader. Allegro Log: " << al_get_shader_log(m_shader);
		}

		if (!al_build_shader(m_shader))
		{
			LOG_S(ERROR) << "Could not build shader. Allegro Log: " << al_get_shader_log(m_shader);
		}
	}

	Shader::~Shader()
	{
		al_destroy_shader(m_shader);
	}

	void Shader::use()
	{
		al_use_shader(m_shader);
	}

	void Shader::disable()
	{
		al_use_shader(nullptr);
	}
}