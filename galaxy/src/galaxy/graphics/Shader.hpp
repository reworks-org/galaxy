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
			/// \return Uniform info.
			///
			[[nodiscard]] std::optional<UniformInfo> get_uniform_info(const std::string& name);

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
		};

		template<typename... Uniforms>
		inline void Shader::set_uniform(const std::string& name, const Uniforms&... args)
		{
			// If type does not have specialization, throw error.
			static_assert(false, "Invalid shader uniform type!");
		}

		template<>
		inline void Shader::set_uniform<bool>(const std::string& name, const bool& a)
		{
			glUniform1i(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<int>(const std::string& name, const int& a)
		{
			glUniform1i(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<int, int>(const std::string& name, const int& a, const int& b)
		{
			glUniform2i(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<int, int, int>(const std::string& name, const int& a, const int& b, const int& c)
		{
			glUniform3i(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<int, int, int, int>(const std::string& name, const int& a, const int& b, const int& c, const int& d)
		{
			glUniform4i(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<unsigned int>(const std::string& name, const unsigned int& a)
		{
			glUniform1ui(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int>(const std::string& name, const unsigned int& a, const unsigned int& b)
		{
			glUniform2ui(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int>(const std::string& name,
			const unsigned int& a,
			const unsigned int& b,
			const unsigned int& c)
		{
			glUniform3ui(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int, unsigned int>(const std::string& name,
			const unsigned int& a,
			const unsigned int& b,
			const unsigned int& c,
			const unsigned int& d)
		{
			glUniform4ui(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<float>(const std::string& name, const float& a)
		{
			glUniform1f(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<float, float>(const std::string& name, const float& a, const float& b)
		{
			glUniform2f(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<float, float, float>(const std::string& name, const float& a, const float& b, const float& c)
		{
			glUniform3f(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<float, float, float, float>(const std::string& name, const float& a, const float& b, const float& c, const float& d)
		{
			glUniform4f(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<double>(const std::string& name, const double& a)
		{
			glUniform1d(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<double, double>(const std::string& name, const double& a, const double& b)
		{
			glUniform2d(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<double, double, double>(const std::string& name, const double& a, const double& b, const double& c)
		{
			glUniform3d(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void
		Shader::set_uniform<double, double, double, double>(const std::string& name, const double& a, const double& b, const double& c, const double& d)
		{
			glUniform4d(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<glm::mat3>(const std::string& name, const glm::mat3& a)
		{
			glUniformMatrix3fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a));
		}

		template<>
		inline void Shader::set_uniform<glm::mat4>(const std::string& name, const glm::mat4& a)
		{
			glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(a));
		}

		template<>
		inline void Shader::set_uniform<glm::vec2>(const std::string& name, const glm::vec2& a)
		{
			glUniform2f(get_uniform_location(name), a.x, a.y);
		}

		template<>
		inline void Shader::set_uniform<glm::vec3>(const std::string& name, const glm::vec3& a)
		{
			glUniform3f(get_uniform_location(name), a.x, a.y, a.z);
		}

		template<>
		inline void Shader::set_uniform<glm::vec4>(const std::string& name, const glm::vec4& a)
		{
			glUniform4f(get_uniform_location(name), a.x, a.y, a.z, a.w);
		}

		template<>
		inline void Shader::set_uniform<std::array<float, 4>>(const std::string& name, const std::array<float, 4>& a)
		{
			glUniform4f(get_uniform_location(name), a[0], a[1], a[2], a[3]);
		}

		template<>
		inline void Shader::set_uniform<graphics::Colour>(const std::string& name, const graphics::Colour& a)
		{
			glUniform4i(get_uniform_location(name), a.m_red, a.m_green, a.m_blue, a.m_alpha);
		}
	} // namespace graphics
} // namespace galaxy

#endif