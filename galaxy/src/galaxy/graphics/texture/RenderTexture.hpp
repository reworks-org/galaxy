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
#include "galaxy/graphics/texture/TextureFilters.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Forward dec.
		///
		class Window;

		///
		/// Framebuffer texture. You render to this texture instead of the window.
		///
		class RenderTexture final : public BaseTexture
		{
		public:
			///
			/// Default constructor.
			///
			RenderTexture();

			///
			/// Argument constructor.
			///
			/// \param width Width of the RenderTexture.
			/// \param height Height of the RenderTexture.
			///
			explicit RenderTexture(const int width, const int height);

			///
			/// Copy constructor.
			///
			RenderTexture(const RenderTexture&) = delete;

			///
			/// Move constructor.
			///
			RenderTexture(RenderTexture&&);

			///
			/// Copy assignment operator.
			///
			RenderTexture& operator=(const RenderTexture&) = delete;

			///
			/// Move assignment operator.
			///
			RenderTexture& operator=(RenderTexture&&);

			///
			/// Destructor.
			///
			virtual ~RenderTexture();

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
			void change_size(const int width, const int height);

			///
			/// Activate texture context.
			///
			void bind() override;

			///
			/// Deactivate texture context.
			///
			void unbind() override;

			///
			/// Modify projection of render texture.
			///
			/// \param left Left point of ortho perspective.
			/// \param right Right point of ortho perspective.
			/// \param bottom Bottom point of ortho perspective.
			/// \param top Top point of ortho perspective.
			///
			void set_projection(const float left, const float right, const float bottom, const float top);

			///
			/// Get projection.
			///
			/// \return Reference to glm::mat4.
			///
			[[nodiscard]] glm::mat4& get_proj();

		private:
			///
			/// Projection.
			///
			glm::mat4 m_projection;

			///
			/// OpenGL framebuffer handle.
			///
			unsigned int m_framebuffer;
		};
	} // namespace graphics
} // namespace galaxy

#endif