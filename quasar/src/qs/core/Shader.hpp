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

#include "qs/utils/Error.hpp"

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
		/// Creation Constructor.
		///
		/// \param vertexFile Path to vertex shader source code.
		/// \param fragmentFile Path to fragment shader source code.
		///
		Shader(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile);

		///
		/// Default destructor.
		///
		~Shader() noexcept;

		///
		/// Loads a shader into OpenGL from source and sets up the shader program.
		///
		/// \param vertexFile Path to vertex shader source code.
		/// \param fragmentFile Path to fragment shader source code.
		///
		/// \return boolean True if successful.
		///
		bool load(const std::filesystem::path& vertexFile, const std::filesystem::path& fragmentFile);

		///
		/// Enable this shader for rendering.
		///
		void use() noexcept;

		///
		/// Disable this currently active shader.
		///
		void disable() noexcept;

		///
		/// Set a uniform for integers or bools.
		///
		template<typename ...Args>
		void setUniformIntegral(const std::string& name, const Args& ...args) const;

		template<typename ...Args>
		void setUniformFloating(const std::string& name, const Args& ...args) const;

	private:
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

	template<typename ...Args>
	inline void Shader::setUniformIntegral(const std::string& name, const Args& ...args) const
	{
		if (!std::is_integral<typename std::decay<Args>::type>::value)
		{
			throw std:::runtime_error("setUniformIB - type not integral.");
		}

		constexpr std::size_t count = sizeof...(Args);

		switch (count)
		{
		case 1:
			glUniform1i(getUniformLocation(name), static_cast<int>(args));
			break;

		case 2:
			glUniform2i(getUniformLocation(name), static_cast<int>(args)...);
			break;

		case 3:
			glUniform3i(getUniformLocation(name), static_cast<int>(args)...);
			break;

		case 4:
			glUniform4i(getUniformLocation(name), static_cast<int>(args)...);
			break;

		default:
			qs::Error::handle().callback("Shader.hpp", 111, "SetUniformInt Invalid size. Must be 1-4.");
			break;
		}
	}

	template<typename ...Args>
	inline void Shader::setUniformFloating(const std::string& name, const Args& ...args) const
	{
		if (!std::is_floating_point<typename std::decay<Args>::type>::value)
		{
			throw std:::runtime_error("setUniformFloating - type not decimal.");
		}

		constexpr std::size_t count = sizeof...(Args);

		switch (count)
		{
		case 1:
			glUniform1f(getUniformLocation(name), static_cast<float>(args));
			break;

		case 2:
			glUniform2f(getUniformLocation(name), static_cast<float>(args)...);
			break;

		case 3:
			glUniform3f(getUniformLocation(name), static_cast<float>(args)...);
			break;

		case 4:
			glUniform4f(getUniformLocation(name), static_cast<float>(args)...);
			break;

		default:
			qs::Error::handle().callback("Shader.hpp", 140, "SetUniformFloat Invalid size. Must be 1-4.");
			break;
		}
	}
}

#endif