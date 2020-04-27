///
/// RenderTexture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_RENDERTEXTURE_HPP_
#define QUASAR_RENDERTEXTURE_HPP_

#include <glm/gtc/matrix_transform.hpp>

#include "qs/core/Colours.hpp"
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
	class RenderTexture final
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
		explicit RenderTexture(const int w, const int h);

		///
		/// Virtual destructor override.
		///
		~RenderTexture();

		///
		/// Create the RenderTexture.
		///
		/// \param w Width of the RenderTexture.
		/// \param h Height of the RenderTexture.
		///
		void create(const int w, const int h) noexcept;

		///
		/// Saves texture to file on disk.
		///
		/// \param path Path (including filename) to save file to.
		///				Do not include extension. So i.e. "textures/wall" to save to wall.png.
		///
		void save(const std::string& path) noexcept;

		///
		/// Activate RenderTexture for drawing.
		///
		void bind() noexcept;

		///
		/// Display framebuffer to RenderTexture.
		///
		/// \param window Window to restore default rendering to.
		///
		void unbind(qs::Window& window) noexcept;

		///
		/// Modify projection of render texture.
		///
		/// \param left Left point of ortho perspective.
		/// \param right Right point of ortho perspective.
		/// \param bottom Bottom point of ortho perspective.
		/// \param top Top point of ortho perspective.
		///
		void updateProjection(const float left, const float right, const float bottom, const float top) noexcept;

		///
		/// Makes the texture repeat over its verticies.
		///
		void setRepeated() noexcept;

		///
		/// Mirrors the texture.
		///
		void setMirrored() noexcept;

		///
		/// \brief Clamps texture to edges.
		///
		/// Clamps the coordinates between 0 and 1.
		/// The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
		///
		void clampToEdge() noexcept;

		///
		/// \brief Clamps to the border.
		///
		/// Coordinates outside the range are now given a user-specified border color.
		///
		/// \param border Colour of the border.
		///
		void clampToBorder(protostar::Colour& border) noexcept;

		///
		/// Set ansiotropic filtering level.
		///
		/// \param level 2, 4, 8, etc...
		///
		void setAnisotropy(const int level) noexcept;

		///
		/// Set filter when texture is downscaled in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void setMinifyFilter(const qs::TextureFilter& filter) noexcept;

		///
		/// Set filter when texture would be scaled up in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void setMagnifyFilter(const qs::TextureFilter& filter) noexcept;

		///
		/// \brief Get texture width.
		///
		/// Is cached for performance.
		///
		/// \return Width as int. int over unsigned for compat with float.
		///
		const int getWidth() const noexcept;

		///
		/// \brief Get texture height.
		///
		/// Is cached for performance.
		///
		/// \return Height as int. int over unsigned for compat with float.
		///
		const int getHeight() const noexcept;

		///
		/// Gets internal OpenGL id.
		///
		/// \return Const unsigned int.
		///
		const unsigned int getGLTexture() const noexcept;

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
		/// OpenGL texture handle.
		///
		unsigned int m_texture;

		///
		/// OpenGL framebuffer handle.
		///
		unsigned int m_framebuffer;

		///
		/// Cached texture width.
		///
		int m_width;

		///
		/// Cached texture height.
		///
		int m_height;
	};
}

#endif