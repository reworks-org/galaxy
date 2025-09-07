///
/// Shader.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/error/Log.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/utils/Globals.hpp"

#include "Shader.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26414)
#endif

namespace galaxy
{
	namespace graphics
	{
		Shader::Shader()
			: m_id {0}
		{
		}

		Shader::Shader(const nlohmann::json& json)
			: m_id {0}
		{
			if (json.count("file") > 0)
			{
				GALAXY_UNUSED(load(json.at("file")));
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No shader file found in JSON constructor.");
			}
		}

		Shader::Shader(Shader&& s)
		{
			if (this->m_id != 0)
			{
				glDeleteProgram(this->m_id);
			}

			this->m_cache = std::move(s.m_cache);
			this->m_src   = std::move(s.m_src);
			this->m_id    = s.m_id;

			s.m_id = 0;
		}

		Shader& Shader::operator=(Shader&& s)
		{
			if (this != &s)
			{
				if (this->m_id != 0)
				{
					glDeleteProgram(this->m_id);
				}

				this->m_cache = std::move(s.m_cache);
				this->m_src   = std::move(s.m_src);
				this->m_id    = s.m_id;

				s.m_id = 0;
			}

			return *this;
		}

		Shader::~Shader()
		{
			destroy();
		}

		bool Shader::load(const std::string& file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto data = fs.read(file);
			return parse(data);
		}

		bool Shader::load(const std::string& vertex, const std::string& frag)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			auto v = fs.read(vertex);
			auto f = fs.read(frag);

			return parse(v, f);
		}

		bool Shader::parse(const std::string& src)
		{
			return preprocess(src);
		}

		bool Shader::parse(const std::string& vertex, const std::string& frag)
		{
			auto result = true;

			if (vertex.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Shader was passed an empty vertex shader.");
				result = false;
			}

			if (frag.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Shader was passed an empty fragment shader.");
				result = false;
			}

			if (result)
			{
				m_src.first()  = vertex;
				m_src.second() = frag;
			}

			return result;
		}

		void Shader::compile()
		{
			if (!m_src.first().empty() && !m_src.second().empty())
			{
				// Error reporting for OpenGL.
				char info[1024] = {0};
				auto success    = 0;
				auto v_id       = 0u;
				auto f_id       = 0u;

				// Then we need to convert the stream to a c string because OpenGL requires a refernece to a c string.
				auto v_src = m_src.first().c_str();
				auto f_src = m_src.second().c_str();

				// Retrieve the ids from opengl when creating the shader, then compile shaders, while checking for errors.
				v_id = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(v_id, 1, &v_src, nullptr);
				glCompileShader(v_id);

				glGetShaderiv(v_id, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(v_id, 1024, nullptr, info);

					GALAXY_LOG(GALAXY_ERROR, "Failed to compile vertex shader. {0}.", info);
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
				}

				if (success)
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
					}

					GLint uniform_count = 0;
					glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &uniform_count);

					if (uniform_count != 0)
					{
						GLint max_name_len = 0;
						glGetProgramiv(m_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &max_name_len);

						GLsizei length = 0;
						GLsizei count  = 0;
						GLenum  type   = GL_NONE;

						for (GLint i = 0; i < uniform_count; ++i)
						{
							auto uniform_name = std::make_unique<char[]>(max_name_len);
							glGetActiveUniform(m_id, i, max_name_len, &length, &count, &type, uniform_name.get());

							m_cache.emplace(std::string(uniform_name.get(), length), glGetUniformLocation(m_id, uniform_name.get()));
						}
					}
				}

				// Cleanup shaders.
				glDeleteShader(v_id);
				glDeleteShader(f_id);
				m_src = {};
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Tried to compile empty shader.");
			}
		}

		void Shader::destroy()
		{
			if (m_id != 0)
			{
				glDeleteProgram(m_id);
			}
		}

		void Shader::bind() const
		{
			glUseProgram(m_id);
		}

		void Shader::unbind() const
		{
			glUseProgram(0);
		}

		unsigned int Shader::id() const
		{
			return m_id;
		}

		bool Shader::preprocess(const std::string& src)
		{
			auto result = true;

			if (!src.empty())
			{
				const auto token = "#type";
				const auto len   = std::strlen(token);

				auto pos = src.find(token, 0);
				while (pos != std::string::npos)
				{
					const auto eol   = src.find_first_of("\r\n", pos);
					const auto begin = pos + len + 1;

					auto type = src.substr(begin, eol - begin);

					if (type == "vertex" || type == "fragment")
					{
						const auto next_line = src.find_first_not_of("\r\n", eol);
						pos                  = src.find(token, next_line);

						if (type == "vertex")
						{
							m_src.first() = (pos == std::string::npos) ? src.substr(next_line) : src.substr(next_line, pos - next_line);
						}
						else
						{
							m_src.second() = (pos == std::string::npos) ? src.substr(next_line) : src.substr(next_line, pos - next_line);
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to parse shader type. Must be 'vertex' or 'fragment'.");
						result = false;
					}
				}
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Empty source code provided to shader.");
				result = false;
			}

			return result;
		}

		int Shader::get_uniform_location(const std::string& name)
		{
			if (m_cache.contains(name))
			{
				return m_cache[name];
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to get uniform location for '{0}'.", name);
				return -1;
			}
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
