///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/Buffer.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace core
	{
		class Window;
	} // namespace core

	namespace graphics
	{
		///
		/// 2D OpenGL renderer.
		///
		class Renderer final
		{
			friend class core::Window;

		public:
			///
			/// Destructor.
			///
			~Renderer() = default;

			///
			/// Update the camera uniform buffer.
			///
			/// \param camera Camera object to buffer.
			///
			static void buffer_camera(Camera& camera);

			///
			/// Submit render data to renderer.
			///
			/// \param command Data from an entity to render.
			///
			static void submit(RenderCommand& command);

			///
			/// Draw all submitted data to screen.
			///
			static void draw();

			///
			/// Delete any data stored by the renderer.
			///
			static void flush();

			///
			/// Draw a texture to a render texture.
			///
			/// \param target Target to draw texture to.
			/// \param texture Texture to draw to the target.
			/// \param va Vertex array object.
			/// \param transform Orthographic transform component.
			///
			static void draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform);

			///
			/// Draws a texture to the active framebuffer.
			///
			/// \param texture OpenGL texture handlr.
			/// \param width Width of texture.
			/// \param height Height of texture.
			///
			static void draw_texture(const unsigned int texture, const int width, const int height);

		private:
			///
			/// \brief Constructor.
			///
			/// Deleted.
			///
			Renderer() = delete;

			///
			/// Initialize renderer resources.
			///
			static void init();

			///
			/// Destroy renderer resources.
			///
			static void destroy();

		private:
			///
			/// Uniform buffer for camera.
			///
			static std::unique_ptr<UniformBuffer> s_camera_ubo;

			///
			/// 2D renderer UBO.
			///
			static std::unique_ptr<UniformBuffer> s_r2d_ubo;

			///
			/// Storage for renderables submitted.
			///
			static meta::vector<RenderCommand> s_data;

			///
			/// Shader used when drawing with renderer.
			///
			static graphics::Shader s_r2d_shader;

			///
			/// Cache previous texture to prevent rebinding.
			///
			static int s_prev_texture;

			///
			/// Cache previous nm texture to prevent rebinding.
			///
			static int s_prev_nm;
		};
	} // namespace graphics
} // namespace galaxy

#endif