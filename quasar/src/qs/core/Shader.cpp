///
/// Shader.cpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <sstream>

#include <glad/glad.h>

#include "qs/utils/Error.hpp"

#include "Shader.hpp"

///
/// Core namespace.
///
namespace qs
{
	Shader::Shader() noexcept
		:m_id(0)
	{
	}

	Shader::Shader(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile)
	{
		if (!load(vertexFile, fragmentFile))
		{
			qs::Error::handle().callback("Shader.cpp", 28, "Failed to create shader program!");
		}
	}

	Shader::~Shader() noexcept
	{
		glDeleteProgram(m_id);
	}

	bool Shader::load(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile)
	{
		// Yes, all this is required to open a file in C++17. Sigh. OK here we go:
		bool result = true;
		unsigned int vertexID = 0;
		unsigned int fragmentID = 0;

		// Retrieve path into string format.
		std::string vertexFileStr = vertexFile.string();
		std::string fragmentFileStr = fragmentFile.string();

		// Create an input stream of the file from disk in read only mode.
		std::ifstream vertexStream(vertexFileStr, std::ios::in);
		std::ifstream fragmentStream(fragmentFileStr, std::ios::in);
		
		// Check for errors...
		if (!vertexStream)
		{
			qs::Error::handle().callback("Shader.cpp", 55, "ifstream failed to open file: " + vertexFileStr);

			result = false;
		}

		if (!fragmentStream)
		{
			qs::Error::handle().callback("Shader.cpp", 62, "ifstream failed to open file: " + fragmentFileStr);

			result = false;
		}

		// If no errors occured, continue loading shader...
		if (result)
		{
			// Then read entire input buffer in to a string stream object.
			//  You can't read it directly into a std::string in a method that is resonably performant.
			std::stringstream vertexBuffer;
			std::stringstream fragmentBuffer;
			vertexBuffer << vertexStream.rdbuf();
			fragmentBuffer << fragmentStream.rdbuf();

			if (!vertexBuffer)
			{
				qs::Error::handle().callback("Shader.cpp", 79, "stringstream failed to read vertexBuffer for: " + vertexFileStr);

				result = false;
			}

			if (!fragmentBuffer)
			{
				qs::Error::handle().callback("Shader.cpp", 86, "stringstream failed to read fragmentBuffer for: " + fragmentFileStr);

				result = false;
			}

			// Once again, if no errors, continue loading...
			if (result)
			{
				// Error reporting for OpenGL.
				int success = 0;
				char infoLog[1024];

				// Then we need to convert the stream to a c string because OpenGL requires a refernece to a c string. yeah.
				std::string vertexSourceStr = vertexBuffer.str();
				const char* vertexSource = vertexSourceStr.c_str();
				
				std::string fragmentSourceStr = fragmentBuffer.str();
				const char* fragmentSource = fragmentSourceStr.c_str();

				// Retrieve the ids from opengl when creating the shader, then compile shaders, while checking for errors.
				vertexID = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertexID, 1, &vertexSource, nullptr);
				glCompileShader(vertexID);

				glGetShaderiv(vertexID, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertexID, 1024, nullptr, infoLog);
					
					std::string err = "Failed to vertex compile shader. GL_ERROR: ";
					err += infoLog;
					qs::Error::handle().callback("Shader.cpp", 120, err);

					result = false;
				}

				// Now do the same for the fragment shader.
				fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragmentID, 1, &fragmentSource, nullptr);
				glCompileShader(fragmentID);

				glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragmentID, 1024, nullptr, infoLog);

					std::string err = "Failed to compile fragment shader. GL_ERROR: ";
					err += infoLog;
					qs::Error::handle().callback("Shader.cpp", 137, err);

					result = false;
				}

				// Hopefully by this point nothing has gone wrong...
				if (result)
				{
					// Create and link program.
					m_id = glCreateProgram();
					glAttachShader(m_id, vertexID);
					glAttachShader(m_id, fragmentID);
					glLinkProgram(m_id);

					glGetProgramiv(m_id, GL_LINK_STATUS, &success);
					if (!success)
					{
						glGetProgramInfoLog(m_id, 1024, nullptr, infoLog);

						std::string err = "Failed to attach shaders. GL_ERROR: ";
						err += infoLog;
						qs::Error::handle().callback("Shader.cpp", 158, err);

						result = false;
					}
				}
			}
		}

		// Cleanup shaders.
		glDeleteShader(vertexID);
		glDeleteShader(fragmentID);

		// Close the input stream since we are done.
		vertexStream.close();
		fragmentStream.close();

		return result;
	}

	void Shader::use() noexcept
	{
		glUseProgram(m_id);
	}

	void Shader::disable() noexcept
	{
		glUseProgram(0);
	}

	int Shader::getUniformLocation(const std::string& name)
	{
		// If uniform already exists return it from cache to avoid querying OpenGL, which is slow.
		if (m_cache.find(name) != m_cache.end())
		{
			return m_cache[name];
		}
		else
		{
			// Otherwise, for the first time, retrieve location.
			unsigned int location = glGetUniformLocation(m_id, name.c_str());
			if (location != -1)
			{
				// Then if not error, add to hash map.
				m_cache.emplace(name, location);
			}
			else
			{
				qs::Error::handle().callback("Shader.cpp", 199, "Failed to find uniform: " + name);
			}

			return location;
		}
	}
}