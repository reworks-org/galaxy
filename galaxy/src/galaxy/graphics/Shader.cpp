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

#include "Shader.hpp"

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(push)
#pragma warning(disable : 26414)
#endif

namespace galaxy
{
	namespace graphics
	{
		Shader::Shader() noexcept
			: m_id {0}
		{
		}

		Shader::Shader(std::string_view vertex_file, std::string_view frag_file)
			: m_id {0}
		{
			if (!load_file(vertex_file, frag_file))
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to construct shader with files: {0} | {1}.", vertex_file, frag_file);
			}
		}

		Shader::Shader(const nlohmann::json& json)
			: m_id {0}
		{
			if ((json.count("vertex_file") > 0) && (json.count("fragment_file") > 0))
			{
				std::string vert = json.at("vertex_file");
				std::string frag = json.at("fragment_file");
				load_file(vert, frag);
			}
			else if ((json.count("vertex_raw") > 0) && (json.count("fragment_raw") > 0))
			{
				std::string vert = json.at("vertex_raw");
				std::string frag = json.at("fragment_raw");
				load_raw(vert, frag);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "No valid shader loading config found.");
			}
		}

		Shader::Shader(Shader&& s) noexcept
		{
			this->destroy();

			this->m_id           = s.m_id;
			this->m_cache        = std::move(s.m_cache);
			this->m_fragment_src = std::move(s.m_fragment_src);
			this->m_vertex_src   = std::move(s.m_vertex_src);

			s.m_id = 0;
		}

		Shader& Shader::operator=(Shader&& s) noexcept
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

		Shader::~Shader() noexcept
		{
			destroy();
		}

		bool Shader::load_file(std::string_view vertex_file, std::string_view frag_file)
		{
			auto& fs = core::ServiceLocator<fs::VirtualFileSystem>::ref();

			const auto vertex = fs.open(vertex_file);
			if (!vertex.has_value())
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load vertex shader '{0}'.", vertex_file);
				return false;
			}

			const auto fragment = fs.open(frag_file);
			if (!fragment.has_value())
			{
				GALAXY_LOG(GALAXY_ERROR, "Failed to load fragment shader '{0}'.", frag_file);
				return false;
			}

			return load_raw(vertex.value(), fragment.value());
		}

		bool Shader::load_raw(std::string_view vertex_str, std::string_view fragment_str)
		{
			bool result = true;

			if (vertex_str.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Shader was passed an empty vertex shader.");
				result = false;
			}

			if (fragment_str.empty())
			{
				GALAXY_LOG(GALAXY_ERROR, "Shader was passed an empty fragment shader.");
				result = false;
			}

			if (result)
			{
				m_vertex_src   = vertex_str;
				m_fragment_src = fragment_str;
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
					GLenum type    = GL_NONE;
					for (GLint i = 0; i < uniform_count; ++i)
					{
						auto uniform_name = std::make_unique<char[]>(max_name_len);
						glGetActiveUniform(m_id, i, max_name_len, &length, &count, &type, uniform_name.get());

						// clang-format off
							auto uniform_info = UniformInfo
							{
								.m_location = glGetUniformLocation(m_id, uniform_name.get()),
								.m_count = count
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

		void Shader::destroy() noexcept
		{
			if (m_id != 0)
			{
				glDeleteProgram(m_id);
				m_id = 0;
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

		GLint Shader::get_uniform_location(const std::string& name)
		{
			if (m_cache.contains(name))
			{
				return m_cache[name].m_location;
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
				return m_cache[name].m_count;
			}
			else
			{
				GALAXY_LOG(GALAXY_WARNING, "Failed to get uniform count for '{0}'.", name);
				return -1;
			}
		}

		meta::OptionalRef<UniformInfo> Shader::get_uniform_info(const std::string& name)
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

		unsigned int Shader::id() const noexcept
		{
			return m_id;
		}
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
#pragma warning(pop)
#endif