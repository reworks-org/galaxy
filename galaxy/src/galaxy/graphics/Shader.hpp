///
/// Shader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_HPP_
#define GALAXY_GRAPHICS_SHADER_HPP_

#include <string>

#include <ankerl/unordered_dense.h>
#include <raylib.hpp>

#include "galaxy/math/Matrix.hpp"
#include "galaxy/math/Vector2.hpp"
#include "galaxy/math/Vector3.hpp"
#include "galaxy/math/Vector4.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
GALAXY_DISABLE_WARNING(26437)
#endif

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief GLSL Shader.
		///
		/// Only supports vertex and fragment shaders.
		///
		class Shader final : public ::Shader
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
			[[nodiscard]]
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
			bool load(const std::string& file);

			///
			/// Loads an OpenGL shader.
			///
			/// \param vertex Vertex shader.
			/// \param frag Fragment shader.
			///
			/// \return True if successful.
			///
			[[nodiscard]]
			bool load(const std::string& vertex, const std::string& frag);

			///
			/// Loads a combined raw shader.
			///
			/// \param src Raw shader code.
			///
			/// \return True if successful.
			///
			[[nodiscard]]
			bool parse(const std::string& src);

			///
			/// Loads a raw shader.
			///
			/// \param vertex Raw vertex shader code.
			/// \param frag Raw fragment shader code.
			///
			/// \return True if successful.
			///
			[[nodiscard]]
			bool parse(const std::string& vertex, const std::string& frag);

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

			///
			/// Set sampler2d shader uniform.
			///
			/// \param name The name of the uniform to set.
			/// \param sampler2d Texture sampler2d index.
			///
			void set_uniform_sampler2d(const std::string& name, const int sampler2d);

			///
			/// Retrieves the location of a shader uniform.
			///
			/// \param name Shader uniform name.
			///
			/// \return Uniform location. -1 on error.
			///
			[[nodiscard]]
			int get_uniform_location(const std::string& name);

			///
			/// Check to make sure shader is valid.
			///
			/// \return Boolean.
			///
			[[nodiscard]]
			bool valid() const noexcept;

			///
			/// Unload shader object.
			///
			void unload() noexcept;

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
			/// Extract source code from a combined shader.
			///
			/// \param src Raw source code.
			///
			/// \return True if parse was successful.
			///
			[[nodiscard]]
			bool preprocess(const std::string& src);

			///
			/// Load source into raylib.
			///
			/// \param vertex Vertex shader.
			/// \param frag Fragment shader.
			///
			/// \return True if successful.
			///
			[[nodiscard]]
			bool load_into_raylib(const std::string& vertex, const std::string& frag);

		private:
			///
			/// Cache of uniforms for better performance.
			///
			ankerl::unordered_dense::map<std::string, int> m_cache;
		};

		template<>
		inline void Shader::set_uniform(const std::string& name)
		{
			std::unreachable();
		}

		template<>
		inline void Shader::set_uniform<bool>(const std::string& name, const bool& data)
		{
			::SetShaderValue(*this, get_uniform_location(name), &data, RL_SHADER_UNIFORM_INT);
		}

		template<>
		inline void Shader::set_uniform<int>(const std::string& name, const int& data)
		{
			::SetShaderValue(*this, get_uniform_location(name), &data, RL_SHADER_UNIFORM_INT);
		}

		template<>
		inline void Shader::set_uniform<float>(const std::string& name, const float& data)
		{
			::SetShaderValue(*this, get_uniform_location(name), &data, RL_SHADER_UNIFORM_FLOAT);
		}

		template<>
		inline void Shader::set_uniform<unsigned int>(const std::string& name, const unsigned int& data)
		{
			::SetShaderValue(*this, get_uniform_location(name), &data, RL_SHADER_UNIFORM_UINT);
		}

		template<>
		inline void Shader::set_uniform<Vector2>(const std::string& name, const Vector2& data)
		{
			::SetShaderValueV(*this, get_uniform_location(name), &data, RL_SHADER_UNIFORM_VEC2, 2);
		}

		template<>
		inline void Shader::set_uniform<Vector3>(const std::string& name, const Vector3& data)
		{
			::SetShaderValueV(*this, get_uniform_location(name), &data, RL_SHADER_UNIFORM_VEC3, 3);
		}

		template<>
		inline void Shader::set_uniform<Vector4>(const std::string& name, const Vector4& data)
		{
			::SetShaderValueV(*this, get_uniform_location(name), &data, RL_SHADER_UNIFORM_VEC4, 4);
		}

		template<>
		inline void Shader::set_uniform<Matrix>(const std::string& name, const Matrix& data)
		{
			::SetShaderValueMatrix(*this, get_uniform_location(name), data);
		}

		/*template<>
		inline void Shader::set_uniform<::Texture2D>(const std::string& name, const ::Texture2D& data)
		{
			::SetShaderValueTexture(*this, get_uniform_location(name), data);
		}*/
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
