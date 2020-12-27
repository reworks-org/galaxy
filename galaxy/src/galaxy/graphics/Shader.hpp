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

#include "galaxy/error/Log.hpp"

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
			/// Default constructor.
			///
			Shader();

			///
			/// Path constructor.
			///
			/// \param vertex_file Path to vertex shader source code.
			/// \param frag_file Path to fragment shader source code.
			///
			Shader(std::string_view vertex_file, std::string_view frag_file);

			///
			/// JSON constructor.
			///
			/// \param json JSON defining object.
			///
			Shader(const nlohmann::json& json);

			///
			/// Copy constructor.
			///
			Shader(const Shader&);

			///
			/// Move constructor.
			///
			Shader(Shader&&);

			///
			/// Copy assignment operator.
			///
			Shader& operator=(const Shader&);

			///
			/// Move assignment operator.
			///
			Shader& operator=(Shader&&);

			///
			/// Destructor.
			///
			~Shader();

			///
			/// Loads a shader into OpenGL from source and sets up the shader program.
			///
			/// \param vertex_file Path to vertex shader source code.
			/// \param fragment_file Path to fragment shader source code.
			///
			/// \return boolean True if successful.
			///
			[[maybe_unused]] bool load_path(std::string_view vertex_file, std::string_view fragment_file);

			///
			/// Loads a shader into OpenGL from raw strings and sets up the shader program.
			///
			/// \param vertex_str Raw vertex shader code.
			/// \param fragment_str Raw fragment shader code.
			///
			/// \return boolean True if successful.
			///
			[[maybe_unused]] bool load_raw(const std::string& vertex_str, const std::string& fragment_str);

			///
			/// Enable this shader for rendering.
			///
			void bind();

			///
			/// Disable this currently active shader.
			///
			void unbind();

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

			///
			/// Query if shader has been loaded.
			///
			/// \return Const bool. True if loaded.
			///
			[[nodiscard]] const bool is_loaded() const;

		private:
			///
			/// \brief Retrieves location of shader uniform from GPU.
			///
			/// In a seperate function because it checks interal cache first.
			///
			int get_uniform_location(std::string_view name);

		private:
			///
			/// OpenGL id of shader program.
			///
			unsigned int m_id;

			///
			/// Cache of uniforms for better performance.
			///
			robin_hood::unordered_map<std::string, int> m_cache;

			///
			/// Boolean keeping track if shader is loaded.
			///
			bool m_loaded;
		};

		template<typename... Uniforms>
		inline void Shader::set_uniform(std::string_view name, const Uniforms&... args)
		{
			// If type does not have specialization, throw error.
			GALAXY_LOG(GALAXY_ERROR, "Invalid shader uniform type!");
		}

		template<>
		inline void Shader::set_uniform<int>(std::string_view name, const int& a)
		{
			glUniform1i(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<int, int>(std::string_view name, const int& a, const int& b)
		{
			glUniform2i(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<int, int, int>(std::string_view name, const int& a, const int& b, const int& c)
		{
			glUniform3i(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<int, int, int, int>(std::string_view name, const int& a, const int& b, const int& c, const int& d)
		{
			glUniform4i(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<unsigned int>(std::string_view name, const unsigned int& a)
		{
			glUniform1ui(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int>(std::string_view name, const unsigned int& a, const unsigned int& b)
		{
			glUniform2ui(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int>(std::string_view name, const unsigned int& a, const unsigned int& b, const unsigned int& c)
		{
			glUniform3ui(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int, unsigned int>(std::string_view name, const unsigned int& a, const unsigned int& b, const unsigned int& c, const unsigned int& d)
		{
			glUniform4ui(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<float>(std::string_view name, const float& a)
		{
			glUniform1f(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<float, float>(std::string_view name, const float& a, const float& b)
		{
			glUniform2f(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<float, float, float>(std::string_view name, const float& a, const float& b, const float& c)
		{
			glUniform3f(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<float, float, float, float>(std::string_view name, const float& a, const float& b, const float& c, const float& d)
		{
			glUniform4f(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<double>(std::string_view name, const double& a)
		{
			glUniform1d(get_uniform_location(name), a);
		}

		template<>
		inline void Shader::set_uniform<double, double>(std::string_view name, const double& a, const double& b)
		{
			glUniform2d(get_uniform_location(name), a, b);
		}

		template<>
		inline void Shader::set_uniform<double, double, double>(std::string_view name, const double& a, const double& b, const double& c)
		{
			glUniform3d(get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<double, double, double, double>(std::string_view name, const double& a, const double& b, const double& c, const double& d)
		{
			glUniform4d(get_uniform_location(name), a, b, c, d);
		}

		template<>
		inline void Shader::set_uniform<glm::mat4>(std::string_view name, const glm::mat4& transform)
		{
			glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(transform));
		}

		template<>
		inline void Shader::set_uniform<glm::vec2>(std::string_view name, const glm::vec2& a)
		{
			glUniform2f(get_uniform_location(name), a.x, a.y);
		}

		template<>
		inline void Shader::set_uniform<glm::vec3>(std::string_view name, const glm::vec3& a)
		{
			glUniform3f(get_uniform_location(name), a.x, a.y, a.z);
		}

		template<>
		inline void Shader::set_uniform<glm::vec4>(std::string_view name, const glm::vec4& a)
		{
			glUniform4f(get_uniform_location(name), a.x, a.y, a.z, a.w);
		}

		template<>
		inline void Shader::set_uniform<std::array<float, 4>>(std::string_view name, const std::array<float, 4>& a)
		{
			glUniform4f(get_uniform_location(name), a[0], a[1], a[2], a[3]);
		}
	} // namespace graphics
} // namespace galaxy

#endif