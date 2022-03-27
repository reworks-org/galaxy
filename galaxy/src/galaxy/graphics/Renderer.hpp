///
/// Renderer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERER_HPP_
#define GALAXY_GRAPHICS_RENDERER_HPP_

#include <string>

#include "galaxy/graphics/Camera.hpp"
#include "galaxy/graphics/Renderable.hpp"
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
			~Renderer() noexcept = default;

			static void buffer_camera(Camera& camera);

			static void submit(Renderable* renderable);
			static void draw();

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
			static UniformBuffer m_ubo;

			///
			/// Storage for renderables submitted.
			///
			static std::vector<Renderable*> m_data;
		};
	} // namespace graphics
} // namespace galaxy

#endif