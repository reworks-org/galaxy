///
/// Shader.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include <entt/locator/locator.hpp>

#include "galaxy/fs/VirtualFileSystem.hpp"

#include "Shader.hpp"

namespace galaxy
{
	Shader::Shader() noexcept
	{
	}

	Shader::Shader(Shader&& s) noexcept
	{
		if (this->m_shader.id != 0)
		{
			ray::UnloadShader(this->m_shader);
			this->m_cache.clear();
		}

		this->m_shader.id   = s.m_shader.id;
		this->m_shader.locs = s.m_shader.locs;
		this->m_cache       = std::move(s.m_cache);

		s.m_shader = {0};
	}

	Shader& Shader::operator=(Shader&& s) noexcept
	{
		if (this != &s)
		{
			if (this->m_shader.id != 0)
			{
				ray::UnloadShader(this->m_shader);
				this->m_cache.clear();
			}

			this->m_shader.id   = s.m_shader.id;
			this->m_shader.locs = s.m_shader.locs;
			this->m_cache       = std::move(s.m_cache);

			s.m_shader = {0};
		}

		return *this;
	}

	Shader::~Shader() noexcept
	{
		if (m_shader.id != 0)
		{
			ray::UnloadShader(m_shader);
		}
	}

	bool Shader::load(const std::string& file) noexcept
	{
		auto& fs = entt::locator<VirtualFileSystem>::value();

		auto data = fs.read(file);
		return parse(data);
	}

	bool Shader::load(const std::string& vertex, const std::string& frag) noexcept
	{
		auto& fs = entt::locator<VirtualFileSystem>::value();

		auto v = fs.read(vertex);
		auto f = fs.read(frag);

		return parse(v, f);
	}

	bool Shader::parse(const std::string& src) noexcept
	{
		auto result = true;

		if (!src.empty())
		{
			std::string vertex, fragment;

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
						fragment = (pos == std::string::npos) ? src.substr(next_line) : src.substr(next_line, pos - next_line);
					}
				}
				else
				{
					GALAXY_LOG(GALAXY_ERROR, "Failed to parse shader type. Must be 'vertex' or 'fragment'.");
					result = false;
				}
			}

			if (result)
			{
				return parse(vertex, fragment);
			}
		}
		else
		{
			GALAXY_LOG(GALAXY_ERROR, "Empty source code provided to shader.");
			result = false;
		}

		return result;
	}

	bool Shader::parse(const std::string& vertex, const std::string& frag) noexcept
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
			m_shader = ray::LoadShaderFromMemory(vertex.c_str(), frag.c_str());
			return ray::IsShaderValid(m_shader);
		}

		return result;
	}

	int Shader::get_uniform_location(const std::string& name)
	{
		if (!m_cache.contains(name))
		{
			m_cache[name] = ray::GetShaderLocation(m_shader, name.c_str());
		}

		return m_cache[name];
	}
} // namespace galaxy
