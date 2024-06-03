///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include "galaxy/events/WindowResized.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/gl/Shader.hpp"
#include "galaxy/graphics/gl/ShaderStorageBuffer.hpp"
#include "galaxy/graphics/gl/Texture2D.hpp"
#include "galaxy/graphics/gl/VertexArray.hpp"
#include "galaxy/graphics/PostProcess.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/shapes/Shape.hpp"
#include "galaxy/graphics/text/Text.hpp"
#include "galaxy/graphics/Transform.hpp"
#include "galaxy/meta/Memory.hpp"

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
			/// Get reference to renderer singleton.
			///
			/// \return Static reference.
			///
			[[nodiscard]] static Renderer& ref();

			///
			/// Initialize renderer.
			///
			void init();

			///
			/// Cleanup renderer data.
			///
			void destroy();

			///
			/// Event processing method for window size change.
			///
			/// \param e Takes in a window resized event.
			///
			void on_window_resized(const events::WindowResized& e);

			///
			/// Set the camera to use when calling draw().
			///
			/// \param camera Camera object to buffer.
			///
			void submit_camera(Camera& camera);

			///
			/// Add an entity rendering command.
			///
			/// \param command Rendering command to draw.
			///
			void submit_cmd(RenderCommand& command);

			///
			/// Submit a standalone texture to be rendered.
			///
			/// \param texture Texture to use when drawing.
			/// \param va Vertex data to use when drawing.
			/// \param tf Object transformation.
			/// \param layer Rendering z-level layer.
			/// \param opacity Object opacity.
			///
			void submit_texture(const Texture2D& texture, VertexArray& va, Transform& tf, const int layer, const float opacity);

			///
			/// Submit standalone text to be rendered.
			///
			/// \param text Text to draw.
			/// \param tf Object transformation.
			/// \param layer Rendering z-level layer.
			///
			void submit_text(Text& text, Transform& tf, const int layer);

			///
			/// Submit a standalone shape to be rendered.
			///
			/// \param shape Polymorphic shape to render.
			/// \param tf Object transformation.
			/// \param layer Rendering z-level layer.
			///
			void submit_shape(Shape* shape, Transform& tf, const int layer);

			///
			/// \brief Draw a texture to a render texture.
			///
			/// Standalone function.
			///
			/// \param target Target to draw texture to.
			/// \param texture Texture to draw to the target.
			/// \param va Vertex array object.
			/// \param transform Orthographic transform.
			///
			void draw_texture_to_target(RenderTexture& target, Texture* texture, VertexArray& va, Transform& tf);

			///
			/// Draw all submitted render commands to screen.
			///
			void draw();

			///
			/// Deletes all submitted render commands.
			///
			void flush();

			///
			/// Clears currently active framebuffer.
			///
			void clear_active();

			///
			/// Begin rendering to post process framebuffer.
			///
			void begin_post();

			///
			/// Renders effects to post processing framebuffer and rebinds to default framebuffer.
			///
			void end_post();

			///
			/// Renders final post processing output to active framebuffer.
			///
			void render_post();

			///
			/// \brief Start rendering to default framebuffer.
			///
			/// This should be called at the end of rendering, only rendering your final framebuffer, since afterwards we swap buffers.
			///
			void begin_default();

			///
			/// Swaps buffers.
			///
			void end_default();

		  private:
			///
			/// Constructor.
			///
			Renderer();

			///
			/// Destructor.
			///
			~Renderer() = default;

			///
			/// Need to recalculate default framebuffer viewport.
			///
			/// \param window_width New window width.
			/// \param window_height New window height.
			void calc_viewport(int window_width, int window_height);

		  private:
			///
			/// List of renderables to draw.
			///
			meta::vector<RenderCommand> m_cmds;

			///
			/// Mono render shader.
			///
			Shader m_r2d_shader;

			///
			/// Camera buffer storage.
			///
			ShaderStorageBuffer m_camera;

			///
			/// Uniform buffer storage.
			///
			ShaderStorageBuffer m_renderdata;

			///
			/// Post processor.
			///
			PostProcess m_post;

			///
			/// Default framebuffer viewport.
			///
			glm::vec4 m_viewport;
		};
	} // namespace graphics
} // namespace galaxy

#endif
