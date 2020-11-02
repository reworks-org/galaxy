///
/// RenderTexture.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERTEXTURE_HPP_
#define QUASAR_RENDERTEXTURE_HPP_

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
		/// \param width Width of the RenderTexture.
		/// \param height Height of the RenderTexture.
		///
		explicit RenderTexture(const int width, const int height);

		///
		/// Copy constructor.
		///
		RenderTexture(const RenderTexture&) noexcept = delete;

		///
		/// Move constructor.
		///
		RenderTexture(RenderTexture&&);

		///
		/// Copy assignment operator.
		///
		RenderTexture& operator=(const RenderTexture&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		RenderTexture& operator=(RenderTexture&&);

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
		void change_size(const int width, const int height);

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
		/// \return Reference to glm::mat4.
		///
		[[nodiscard]] glm::mat4& get_proj() noexcept;

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
} // namespace qs

#endif