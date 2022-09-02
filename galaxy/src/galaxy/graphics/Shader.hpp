///
/// Shader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_SHADER_HPP_
#define GALAXY_GRAPHICS_SHADER_HPP_

#include <array>
#include <string_view>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json_fwd.hpp>
#include <robin_hood.h>

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/graphics/UniformInfo.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Class for managing OpenGL shader programs.
		///
		class Shader final
		{
		public:
			///
			/// Constructor.
			///
			Shader() noexcept;

			///
			/// Path Constructor.
			///
			/// \param vertex_file Path to vertex shader source code.
			/// \param frag_file Path to fragment shader source code.
			///
			Shader(std::string_view vertex_file, std::string_view frag_file);

			///
			/// JSON Constructor.
			///
			/// \param json JSON defining object.
			///
			Shader(const nlohmann::json& json);

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
			/// Loads a shader into OpenGL from source and sets up the shader program.
			///
			/// \param vertex_file Path to vertex shader source code.
			/// \param fragment_file Path to fragment shader source code.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool load_file(std::string_view vertex_file, std::string_view fragment_file);

			///
			/// Loads a shader into OpenGL from raw strings and sets up the shader program.
			///
			/// \param vertex_str Raw vertex shader code.
			/// \param fragment_str Raw fragment shader code.
			///
			/// \return True if successful.
			///
			[[maybe_unused]] bool load_raw(const std::string& vertex_str, const std::string& fragment_str);

			///
			/// Compiles shader into OpenGL.
			///
			void compile();

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
			template<typename... Uniforms>
			void set_uniform(const std::string& name, const Uniforms&... args);

			///
			/// Retrieves the location of a shader uniform.
			///
			/// \param name Shader uniform name.
			///
			/// \return Uniform location. -1 on error.
			///
			[[nodiscard]] GLint get_uniform_location(const std::string& name);

			///
			/// Retrieves the count of a shader uniform.
			///
			/// \param name Shader uniform name.
			///
			/// \return Uniform count. -1 on error.
			///
			[[nodiscard]] GLint get_uniform_count(const std::string& name);

			///
			/// Retrieves all info about a shader uniform.
			///
			/// \param name Shader uniform name.
			///
			/// \return Uniform info as an optional reference.
			///
			[[nodiscard]] meta::OptionalRef<UniformInfo> get_uniform_info(const std::string& name);

			///
			/// Get program id.
			///
			/// \return unsigned int.
			///
			[[nodiscard]] unsigned int id() const noexcept;

		private:
			///
			/// Copy constructor.
			///
			Shader(const Shader&) = delete;

			///
			/// Copy assignment operator.
			///
			Shader& operator=(const Shader&) = delete;

		private:
			///
			/// OpenGL program id.
			///
			unsigned int m_id;

			///
			/// Cache of uniforms for better performance.
			///
			robin_hood::unordered_flat_map<std::string, UniformInfo> m_cache;

			///
			/// \brief Vertex source.
			///
			/// Destroyed once shader is compiled.
			///
			std::unique_ptr<std::string> m_vertex_src;

			///
			/// \brief Fragment source.
			///
			/// Destroyed once shader is compiled.
			///
			std::unique_ptr<std::string> m_fragment_src;
		};

		template<>
		inline void Shader::set_uniform<bool>(const std::string& name, const bool& a)
		{
			glProgramUniform1i(m_id, get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<int>(const std::string& name, const int& a)
		{
			glProgramUniform1i(m_id, get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<int, int>(const std::string& name, const int& a, const int& b)
		{
			glProgramUniform2i(m_id, get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<int, int, int>(const std::string& name, const int& a, const int& b, const int& c)
		{
			glProgramUniform3i(m_id, get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<int, int, int, int>(const std::string& name, const int& a, const int& b, const int& c, const int& d)
		{
			glProgramUniform4i(m_id, get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<unsigned int>(const std::string& name, const unsigned int& a)
		{
			glProgramUniform1ui(m_id, get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int>(const std::string& name, const unsigned int& a, const unsigned int& b)
		{
			glProgramUniform2ui(m_id, get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int>(const std::string& name,
			const unsigned int& a,
			const unsigned int& b,
			const unsigned int& c)
		{
			glProgramUniform3ui(m_id, get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int, unsigned int>(const std::string& name,
			const unsigned int& a,
			const unsigned int& b,
			const unsigned int& c,
			const unsigned int& d)
		{
			glProgramUniform4ui(m_id, get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<float>(const std::string& name, const float& a)
		{
			glProgramUniform1f(m_id, get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<float, float>(const std::string& name, const float& a, const float& b)
		{
			glProgramUniform2f(m_id, get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<float, float, float>(const std::string& name, const float& a, const float& b, const float& c)
		{
			glProgramUniform3f(m_id, get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<float, float, float, float>(const std::string& name, const float& a, const float& b, const float& c, const float& d)
		{
			glProgramUniform4f(m_id, get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<double>(const std::string& name, const double& a)
		{
			glProgramUniform1d(m_id, get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<double, double>(const std::string& name, const double& a, const double& b)
		{
			glProgramUniform2d(m_id, get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<double, double, double>(const std::string& name, const double& a, const double& b, const double& c)
		{
			glProgramUniform3d(m_id, get_uniform_location(name), a, b, c);
		}

		template<>
		inline void
		Shader::set_uniform<double, double, double, double>(const std::string& name, const double& a, const double& b, const double& c, const double& d)
		{
			glProgramUniform4d(m_id, get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<glm::mat3>(const std::string& name, const glm::mat3& a)
		{
			glProgramUniformMatrix3fv(m_id, get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a));
		}

		template<>
		inline void Shader::set_uniform<glm::mat4>(const std::string& name, const glm::mat4& a)
		{
			glProgramUniformMatrix4fv(m_id, get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a));
		}

		template<>
		inline void Shader::set_uniform<glm::vec2>(const std::string& name, const glm::vec2& a)
		{
			glProgramUniform2f(m_id, get_uniform_location(name), a.x, a.y);
		}

		template<>
		inline void Shader::set_uniform<glm::vec3>(const std::string& name, const glm::vec3& a)
		{
			glProgramUniform3f(m_id, get_uniform_location(name), a.x, a.y, a.z);
		}

		template<>
		inline void Shader::set_uniform<glm::vec4>(const std::string& name, const glm::vec4& a)
		{
			glProgramUniform4f(m_id, get_uniform_location(name), a.x, a.y, a.z, a.w);
		}

		template<>
		inline void Shader::set_uniform<std::array<float, 4>>(const std::string& name, const std::array<float, 4>& a)
		{
			glProgramUniform4f(m_id, get_uniform_location(name), a[0], a[1], a[2], a[3]);
		}

		template<>
		inline void Shader::set_uniform<graphics::Colour>(const std::string& name, const graphics::Colour& a)
		{
			glProgramUniform4i(m_id, get_uniform_location(name), a.m_red, a.m_green, a.m_blue, a.m_alpha);
		}
	} // namespace graphics
} // namespace galaxy

#endif