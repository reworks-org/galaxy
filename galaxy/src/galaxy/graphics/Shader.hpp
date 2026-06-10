///
/// Shader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_HPP_
#define GALAXY_GRAPHICS_SHADER_HPP_

#include <ankerl/unordered_dense.h>
#include <Raylib.hpp>

#include "galaxy/logging/Log.hpp"

namespace galaxy
{
	///
	/// \brief OpenGL Shader Program.
	///
	/// Currently only supports vertex and fragment shaders.
	///
	class Shader final
	{
	public:
		///
		/// Constructor.
		///
		Shader() noexcept;

		///
		/// Move constructor.
		///
		Shader(Shader&&) noexcept;

		///
		/// Move assignment operator.
		///
		Shader& operator=(Shader&&) noexcept;

		///
		/// Destructor.
		///
		~Shader() noexcept;

		///
		/// Loads a combined shader.
		///
		/// \param file Path to a combined shader source.
		///
		/// \return True if successful.
		///
		[[nodiscard]]
		bool load(const std::string& file) noexcept;

		///
		/// Loads an OpenGL shader.
		///
		/// \param vertex Vertex shader.
		/// \param frag Fragment shader.
		///
		/// \return True if successful.
		///
		[[nodiscard]]
		bool load(const std::string& vertex, const std::string& frag) noexcept;

		///
		/// Loads a combined raw shader.
		///
		/// \param src Raw shader code.
		///
		/// \return True if successful.
		///
		[[nodiscard]]
		bool parse(const std::string& src) noexcept;

		///
		/// Loads a raw shader.
		///
		/// \param vertex Raw vertex shader code.
		/// \param frag Raw fragment shader code.
		///
		/// \return True if successful.
		///
		[[nodiscard]]
		bool parse(const std::string& vertex, const std::string& frag) noexcept;

		///
		/// \brief Specialized variadic template for setting shader uniforms.
		///
		/// Useful for i.e. 2f would be ("name", 1.0f, 2.0f) and
		/// 3i would be ("name", 1, 2, 3)
		///
		/// \param name The name of the uniform to set.
		/// \param args Variable amount of arguments for the uniform.
		///
		template<typename... Uniforms>
		void set_uniform(const std::string& name, const Uniforms&... args);

	private:
		///
		/// Copy constructor.
		///
		Shader(const Shader&) = delete;

		///
		/// Copy assignment operator.
		///
		Shader& operator=(const Shader&) = delete;

		///
		/// Retrieves the location of a shader uniform.
		///
		/// \param name Shader uniform name.
		///
		/// \return Uniform location. -1 on error.
		///
		[[nodiscard]]
		int get_uniform_location(const std::string& name);

	private:
		///
		/// Handle.
		///
		ray::Shader m_shader;

		///
		/// Cache of uniforms for better performance.
		///
		ankerl::unordered_dense::map<std::string, int> m_cache;
	};

	template<>
	inline void Shader::set_uniform(const std::string& name)
	{
		GALAXY_LOG(GALAXY_ERROR, "Tried to set uniform without value: '{0}'.", name);
	}

	template<>
	inline void Shader::set_uniform<bool>(const std::string& name, const bool& a)
	{
		ray::SetShaderValue(m_shader, get_uniform_location(name), &a, ray::SHADER_UNIFORM_INT);
	}

	template<>
	inline void Shader::set_uniform<int>(const std::string& name, const int& a)
	{
		ray::SetShaderValue(m_shader, get_uniform_location(name), &a, ray::SHADER_UNIFORM_INT);
	}

	template<>
	inline void Shader::set_uniform<unsigned int>(const std::string& name, const unsigned int& a)
	{
		ray::SetShaderValue(m_shader, get_uniform_location(name), &a, ray::SHADER_UNIFORM_UINT);
	}

	template<>
	inline void Shader::set_uniform<float>(const std::string& name, const float& a)
	{
		ray::SetShaderValue(m_shader, get_uniform_location(name), &a, ray::SHADER_UNIFORM_FLOAT);
	}

	template<>
	inline void Shader::set_uniform<ray::Vector2>(const std::string& name, const ray::Vector2& a)
	{
		ray::SetShaderValue(m_shader, get_uniform_location(name), &a, ray::SHADER_UNIFORM_VEC2);
	}

	template<>
	inline void Shader::set_uniform<ray::Vector3>(const std::string& name, const ray::Vector3& a)
	{
		ray::SetShaderValue(m_shader, get_uniform_location(name), &a, ray::SHADER_UNIFORM_VEC3);
	}

	template<>
	inline void Shader::set_uniform<ray::Vector4>(const std::string& name, const ray::Vector4& a)
	{
		ray::SetShaderValue(m_shader, get_uniform_location(name), &a, ray::SHADER_UNIFORM_VEC4);
	}

	template<>
	inline void Shader::set_uniform<ray::Matrix>(const std::string& name, const ray::Matrix& a)
	{
		ray::SetShaderValueMatrix(m_shader, get_uniform_location(name), a);
	}

	template<>
	inline void Shader::set_uniform<ray::Texture2D>(const std::string& name, const ray::Texture2D& a)
	{
		ray::SetShaderValueTexture(m_shader, get_uniform_location(name), a);
	}
} // namespace galaxy

#endif
