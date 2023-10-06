///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICm_RENDERER_HPP_
#define GALAXY_GRAPHICm_RENDERER_HPP_

#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/Buffer.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/PostProcess.hpp"

struct GLFWwindow;

namespace galaxy
{
	namespace graphics
	{
		///
		/// 2D OpenGL renderer.
		///
		class Renderer final
		{
		public:
			///
			/// Constructor.
			///
			Renderer();

			///
			/// Destructor.
			///
			~Renderer();

			///
			/// Update the camera uniform buffer.
			///
			/// \param camera Camera object to buffer.
			///
			void buffer_camera(Camera& camera);

			///
			/// Submit render data to renderer.
			///
			/// \param command Data from an entity to render.
			///
			void submit(RenderCommand& command);

			///
			/// Draw all submitted data to screen.
			///
			void draw();

			///
			/// Delete any data stored by the renderer.
			///
			void flush();

			///
			/// Begin post processing.
			///
			void begin_postprocessing();

			///
			/// Finalize post processing.
			///
			void end_postprocessing();

			///
			/// Render post processing data.
			///
			void render_postprocessing();

			///
			/// Prepares to render to the default framebuffer.
			///
			void prepare_default();

			///
			/// Clears currently active framebuffer.
			///
			void clear();

			///
			/// Swap backbuffer to draw to screen.
			///
			/// \param window Window backbuffer to use.
			///
			void swap_buffers(GLFWwindow* window);

			///
			/// Resize any framebuffers the renderer uses.
			///
			/// \param width Viewport width.
			/// \param height Viewport height.
			///
			void resize(const int width, const int height);

			///
			/// Draw a texture to a render texture.
			///
			/// \param target Target to draw texture to.
			/// \param texture Texture to draw to the target.
			/// \param va Vertex array object.
			/// \param transform Orthographic transform component.
			///
			void draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform);

			///
			/// Draws a texture to the active framebuffer.
			///
			/// \param texture OpenGL texture handlr.
			/// \param width Width of texture.
			/// \param height Height of texture.
			///
			void draw_texture(const unsigned int texture, const int width, const int height);

		private:
			///
			/// Uniform buffer for camera.
			///
			std::unique_ptr<UniformBuffer> m_camera_ubo;

			///
			/// 2D renderer UBO.
			///
			std::unique_ptr<UniformBuffer> m_r2d_ubo;

			///
			/// Storage for renderables submitted.
			///
			meta::vector<RenderCommand> m_data;

			///
			/// Shader used when drawing with renderer.
			///
			graphics::Shader m_r2d_shader;

			///
			/// Cache previous texture to prevent rebinding.
			///
			int m_prev_texture;

			///
			/// Cache previous nm texture to prevent rebinding.
			///
			int m_prev_nm;

			///
			/// Post processor.
			///
			graphics::PostProcess m_postprocess;

			///
			/// Renderer frame width.
			///
			int m_width;

			///
			/// Renderer frame height.
			///
			int m_height;
		};
	} // namespace graphics
} // namespace galaxy

#endif