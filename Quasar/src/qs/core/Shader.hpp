///
/// Shader.hpp
/// Quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_SHADER_HPP_
#define QUASAR_SHADER_HPP_

#include "qs/utils/Result.hpp"

namespace qs
{
	///
	/// Represents the source of a shader.
	///
	struct ShaderSource
	{
		///
		/// The type of the shader. Vertex, fragment, etc.
		///
		GLenum m_type;

		///
		/// The source code of the shader.
		///
		std::string m_source;
	};

	///
	/// Class for managing OpenGL shader programs.
	///
	class Shader
	{
	public:
		///
		/// Represents the type of the shader.
		///
		enum class Type
		{
			VERTEX,
			FRAGMENT,
			GEOMETRY
		};

		Shader();
		~Shader();

		qs::Result load(const std::string& source, qs::Shader::Type type);

		void use();

	private:
		qs::Shader::Type m_type;
		unsigned int m_id;
	};
}

#endif