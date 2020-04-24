///
/// Shader.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SHADER_HPP_
#define QUASAR_SHADER_HPP_

#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "qs/utils/Error.hpp"
#include "qs/core/Colours.hpp"
#include "qs/utils/Utility.hpp"

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
		/// Destructor.
		///
		~Shader() noexcept;

		///
		/// Loads a shader into OpenGL from source and sets up the shader program.
		///
		/// \param vertex Path to vertex shader source code.
		/// \param fragment Path to fragment shader source code.
		///
		/// \return boolean True if successful.
		///
		bool loadFromPath(const std::string& vertex, const std::string& fragment);

		///
		/// Loads a shader from a single file, seperated by #shader <type> preprocessor.
		///
		/// \param glsl Path to shader source.
		///
		bool loadFromPath(const std::string& glsl);

		///
		/// Loads a shader into OpenGL from raw strings and sets up the shader program.
		///
		/// \param vertex Raw vertex shader code.
		/// \param fragment Raw fragment shader code.
		///
		/// \return boolean True if successful.
		///
		bool loadFromRaw(const std::string& vertex, const std::string& fragment);

		///
		/// Loads a shader from a single file, seperated by #shader <type> preprocessor.
		///
		/// \param glsl Raw glsl shader code.
		///
		bool loadFromRaw(const std::string& glsl);

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
		template<typename ...UniformTypes>
		void setUniform(const std::string& name, const UniformTypes&... args);

	private:
		///
		/// \brief Retrieves location of shader uniform from GPU.
		///
		/// In a seperate function because it checks interal cache first.
		///
		int getUniformLocation(const std::string& name);

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

	template<typename ...UniformTypes>
	inline void Shader::setUniform(const std::string& name, const UniformTypes&... args)
	{
		// If type does not have specialization, throw error.
		qs::Error::handle().callback("Shader.hpp", 102, "Invalid shader uniform type!");
	}

	template<>
	inline void Shader::setUniform<int>(const std::string& name, const int& a)
	{
		glUniform1i(getUniformLocation(name), a);
	}

	template<>
	inline void Shader::setUniform<int, int>(const std::string& name, const int& a, const int& b)
	{
		glUniform2i(getUniformLocation(name), a, b);
	}

	template<>
	inline void Shader::setUniform<int, int, int>(const std::string& name, const int& a, const int& b, const int& c)
	{
		glUniform3i(getUniformLocation(name), a, b, c);
	}

	template<>
	inline void Shader::setUniform<int, int, int, int>(const std::string& name, const int& a, const int& b, const int& c, const int& d)
	{
		glUniform4i(getUniformLocation(name), a, b, c, d);
	}

	template<>
	inline void Shader::setUniform<unsigned int>(const std::string& name, const unsigned int& a)
	{
		glUniform1ui(getUniformLocation(name), a);
	}

	template<>
	inline void Shader::setUniform<unsigned int, unsigned int>(const std::string& name, const unsigned int& a, const unsigned int& b)
	{
		glUniform2ui(getUniformLocation(name), a, b);
	}

	template<>
	inline void Shader::setUniform<unsigned int, unsigned int, unsigned int>(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c)
	{
		glUniform3ui(getUniformLocation(name), a, b, c);
	}

	template<>
	inline void Shader::setUniform<unsigned int, unsigned int, unsigned int, unsigned int>(const std::string& name, const unsigned int& a, const unsigned int& b, const unsigned int& c, const unsigned int& d)
	{
		glUniform4ui(getUniformLocation(name), a, b, c, d);
	}

	template<>
	inline void Shader::setUniform<float>(const std::string& name, const float& a)
	{
		glUniform1f(getUniformLocation(name), a);
	}

	template<>
	inline void Shader::setUniform<float, float>(const std::string& name, const float& a, const float& b)
	{
		glUniform2f(getUniformLocation(name), a, b);
	}

	template<>
	inline void Shader::setUniform<float, float, float>(const std::string& name, const float& a, const float& b, const float& c)
	{
		glUniform3f(getUniformLocation(name), a, b, c);
	}

	template<>
	inline void Shader::setUniform<float, float, float, float>(const std::string& name, const float& a, const float& b, const float& c, const float& d)
	{
		glUniform4f(getUniformLocation(name), a, b, c, d);
	}

	template<>
	inline void Shader::setUniform<double>(const std::string& name, const double& a)
	{
		glUniform1d(getUniformLocation(name), a);
	}

	template<>
	inline void Shader::setUniform<double, double>(const std::string& name, const double& a, const double& b)
	{
		glUniform2d(getUniformLocation(name), a, b);
	}

	template<>
	inline void Shader::setUniform<double, double, double>(const std::string& name, const double& a, const double& b, const double& c)
	{
		glUniform3d(getUniformLocation(name), a, b, c);
	}

	template<>
	inline void Shader::setUniform<double, double, double, double>(const std::string& name, const double& a, const double& b, const double& c, const double& d)
	{
		glUniform4d(getUniformLocation(name), a, b, c, d);
	}

	template<>
	inline void Shader::setUniform<glm::mat4>(const std::string& name, const glm::mat4& transform)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(transform));
	}

	template<>
	inline void Shader::setUniform<glm::vec2>(const std::string& name, const glm::vec2& a)
	{
		glUniform2f(getUniformLocation(name), a.x, a.y);
	}

	template<>
	inline void Shader::setUniform<glm::vec3>(const std::string& name, const glm::vec3& a)
	{
		glUniform3f(getUniformLocation(name), a.x, a.y, a.z);
	}
	
	template<>
	inline void Shader::setUniform<glm::vec4>(const std::string& name, const glm::vec4& a)
	{
		glUniform4f(getUniformLocation(name), a.x, a.y, a.z, a.w);
	}

	template<>
	inline void Shader::setUniform<std::array<float, 4>>(const std::string& name, const std::array<float, 4>& a)
	{
		glUniform4f(getUniformLocation(name), a[0], a[1], a[2], a[3]);
	}


	template<>
	inline void Shader::setUniform<protostar::Colour>(const std::string& name, const protostar::Colour& a)
	{
		glUniform4f(getUniformLocation(name),
			qs::Utils::uint8ToFloat(a.m_red),
			qs::Utils::uint8ToFloat(a.m_green),
			qs::Utils::uint8ToFloat(a.m_blue),
			qs::Utils::uint8ToFloat(a.m_alpha)
		);
	}
}

#endif