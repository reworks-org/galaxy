///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/RenderCommand.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/Shader.hpp"
#include "galaxy/graphics/UniformBuffer.hpp"
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
			///
			///
			static void buffer_camera(Camera& camera);

			///
			///
			///
			static void submit(RenderCommand& command);

			///
			///
			///
			static void draw();

			///
			///
			///
			static void flush();

			///
			///
			///
			static void draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform);

			///
			///
			///
			static void draw_texture_to_framebuffer(const unsigned int texture, VertexArray& va, components::Transform& transform, const glm::mat4& proj);

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
			///
			///
			static graphics::Shader s_r2d_shader;

			///
			/// Cache previous texture to prevent rebinding.
			///
			static int s_prev_texture;
		};
	} // namespace graphics
} // namespace galaxy

#endif