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

		//
		///
		//////////////
		template<typename ...UniformTypes>
		void setUniform(const std::string& name, const UniformTypes&... args);

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

	template<typename ...UniformTypes>
	inline void Shader::setUniform(const std::string& name, const UniformTypes&... args)
	{
		qs::Error::handle().callback("Shader.hpp", 90, "Invalid shader uniform type!");
	}
}

#endif