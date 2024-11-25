///
/// Shader.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/fs/VirtualFileSystem.hpp"
#include "galaxy/logging/Log.hpp"

#include "Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		Shader::Shader()
		{
		}

		Shader::Shader(Shader&& s)
		{
			if (valid())
			{
				::UnloadShader(*this);
			}

			this->id   = s.id;
			this->locs = s.locs;
		}

		Shader& Shader::operator=(Shader&& s)
		{
			if (this != &s)
			{
				if (valid())
				{
					::UnloadShader(*this);
				}

				this->id   = s.id;
				this->locs = s.locs;
			}

			return *this;
		}

		Shader::~Shader()
		{
			if (valid())
			{
				::UnloadShader(*this);
			}
		}

		bool Shader::load(const std::string& file)
		{
			auto& fs = entt::locator<fs::VirtualFileSystem>::value();

			auto data = fs.read(file);
			return parse(data);
		}

		bool Shader::load(const std::string& vertex, const std::string& frag)
		{
			auto& fs = entt::locator<fs::VirtualFileSystem>::value();

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
				result = load_into_raylib(vertex, frag);
			}

			return result;
		}

		int Shader::get_uniform_location(const std::string& name)
		{
			if (!m_cache.contains(name))
			{
				m_cache[name] = ::GetShaderLocation(*this, name.c_str());
			}

			return m_cache[name];
		}

		void Shader::set_uniform_sampler2d(const std::string& name, const int sampler2d)
		{
			::SetShaderValue(*this, get_uniform_location(name), &sampler2d, RL_SHADER_UNIFORM_SAMPLER2D);
		}

		bool Shader::valid() const
		{
			return ::IsShaderValid(*this);
		}

		bool Shader::preprocess(const std::string& src)
		{
			auto result = true;

			if (!src.empty())
			{
				std::string vertex;
				std::string frag;

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
							vertex = (pos == std::string::npos) ? src.substr(next_line) : src.substr(next_line, pos - next_line);
						}
						else
						{
							frag = (pos == std::string::npos) ? src.substr(next_line) : src.substr(next_line, pos - next_line);
						}
					}
					else
					{
						GALAXY_LOG(GALAXY_ERROR, "Failed to parse shader type. Must be 'vertex' or 'fragment'.");
						result = false;
					}
				}

				result = load_into_raylib(vertex, frag);
			}
			else
			{
				GALAXY_LOG(GALAXY_ERROR, "Empty source code provided to shader.");
				result = false;
			}

			return result;
		}

		bool Shader::load_into_raylib(const std::string& vertex, const std::string& frag)
		{
			if (valid())
			{
				::UnloadShader(*this);
			}

			::Shader shader = ::LoadShaderFromMemory(vertex.c_str(), frag.c_str());
			this->id        = shader.id;
			this->locs      = shader.locs;
		}
	} // namespace graphics
} // namespace galaxy
