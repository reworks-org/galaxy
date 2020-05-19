///
/// RenderTexture.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERTEXTURE_HPP_
#define QUASAR_RENDERTEXTURE_HPP_

#include <protostar/system/Colour.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "qs/texture/BaseTexture.hpp"
#include "qs/utils/TextureFilters.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Forward dec.
	///
	class Window;

	///
	/// Framebuffer texture. You render to this texture instead of the window.
	///
	class RenderTexture final : public qs::BaseTexture
	{
	public:
		///
		/// Default constructor.
		///
		RenderTexture() noexcept;

		///
		/// Argument constructor.
		///
		/// \param w Width of the RenderTexture.
		/// \param h Height of the RenderTexture.
		///
		explicit RenderTexture(const int w, const int h) noexcept;

		///
		/// Virtual destructor override.
		///
		~RenderTexture() noexcept;

		///
		/// Create the RenderTexture.
		///
		/// \param w Width of the RenderTexture.
		/// \param h Height of the RenderTexture.
		///
		void create(const int w, const int h) noexcept;

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
		void setProjection(const float left, const float right, const float bottom, const float top) noexcept;
		
		///
		/// Get projection.
		///
		/// \return Reference to glm::mat4.
		///
		glm::mat4& getProjection() noexcept;

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
}

#endif