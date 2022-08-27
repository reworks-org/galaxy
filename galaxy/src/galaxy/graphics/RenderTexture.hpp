///
/// RenderTexture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERTEXTURE_HPP_
#define GALAXY_GRAPHICS_RENDERTEXTURE_HPP_

#include <string_view>

#include <glm/mat4x4.hpp>

#include "galaxy/graphics/Framebuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Framebuffer + Render Target.
		///
		class RenderTexture final
		{
		public:
			///
			/// Constructor.
			///
			RenderTexture() noexcept;

			///
			/// \brief Argument constructor.
			///
			/// Calls create().
			///
			/// \param width Width of the render target. Must be greater than 0.
			/// \param height Height of the render target. Must be greater than 0.
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
			~RenderTexture() noexcept = default;

			///
			/// Create the RenderTexture.
			///
			/// \param width Width of the render target. Must be greater than 0.
			/// \param height Height of the render target. Must be greater than 0.
			///
			void create(int width, int height);

			///
			/// Change RenderTexture size.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			void resize(int width, int height);

			///
			/// \brief Saves texture to file on disk.
			///
			/// Depending on the size of the buffer this can be quite slow.
			///
			/// \param filepath Path and filename to save texture to. Can only save .PNG.
			///
			void save(std::string_view filepath);

			///
			/// Bind framebuffer and texture.
			///
			/// \param clear Should the framebuffer clear attachments.
			///
			void bind(const bool clear) noexcept;

			///
			/// Unbind framebuffer and texture.
			///
			void unbind() noexcept;

			///
			/// Clear framebuffer attachments.
			///
			void clear() noexcept;

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
			/// Get texture width.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_width() const noexcept;

			///
			/// Get texture height.
			///
			/// \return Integer.
			///
			[[nodiscard]] int get_height() const noexcept;

			///
			/// Get projection.
			///
			/// \return Reference to glm::mat4.
			///
			[[nodiscard]] glm::mat4& get_proj() noexcept;

			///
			/// Get texture.
			///
			/// \return OpenGL texture handle.
			///
			[[nodiscard]] unsigned int get_texture() const noexcept;

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
			/// Framebuffer to draw to.
			///
			Framebuffer m_framebuffer;

			///
			/// Projection.
			///
			glm::mat4 m_projection;
		};
	} // namespace graphics
} // namespace galaxy

#endif