///
/// Shader.cpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include "qs/libs/glad/glad.h"

#include "Shader.hpp"

namespace qs
{
	Shader::Shader()
		:m_type(qs::Shader::Type::VERTEX), m_id(0)
	{
	}

	Shader::~Shader()
	{
	}

	qs::Result Shader::load(const std::string& source, qs::Shader::Type type)
	{
		qs::Result result;
		m_type = type;

		// Determine the type of shader to create.
		switch (m_type)
		{
		case qs::Shader::Type::VERTEX:
			m_id = glCreateShader(GL_VERTEX_SHADER);
			break;

		case qs::Shader::Type::FRAGMENT:
			m_id = glCreateShader(GL_FRAGMENT_SHADER);
			break;

		case qs::Shader::Type::GEOMETRY:
			m_id = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		}
	
		// Attach source and compile shader.
		const char* c_str = source.c_str();
		glShaderSource(m_id, 1, &c_str, nullptr);
		glCompileShader(m_id);

		// Check for errors in compilation. First we have to check success of compile.
		int success;
		char log[512];
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			// Retrieve log and set error info.
			glGetShaderInfoLog(m_id, 512, nullptr, log);
			result.m_message = "Failed to compile shader. Log: ";
			result.m_message += log;
			result.m_status = qs::Result::Status::FAILURE;
		}
		else
		{
			// Create shader program.
		}

		return result;
	}
	void Shader::use()
	{
	}
}