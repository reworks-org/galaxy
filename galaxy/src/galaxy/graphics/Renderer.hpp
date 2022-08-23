///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include <string>

#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Renderable.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/Texture.hpp"
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
			~Renderer() noexcept = default;

			static void buffer_camera(Camera& camera) noexcept;

			static void submit(Renderable* renderable);
			static void draw();
			static void draw_texture_to_target(RenderTexture& target, Texture& texture, VertexArray& va, components::Transform& transform);

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
			static void init() noexcept;

			///
			/// Destroy renderer resources.
			///
			static void destroy() noexcept;

		private:
			///
			/// Uniform buffer for camera and other general shader data.
			///
			static std::unique_ptr<UniformBuffer> s_ubo;

			///
			/// Storage for renderables submitted.
			///
			static std::vector<Renderable*> s_data;

			///
			/// Cache previous shader to prevent rebinding.
			///
			static int s_prev_shader;

			///
			/// Cache previous texture to prevent rebinding.
			///
			static int s_prev_texture;
		};
	} // namespace graphics
} // namespace galaxy

#endif