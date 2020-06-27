///
/// Shader.cpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#include <fstream>
#include <sstream>

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

	Shader::~Shader() noexcept
	{
		unbind();
		glDeleteProgram(m_id);
	}

	bool Shader::loadFromPath(const std::string& vertex, const std::string& fragment) noexcept
	{
		// Set up vars.
		bool result = true;
		unsigned int vertexID = 0;
		unsigned int fragmentID = 0;

		// Convert to proper paths and:
		// Create an input stream of the file from disk in read only mode.
		std::ifstream vertexStream(std::filesystem::path(vertex).string(), std::ios::in);
		std::ifstream fragmentStream(std::filesystem::path(fragment).string(), std::ios::in);

		// Check for errors...
		if (!vertexStream)
		{
			PL_LOG(PL_ERROR, "std::ifstream failed to open file: " + vertex);

			result = false;
		}

		if (!fragmentStream)
		{
			PL_LOG(PL_ERROR, "std::ifstream failed to open file: " + fragment);

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
				PL_LOG(PL_ERROR, "std::stringstream failed to read vertexBuffer for: " + vertex);
				result = false;
			}

			if (!fragmentBuffer)
			{
				PL_LOG(PL_ERROR, "std::stringstream failed to read fragmentBuffer for: " + fragment);
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
					PL_LOG(PL_ERROR, err);

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
					PL_LOG(PL_ERROR, err);

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
						PL_LOG(PL_ERROR, err);

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

	bool Shader::loadFromPath(const std::string& glsl) noexcept
	{
		// Set up vars.
		bool result = true;
		constexpr unsigned short VERTEX = 0;
		constexpr unsigned short FRAGMENT = 1;
		std::string line = "";
		std::stringstream stringStream[2];

		// Convert to proper paths and:
		// Create an input stream of the file from disk in read only mode.
		std::ifstream shader;
		shader.open(std::filesystem::path(glsl).string(), std::ios::in);

		// Check for errors...
		if (!shader)
		{
			PL_LOG(PL_ERROR, "std::ifstream failed to open file: " + glsl);
			result = false;
		}
		else
		{
			// Parse the two shaders out of one file.

			unsigned short type = 2;
			while (std::getline(shader, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
					{
						type = VERTEX;
					}
					else if (line.find("fragment") != std::string::npos)
					{
						type = FRAGMENT;
					}
				}
				else
				{
					stringStream[type] << line << "\n";
				}
			}

			if (!stringStream[VERTEX])
			{
				PL_LOG(PL_ERROR, "std::stringstream failed to read vertex shader for: " + glsl);
				result = false;
			}

			if (!stringStream[FRAGMENT])
			{
				PL_LOG(PL_ERROR, "std::stringstream failed to read fragment shader for: " + glsl);
				result = false;
			}
		}

		shader.close();

		if (result)
		{
			return loadFromRaw(stringStream[VERTEX].str(), stringStream[FRAGMENT].str());
		}
		else
		{
			return result;
		}
	}

	bool Shader::loadFromRaw(const std::string& vertex, const std::string& fragment) noexcept
	{
		// Set up vars.
		bool result = true;

		if (vertex.empty())
		{
			PL_LOG(PL_ERROR, "Vertex shader string was empty.");
			result = false;
		}

		if (fragment.empty())
		{
			PL_LOG(PL_ERROR, "Fragment shader string was empty.");
			result = false;
		}

		if (result)
		{
			// OpenGL vars.
			int success = 0;
			char infoLog[1024];
			unsigned int vertexID = 0;
			unsigned int fragmentID = 0;

			// Then we need to convert the stream to a c string because OpenGL requires a refernece to a c string. yeah.
			const char* vertexSource = vertex.c_str();
			const char* fragmentSource = fragment.c_str();

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
				PL_LOG(PL_ERROR, err);

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
				PL_LOG(PL_ERROR, err);

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
					PL_LOG(PL_ERROR, err);

					result = false;
				}
			}

			// Cleanup shaders.
			glDeleteShader(vertexID);
			glDeleteShader(fragmentID);
		}

		return result;
	}

	bool Shader::loadFromRaw(const std::string& glsl) noexcept
	{
		// Set up vars.
		bool result = true;
		constexpr unsigned short VERTEX = 0;
		constexpr unsigned short FRAGMENT = 1;
		std::string line = "";
		std::stringstream stringStream[2];

		// Check for errors...
		if (glsl.empty())
		{
			PL_LOG(PL_ERROR, "GLSL shader file is empty.");
			result = false;
		}
		else
		{
			// Parse the two shaders out of one file.
			
			unsigned short type = 2;
			std::istringstream shader(glsl);
			while (std::getline(shader, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
					{
						type = VERTEX;
					}
					else if (line.find("fragment") != std::string::npos)
					{
						type = FRAGMENT;
					}
				}
				else
				{
					stringStream[type] << line << "\n";
				}
			}

			if (!stringStream[VERTEX])
			{
				PL_LOG(PL_ERROR, "std::istringstream failed to read vertex shader for: " + glsl);
				result = false;
			}

			if (!stringStream[FRAGMENT])
			{
				PL_LOG(PL_ERROR, "std::istringstream failed to read fragment shader for: " + glsl);
				result = false;
			}
		}

		if (result)
		{
			return loadFromRaw(stringStream[VERTEX].str(), stringStream[FRAGMENT].str());
		}
		else
		{
			return false;
		}
	}

	void Shader::bind() noexcept
	{
		glUseProgram(m_id);
	}

	void Shader::unbind() noexcept
	{
		glUseProgram(0);
	}

	int Shader::getUniformLocation(const std::string& name) noexcept
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
				PL_LOG(PL_WARNING, "Failed to find uniform: " + name);
			}

			return location;
		}
	}
}