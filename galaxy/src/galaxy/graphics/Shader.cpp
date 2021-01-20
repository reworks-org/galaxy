///
/// Shader.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/FileSystem.hpp"

#include "Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		Shader::Shader() noexcept
		    : m_id {0}, m_loaded {false}
		{
		}

		Shader::Shader(std::string_view vertex_file, std::string_view frag_file)
		    : m_id {0}, m_loaded {false}
		{
			if (!load_path(vertex_file, frag_file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to construct shader with files: {0} | {1}.", vertex_file, frag_file);
			}
		}

		Shader::Shader(const nlohmann::json& json)
		{
			if ((json.count("vertex-file") > 0) && (json.count("frag-file") > 0))
			{
				std::string vert = json.at("vertex-file");
				std::string frag = json.at("frag-file");
				load_path(vert, frag);
			}
			else if ((json.count("vertex-string") > 0) && (json.count("frag-string") > 0))
			{
				std::string vert = json.at("vertex-string");
				std::string frag = json.at("frag-string");
				load_raw(vert, frag);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No valid shader loading config found.");
			}
		}

		Shader::Shader(const Shader& s) noexcept
		{
			this->m_id    = s.m_id;
			this->m_cache = s.m_cache;
		}

		Shader::Shader(Shader&& s) noexcept
		{
			this->m_id    = s.m_id;
			this->m_cache = std::move(s.m_cache);

			s.m_id = 0;
		}

		Shader& Shader::operator=(const Shader& s) noexcept
		{
			this->m_id    = s.m_id;
			this->m_cache = s.m_cache;

			return *this;
		}

		Shader& Shader::operator=(Shader&& s) noexcept
		{
			if (this != &s)
			{
				this->m_id    = s.m_id;
				this->m_cache = std::move(s.m_cache);

				s.m_id = 0;
			}

			return *this;
		}

		Shader::~Shader() noexcept
		{
			unbind();
			glDeleteProgram(m_id);
		}

		const bool Shader::load_path(std::string_view vertex_file, std::string_view frag_file)
		{
			const std::string vertex   = SL_HANDLE.vfs()->open(vertex_file);
			const std::string fragment = SL_HANDLE.vfs()->open(frag_file);

			return load_raw(vertex, fragment);
		}

		const bool Shader::load_raw(const std::string& vertex_str, const std::string& fragment_str)
		{
			bool result = true;

			if (vertex_str.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to read empty vert shader: {0}.", vertex_str);
				result = false;
			}

			if (fragment_str.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Attempted to read empty frag shader: {0}.", fragment_str);
				result = false;
			}

			if (result)
			{
				// Error reporting for OpenGL.
				char info[1024];
				int success       = 0;
				unsigned int v_id = 0;
				unsigned int f_id = 0;

				// Then we need to convert the stream to a c string because OpenGL requires a refernece to a c string.
				const char* v_src = vertex_str.c_str();
				const char* f_src = fragment_str.c_str();

				// Retrieve the ids from opengl when creating the shader, then compile shaders, while checking for errors.
				v_id = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(v_id, 1, &v_src, nullptr);
				glCompileShader(v_id);

				glGetShaderiv(v_id, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(v_id, 1024, nullptr, info);

					GALAXY_LOG(GALAXY_ERROR, "Failed to compile vertex shader. {0}.", info);
					result = false;
				}

				// Now do the same for the fragment shader.
				f_id = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(f_id, 1, &f_src, nullptr);
				glCompileShader(f_id);

				glGetShaderiv(f_id, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(f_id, 1024, nullptr, info);

					GALAXY_LOG(GALAXY_ERROR, "Failed to compile fragment shader. {0}.", info);
					result = false;
				}

				// Hopefully by this point nothing has gone wrong...
				if (result)
				{
					// Create and link program.
					m_id = glCreateProgram();
					glAttachShader(m_id, v_id);
					glAttachShader(m_id, f_id);
					glLinkProgram(m_id);

					glGetProgramiv(m_id, GL_LINK_STATUS, &success);
					if (!success)
					{
						glGetProgramInfoLog(m_id, 1024, nullptr, info);

						GALAXY_LOG(GALAXY_ERROR, "Failed to attach shaders: {0}.", info);
						result = false;
					}
				}

				// Cleanup shaders.
				glDeleteShader(v_id);
				glDeleteShader(f_id);
			}

			m_loaded = result;

			return m_loaded;
		}

		void Shader::bind() noexcept
		{
			glUseProgram(m_id);
		}

		void Shader::unbind() noexcept
		{
			glUseProgram(0);
		}

		const bool Shader::is_loaded() const noexcept
		{
			return m_loaded;
		}

		const GLint Shader::get_uniform_location(std::string_view name)
		{
			const auto str = static_cast<std::string>(name);

			// If uniform already exists return it from cache to avoid querying OpenGL, which is slow.
			if (m_cache.contains(str))
			{
				return m_cache[str];
			}
			else
			{
				// Otherwise, for the first time, retrieve location.
				const auto location = glGetUniformLocation(m_id, str.c_str());
				if (location != -1)
				{
					// Then if not error, add to hash map.
					m_cache.emplace(name, location);
				}
				else
				{
					GALAXY_LOG(GALAXY_WARNING, "Failed to find uniform: {0}.", name);
				}

				return location;
			}
		}
	} // namespace graphics
} // namespace galaxy