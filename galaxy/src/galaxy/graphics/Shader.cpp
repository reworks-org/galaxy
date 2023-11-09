///
/// Shader.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <nlohmann/json.hpp>

#include "galaxy/core/ServiceLocator.hpp"
#include "galaxy/fs/VirtualFileSystem.hpp"

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

		Shader::Shader(const std::string& vertex_src, const std::string& fragment_src)
		{
			load_raw(vertex_src, fragment_src);
		}

		Shader::Shader(const nlohmann::json& json)
			: m_id {0}
		{
			if (json.count("file") > 0)
			{
				std::string file = json.at("file");
				load(file);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No valid shader loading config found.");
			}
		}

		Shader::Shader(Shader&& s)
		{
			this->destroy();

			this->m_id           = s.m_id;
			this->m_cache        = std::move(s.m_cache);
			this->m_fragment_src = std::move(s.m_fragment_src);
			this->m_vertex_src   = std::move(s.m_vertex_src);

			s.m_id = 0;
		}

		Shader& Shader::operator=(Shader&& s)
		{
			if (this != &s)
			{
				this->destroy();

				this->m_id           = s.m_id;
				this->m_cache        = std::move(s.m_cache);
				this->m_fragment_src = std::move(s.m_fragment_src);
				this->m_vertex_src   = std::move(s.m_vertex_src);

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

			auto data = fs.read<meta::FSTextR>(file);
			if (!data.empty())
			{
				return load_raw(data);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load shader '{0}'.", file);
				return false;
			}
		}

		bool Shader::load_raw(const std::string& src)
		{
			const auto paired_src = preprocess(src);

			if (paired_src.has_value())
			{
				const auto& pair = paired_src.value();
				return load_raw(pair.first, pair.second);
			}
			else
			{
				return false;
			}
		}

		bool Shader::load_raw(const std::string& vertex_src, const std::string& fragment_src)
		{
			bool result = true;

			if (vertex_src.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Shader was passed an empty vertex shader.");
				result = false;
			}

			if (fragment_src.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Shader was passed an empty fragment shader.");
				result = false;
			}

			if (result)
			{
				m_vertex_src   = vertex_src;
				m_fragment_src = fragment_src;
			}

			return result;
		}

		void Shader::compile()
		{
			// Error reporting for OpenGL.
			char info[1024] = {0};
			auto success    = 0;
			auto v_id       = 0u;
			auto f_id       = 0u;

			// Then we need to convert the stream to a c string because OpenGL requires a refernece to a c string.
			auto v_src = m_vertex_src.c_str();
			auto f_src = m_fragment_src.c_str();

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

			if (m_vertex_src.empty() || m_fragment_src.empty())
			{
				success = 0;
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

						// clang-format off
							auto uniform_info = UniformInfo
							{
								.location = glGetUniformLocation(m_id, uniform_name.get()),
								.count = count
							};
						// clang-format on

						m_cache.emplace(std::string(uniform_name.get(), length), uniform_info);
					}
				}
			}

			// Cleanup shaders.
			glDeleteShader(v_id);
			glDeleteShader(f_id);

			std::string().swap(m_fragment_src);
			std::string().swap(m_vertex_src);
		}

		void Shader::destroy()
		{
			if (m_id != 0)
			{
				glDeleteProgram(m_id);
				m_id = 0;
			}
		}

		void Shader::bind()
		{
			glUseProgram(m_id);
		}

		void Shader::unbind()
		{
			glUseProgram(0);
		}

		GLint Shader::get_uniform_location(const std::string& name)
		{
			if (m_cache.contains(name))
			{
				return m_cache[name].location;
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to get uniform location for '{0}'.", name);
				return -1;
			}
		}

		GLint Shader::get_uniform_count(const std::string& name)
		{
			if (m_cache.contains(name))
			{
				return m_cache[name].count;
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to get uniform count for '{0}'.", name);
				return -1;
			}
		}

		meta::optional_ref<UniformInfo> Shader::get_uniform_info(const std::string& name)
		{
			if (m_cache.contains(name))
			{
				return std::make_optional(std::ref(m_cache[name]));
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to get uniform info for '{0}'.", name);
				return std::nullopt;
			}
		}

		unsigned int Shader::id() const
		{
			return m_id;
		}

		std::optional<std::pair<std::string, std::string>> Shader::preprocess(const std::string& src)
		{
			std::pair<std::string, std::string> source;

			const auto token = "#type";
			const auto len   = std::strlen(token);

			auto pos = src.find(token, 0);
			while (pos != std::string::npos)
			{
				const auto eol   = src.find_first_of("\r\n", pos);
				const auto begin = pos + len + 1;

				auto type = src.substr(begin, eol - begin);

				if (type != "vertex" && type != "fragment")
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to parse shader type. Must be 'vertex' or 'fragment'.");
					return std::nullopt;
				}
				else
				{
					const auto next_line = src.find_first_not_of("\r\n", eol);
					pos                  = src.find(token, next_line);

					if (type == "vertex")
					{
						source.first = (pos == std::string::npos) ? src.substr(next_line) : src.substr(next_line, pos - next_line);
					}
					else
					{
						source.second = (pos == std::string::npos) ? src.substr(next_line) : src.substr(next_line, pos - next_line);
					}
				}
			}

			if (source.first.empty() || source.second.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to parse a combined shader.");
				return std::nullopt;
			}
			else
			{
				return std::make_optional(source);
			}
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif
