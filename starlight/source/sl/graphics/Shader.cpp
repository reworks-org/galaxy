///
/// Shader.cpp
/// starlight
///
/// Created by reworks on 14/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#include <allegro5/error.h>

#include "sl/fs/VirtualFS.hpp"
#include "sl/libs/loguru/loguru.hpp"
#include "sl/core/ServiceLocator.hpp"

#include "Shader.hpp"

namespace sl
{
	Shader::Shader(const sol::table& table)
		:m_shader(nullptr)
	{
		// Create shader object.
		m_shader = al_create_shader(ALLEGRO_SHADER_AUTO);

		// Attach vertex shader source to object, making sure it succeeded.
		if (!al_attach_shader_source(m_shader, ALLEGRO_VERTEX_SHADER, Locator::virtualFS->openAsString(table.get<std::string>("vertexShader")).c_str()))
		{
			LOG_S(ERROR) << "Could not attach vertex shader. File: " << table.get<std::string>("vertexShader") << ". Allegro Log: " << al_get_shader_log(m_shader) << "Error no: " << al_get_errno();
		}

		// Attach pixel shader source to object, making sure it succeeded.
		if (!al_attach_shader_source(m_shader, ALLEGRO_PIXEL_SHADER, Locator::virtualFS->openAsString(table.get<std::string>("pixelShader")).c_str()))
		{
			LOG_S(ERROR) << "Could not attach pixel shader. File: " << table.get<std::string>("pixelShader") << ". Allegro Log: " << al_get_shader_log(m_shader) << "Error no: " << al_get_errno();
		}

		// Compile shader code.
		if (!al_build_shader(m_shader))
		{
			LOG_S(ERROR) << "Could not build shader. Allegro Log: " << al_get_shader_log(m_shader) << "Error no: " << al_get_errno();
		}
	}

	Shader::Shader(const ALLEGRO_SHADER_PLATFORM platform, const std::string& vertexShader, const std::string& pixelShader)
		:m_shader(nullptr)
	{
		// Create shader object.
		m_shader = al_create_shader(platform);

		// Attach vertex shader source to object, making sure it succeeded.
		if (!al_attach_shader_source(m_shader, ALLEGRO_VERTEX_SHADER, Locator::virtualFS->openAsString(vertexShader).c_str()))
		{
			LOG_S(ERROR) << "Could not attach vertex shader. File: " << vertexShader << ". Allegro Log: " << al_get_shader_log(m_shader) << "Error no: " << al_get_errno();
		}

		// Attach pixel shader source to object, making sure it succeeded.
		if (!al_attach_shader_source(m_shader, ALLEGRO_PIXEL_SHADER, Locator::virtualFS->openAsString(pixelShader).c_str()))
		{
			LOG_S(ERROR) << "Could not attach pixel shader. File: " << pixelShader << ". Allegro Log: " << al_get_shader_log(m_shader) << "Error no: " << al_get_errno();
		}

		// Compile shader code.
		if (!al_build_shader(m_shader))
		{
			LOG_S(ERROR) << "Could not build shader. Allegro Log: " << al_get_shader_log(m_shader) << "Error no: " << al_get_errno();
		}
	}

	Shader::~Shader()
	{
		if (m_shader)
		{
			al_destroy_shader(m_shader);
		}
	}

	void Shader::use() const
	{
		al_use_shader(m_shader);
	}

	void Shader::disable() const
	{
		al_use_shader(nullptr);
	}
}