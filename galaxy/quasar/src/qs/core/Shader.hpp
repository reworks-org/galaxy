///
/// Shader.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SHADER_HPP_
#define QUASAR_SHADER_HPP_

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <pulsar/Log.hpp>

///
/// Core namespace.
///
namespace qs
{
	///
	/// Class for managing OpenGL shader programs.
	///
	class Shader final
	{
	public:
		///
		/// Default constructor.
		///
		Shader() noexcept;

		///
		/// Path constructor.
		///
		/// \param vertex_file Path to vertex shader source code.
		/// \param frag_file Path to fragment shader source code.
		///
		Shader(std::string_view vertex_file, std::string_view frag_file);

		///
		/// Destructor.
		///
		~Shader() noexcept;

		///
		/// Loads a shader into OpenGL from source and sets up the shader program.
		///
		/// \param vertex_file Path to vertex shader source code.
		/// \param fragment_file Path to fragment shader source code.
		///
		/// \return boolean True if successful.
		///
		bool load_path(std::string_view vertex_file, std::string_view fragment_file);

		///
		/// Loads a shader into OpenGL from raw strings and sets up the shader program.
		///
		/// \param vertex_str Raw vertex shader code.
		/// \param fragment_str Raw fragment shader code.
		///
		/// \return boolean True if successful.
		///
		bool load_raw(const std::string& vertex_str, const std::string& fragment_str);

		///
		/// Enable this shader for rendering.
		///
		void bind() noexcept;

		///
		/// Disable this currently active shader.
		///
		void unbind() noexcept;

		///
		/// \brief Specialized variadic template for setting shader uniforms.
		///
		/// Useful for i.e. 2f would be ("name", 1.0f, 2.0f) and
		/// 3i would be ("name", 1, 2, 3)
		///
		/// \param name The name of the uniform to set.
		/// \param args Variable amount of arguments for the uniform.
		///
		template<typename... uniforms>
		void set_uniform(std::string_view name, const uniforms&... args);

	private:
		///
		/// \brief Retrieves location of shader uniform from GPU.
		///
		/// In a seperate function because it checks interal cache first.
		///
		int get_uniform_location(std::string_view name) noexcept;

	private:
		///
		/// OpenGL id of shader program.
		///
		unsigned int m_id;

		///
		/// Cache of uniforms for better performance.
		///
		std::unordered_map<std::string, int> m_cache;
	};

	template<typename... uniforms>
	inline void Shader::set_uniform(std::string_view name, const uniforms&... args)
	{
		// If type does not have specialization, throw error.
		PL_LOG(PL_ERROR, "Invalid shader uniform type!");
	}

	template<>
	inline void Shader::set_uniform<int>(std::string_view name, const int& a) noexcept
	{
		glUniform1i(get_uniform_location(name), a);
	}

	template<>
	inline void Shader::set_uniform<int, int>(std::string_view name, const int& a, const int& b) noexcept
	{
		glUniform2i(get_uniform_location(name), a, b);
	}

	template<>
	inline void Shader::set_uniform<int, int, int>(std::string_view name, const int& a, const int& b, const int& c) noexcept
	{
		glUniform3i(get_uniform_location(name), a, b, c);
	}

	template<>
	inline void Shader::set_uniform<int, int, int, int>(std::string_view name, const int& a, const int& b, const int& c, const int& d) noexcept
	{
		glUniform4i(get_uniform_location(name), a, b, c, d);
	}

	template<>
	inline void Shader::set_uniform<unsigned int>(std::string_view name, const unsigned int& a) noexcept
	{
		glUniform1ui(get_uniform_location(name), a);
	}

	template<>
	inline void Shader::set_uniform<unsigned int, unsigned int>(std::string_view name, const unsigned int& a, const unsigned int& b) noexcept
	{
		glUniform2ui(get_uniform_location(name), a, b);
	}

	template<>
	inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int>(std::string_view name, const unsigned int& a, const unsigned int& b, const unsigned int& c) noexcept
	{
		glUniform3ui(get_uniform_location(name), a, b, c);
	}

	template<>
	inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int, unsigned int>(std::string_view name, const unsigned int& a, const unsigned int& b, const unsigned int& c, const unsigned int& d) noexcept
	{
		glUniform4ui(get_uniform_location(name), a, b, c, d);
	}

	template<>
	inline void Shader::set_uniform<float>(std::string_view name, const float& a) noexcept
	{
		glUniform1f(get_uniform_location(name), a);
	}

	template<>
	inline void Shader::set_uniform<float, float>(std::string_view name, const float& a, const float& b) noexcept
	{
		glUniform2f(get_uniform_location(name), a, b);
	}

	template<>
	inline void Shader::set_uniform<float, float, float>(std::string_view name, const float& a, const float& b, const float& c) noexcept
	{
		glUniform3f(get_uniform_location(name), a, b, c);
	}

	template<>
	inline void Shader::set_uniform<float, float, float, float>(std::string_view name, const float& a, const float& b, const float& c, const float& d) noexcept
	{
		glUniform4f(get_uniform_location(name), a, b, c, d);
	}

	template<>
	inline void Shader::set_uniform<double>(std::string_view name, const double& a) noexcept
	{
		glUniform1d(get_uniform_location(name), a);
	}

	template<>
	inline void Shader::set_uniform<double, double>(std::string_view name, const double& a, const double& b) noexcept
	{
		glUniform2d(get_uniform_location(name), a, b);
	}

	template<>
	inline void Shader::set_uniform<double, double, double>(std::string_view name, const double& a, const double& b, const double& c) noexcept
	{
		glUniform3d(get_uniform_location(name), a, b, c);
	}

	template<>
	inline void Shader::set_uniform<double, double, double, double>(std::string_view name, const double& a, const double& b, const double& c, const double& d) noexcept
	{
		glUniform4d(get_uniform_location(name), a, b, c, d);
	}

	template<>
	inline void Shader::set_uniform<glm::mat4>(std::string_view name, const glm::mat4& transform) noexcept
	{
		glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(transform));
	}

	template<>
	inline void Shader::set_uniform<glm::vec2>(std::string_view name, const glm::vec2& a) noexcept
	{
		glUniform2f(get_uniform_location(name), a.x, a.y);
	}

	template<>
	inline void Shader::set_uniform<glm::vec3>(std::string_view name, const glm::vec3& a) noexcept
	{
		glUniform3f(get_uniform_location(name), a.x, a.y, a.z);
	}

	template<>
	inline void Shader::set_uniform<glm::vec4>(std::string_view name, const glm::vec4& a) noexcept
	{
		glUniform4f(get_uniform_location(name), a.x, a.y, a.z, a.w);
	}

	template<>
	inline void Shader::set_uniform<std::array<float, 4>>(std::string_view name, const std::array<float, 4>& a) noexcept
	{
		glUniform4f(get_uniform_location(name), a[0], a[1], a[2], a[3]);
	}
} // namespace qs

#endif