///
/// Shader.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_SHADER_HPP_
#define GALAXY_GRAPHICS_GL_SHADER_HPP_

#include <ankerl/unordered_dense.h>
#include <entt/core/compressed_pair.hpp>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <nlohmann/json_fwd.hpp>

#include "galaxy/graphics/Colour.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief OpenGL Shader Program.
		///
		/// Currently only supports vertex and fragment shaders.
		///
		class Shader final
		{
		  public:
			///
			/// Constructor.
			///
			Shader();

			///
			/// JSON Constructor.
			///
			/// \param json JSON defining object.
			///
			Shader(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			Shader(Shader&&);

			///
			/// Move assignment operator.
			///
			Shader& operator=(Shader&&);

			///
			/// Destructor.
			///
			~Shader();

			///
			/// Loads a combined shader.
			///
			/// \param file Path to a combined shader source.
			///
			/// \return True if successful.
			///
			[[nodiscard]] bool load(const std::string& file);

			///
			/// Loads an OpenGL shader.
			///
			/// \param vertex Vertex shader.
			/// \param frag Fragment shader.
			///
			/// \return True if successful.
			///
			[[nodiscard]] bool load(const std::string& vertex, const std::string& frag);

			///
			/// Loads a combined raw shader.
			///
			/// \param src Raw shader code.
			///
			/// \return True if successful.
			///
			[[nodiscard]] bool parse(const std::string& src);

			///
			/// Loads a raw shader.
			///
			/// \param vertex Raw vertex shader code.
			/// \param frag Raw fragment shader code.
			///
			/// \return True if successful.
			///
			[[nodiscard]] bool parse(const std::string& vertex, const std::string& frag);

			///
			/// Compiles shader into GPU mem.
			///
			void compile();

			///
			/// Destroys shader program.
			///
			void destroy();

			///
			/// Make active shader.
			///
			void bind() const;

			///
			/// Unbind.
			///
			void unbind() const;

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
			/// Get program id.
			///
			/// \return unsigned int.
			///
			[[nodiscard]] unsigned int id() const;

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
			[[nodiscard]] bool preprocess(const std::string& src);

			///
			/// Retrieves the location of a shader uniform.
			///
			/// \param name Shader uniform name.
			///
			/// \return Uniform location. -1 on error.
			///
			[[nodiscard]] int get_uniform_location(const std::string& name);

		  private:
			///
			/// OpenGL handle.
			///
			unsigned int m_id;

			///
			/// Cache of uniforms for better performance.
			///
			ankerl::unordered_dense::map<std::string, int> m_cache;

			///
			/// Shader source code.
			///
			entt::compressed_pair<std::string, std::string> m_src;
		};

		template<>
		inline void Shader::set_uniform(const std::string& name)
		{
			std::unreachable();
		}

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
			const unsigned int&                                                                      a,
			const unsigned int&                                                                      b,
			const unsigned int&                                                                      c)
		{
			glProgramUniform3ui(m_id, get_uniform_location(name), a, b, c);
		}

		template<>
		inline void Shader::set_uniform<unsigned int, unsigned int, unsigned int, unsigned int>(const std::string& name,
			const unsigned int&                                                                                    a,
			const unsigned int&                                                                                    b,
			const unsigned int&                                                                                    c,
			const unsigned int&                                                                                    d)
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
		inline void Shader::set_uniform<std::uint64_t>(const std::string& name, const std::uint64_t& a)
		{
			glProgramUniform1ui64ARB(m_id, get_uniform_location(name), a);
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
			set_uniform(name, a.vec4());
		}
	} // namespace graphics
} // namespace galaxy

#endif
