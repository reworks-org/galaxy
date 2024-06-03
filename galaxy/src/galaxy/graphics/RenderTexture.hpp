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

#include "galaxy/graphics/gl/Framebuffer.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Draw to an opengl texture instead of the screen.
		///
		class RenderTexture final
		{
		  public:
			///
			/// Constructor.
			///
			RenderTexture();

			///
			/// Move constructor.
			///
			RenderTexture(RenderTexture&&);

			///
			/// Move assignment operator.
			///
			RenderTexture& operator=(RenderTexture&&);

			///
			/// Destructor.
			///
			virtual ~RenderTexture();

			///
			/// Create framebuffer and texture.
			///
			/// \param width Width of the render target. Must be greater than 0.
			/// \param height Height of the render target. Must be greater than 0.
			///
			void create(const int width, const int height);

			///
			/// \brief Destroy and re-create framebuffer.
			///
			/// You will need to mark it bindless again.
			///
			/// \param width Optional. Will use existing size otherwise. Width of the render target. Must be greater than 0.
			/// \param height Optional. Will use existing size otherwise. Height of the render target. Must be greater than 0.
			///
			void recreate(const int width = -1, const int height = -1);

			///
			/// Saves texture to file on disk.
			///
			/// \param file Path and filename to save texture to. Does not need extension (it will be ignored).
			///
			void save(std::string_view file);

			///
			/// Activate context.
			///
			/// \param clear Clear framebuffer after binding.
			///
			void bind(bool clear = true);

			///
			/// Deactivate context.
			///
			void unbind();

			///
			/// Clear framebuffer attachments.
			///
			void clear();

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as int.
			///
			[[nodiscard]] int width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as int.
			///
			[[nodiscard]] int height() const;

			///
			/// Gets framebuffer texture.
			///
			/// \return Texture ID.
			///
			[[nodiscard]] unsigned int texture() const;

			///
			/// Get render texture projection.
			///
			/// \return Const reference to framebuffer viewport projection.
			///
			[[nodiscard]] const glm::mat4& get_proj();

			///
			/// Get framebuffer.
			///
			/// \return Reference to framebuffer.
			///
			[[nodiscard]] Framebuffer& fbo();

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
			/// Cached width.
			///
			int m_width;

			///
			/// Cached height.
			///
			int m_height;

			///
			/// OpenGL framebuffer abstraction.
			///
			Framebuffer m_framebuffer;

			///
			/// Projection.
			///
			glm::mat4 m_proj;
		};
	} // namespace graphics
} // namespace galaxy

#endif
