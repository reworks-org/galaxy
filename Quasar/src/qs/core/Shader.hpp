///
/// Shader.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SHADER_HPP_
#define QUASAR_SHADER_HPP_

#include <string>
#include <filesystem>

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

		template<typename Type>
		void setUniform(const std::string& name, Type value) const noexcept;

	private:
		///
		/// OpenGL id of shader program.
		///
		unsigned int m_id;

		std::unordered_map<std::string, int> m_cache;
	};

	template<typename Type>
	inline void Shader::setUniform(const std::string& name, Type value) const noexcept
	{
	}
}

#endif