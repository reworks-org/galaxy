///
/// Shader.hpp
/// starlight
///
/// Created by reworks on 14/10/2017.
/// MIT License.
/// Refer to LICENSE.txt for more details.
///

#ifndef STARLIGHT_SHADER_HPP_
#define STARLIGHT_SHADER_HPP_

#include <allegro5/shader.h>

#include "sl/libs/sol2/sol_forward.hpp"

namespace sl
{
	///
	/// Represents a shader object.
	/// Can be GLSL or HLSL.
	/// Wrapper for Allegro's shader structures.
	/// http://liballeg.org/a5docs/trunk/shader.html
	///
	class Shader final
	{
	public:
		///
		/// \brief Lua Constructor.
		///
		/// Platform is AUTO by default, so you can create GLSL or HLSL shaders.
		///
		/// \param table sol::table containing data.
		///
		explicit Shader(const sol::table& table);

		///
		/// Argument Constructor.
		///
		/// \param platform Which type of Shader it is (HLSL, GLSL, or AUTO).
		/// \param vertexShader File containing vertex shader code in the VFS.
		/// \param pixelShader File containing pixel shader code in the VFS.
		///
		Shader(const ALLEGRO_SHADER_PLATFORM platform, const std::string& vertexShader, const std::string& pixelShader);

		///
		/// Destructor.
		///
		~Shader() noexcept;

		///
		/// \brief Use the shader.
		///
		/// Uses the shader for subsequent drawing operations on the current target bitmap i.e window.
		///
		void use() const;

		///
		/// Removes the shader from the current drawing target.
		///
		void disable() const;

	private:
		///
		/// Default constructor.
		/// Deleted.
		///
		Shader() = delete;

	private:
		///
		/// Allegro shader object this class wraps around.
		///
		ALLEGRO_SHADER* m_shader;
	};
}

#endif