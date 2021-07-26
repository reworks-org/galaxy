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
		///
		/// Displays an image during app startup.
		///
		class AppLogo final
		{
		public:
			///
			/// Constructor.
			///
			AppLogo() noexcept;

			///
			/// Destructor.
			///
			~AppLogo() noexcept = default;

			///
			/// Load logo.
			///
			/// \param texture Texture file in the VFS.
			///
			void load(std::string_view texture);

			///
			/// Display app logo to screen.
			///
			/// \param width Screen width.
			/// \param height Screen height.
			/// \param window Pointer to GLFW window.
			///
			void display(const float width, const float height, GLFWwindow* window);

			///
			/// Sleeps thread from the difference between display() and wait().
			///
			/// \param milliseconds Time to wait, defaulting to 2 seconds.
			///
			void wait(const float milliseconds = 2000) noexcept;

		private:
			///
			/// Shader.
			///
			graphics::Shader m_shader;

			///
			/// Texture.
			///
			graphics::Texture m_texture;

			///
			/// Vertex Array Object.
			///
			graphics::VertexArray m_vao;

			///
			/// Time point for when display() is called.
			///
			std::chrono::high_resolution_clock::time_point m_displayed_at;
		};

	} // namespace core
} // namespace galaxy

#endif