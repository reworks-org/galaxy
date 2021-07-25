///
/// AppLogo.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_CORE_APPLOGO_HPP_
#define GALAXY_CORE_APPLOGO_HPP_

#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/VertexArray.hpp"

struct GLFWwindow;

namespace galaxy
{
	namespace core
	{
		class AppLogo final
		{
		public:
			AppLogo() noexcept;
			~AppLogo() noexcept;

			void load(std::string_view texture);
			void display(const float width, const float height, GLFWwindow* window);

			///
			/// Sleeps thread from the difference between display() and wait().
			///
			/// \param milliseconds Time to wait, defaulting to 2 seconds.
			///
			void wait(const float milliseconds = 2000) noexcept;

		private:
			graphics::Shader m_shader;
			graphics::Texture m_texture;
			graphics::VertexArray m_vao;
			std::chrono::high_resolution_clock::time_point m_displayed_at;
		};

	} // namespace core
} // namespace galaxy

#endif