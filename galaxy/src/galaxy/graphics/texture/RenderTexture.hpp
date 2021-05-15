///
/// RenderTexture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_RENDERTEXTURE_HPP_
#define GALAXY_GRAPHICS_TEXTURE_RENDERTEXTURE_HPP_

#include <glm/gtc/matrix_transform.hpp>

#include "galaxy/graphics/texture/BaseTexture.hpp"

namespace galaxy
{
	namespace core
	{
		class Window;

	} // namespace core

	namespace graphics
	{
		///
		/// Framebuffer texture. You render to this texture instead of the window.
		///
		class RenderTexture final : public BaseTexture
		{
		public:
			///
			/// Constructor.
			///
			RenderTexture() noexcept;

			///
			/// Argument constructor.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			RenderTexture(const int width, const int height);

			///
			/// Move constructor.
			///
			RenderTexture(RenderTexture&&) noexcept;

			///
			/// Move assignment operator.
			///
			RenderTexture& operator=(RenderTexture&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~RenderTexture() noexcept;

			///
			/// Create the RenderTexture.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			void create(const int width, const int height);

			///
			/// Change RenderTexture size.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			void resize(const int width, const int height);

			///
			/// Toggle framebuffer clearing.
			///
			/// \param clear Flag to clear framebuffers existing contents.
			///
			void toggle_clearing(const bool clear);

			///
			/// Manually clear framebuffer.
			///
			void clear_framebuffer() noexcept;

			///
			/// Activate texture context.
			///
			void bind() noexcept override;

			///
			/// Deactivate texture context.
			///
			void unbind() noexcept override;

			///
			/// Modify projection of render texture.
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			///
			void set_projection(const float left, const float right, const float bottom, const float top) noexcept;

			///
			/// Get projection.
			///
			/// \return Const reference to glm::mat4.
			///
			[[nodiscard]] const glm::mat4& get_proj() noexcept;

		private:
			///
			/// Copy constructor.
			///
			RenderTexture(const RenderTexture&) = delete;

			///
			/// Copy assignment operator.
			///
			RenderTexture& operator=(const RenderTexture&) = delete;

		private:
			///
			/// Clear flag.
			///
			bool m_clear;

			///
			/// Projection.
			///
			glm::mat4 m_projection;

			///
			/// OpenGL framebuffer handle.
			///
			unsigned int m_framebuffer;

			///
			/// OpenGL renderbuffer handle.
			///
			unsigned int m_renderbuffer;
		};
	} // namespace graphics
} // namespace galaxy

#endif