///
/// BaseTexture.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QS_BASETEXTURE_HPP_
#define QS_BASETEXTURE_HPP_

#include "qs/utils/TextureFilters.hpp"

///
/// Core namespace.
///
namespace qs
{
	class BaseTexture
	{
	public:
		///
		/// Virtual destructor.
		///
		virtual ~BaseTexture() noexcept;

		///
		/// Saves texture to file on disk.
		///
		/// \param path Path (including filename) to save file to.
		///				Do not include extension. So i.e. "textures/wall" to save to wall.png.
		///
		void save(const std::string& path) noexcept;

		///
		/// Activate texture context.
		///
		virtual void bind() noexcept = 0;

		///
		/// Deactivate texture context.
		///
		virtual void unbind() noexcept = 0;

		///
		/// \brief Clamps texture to edges.
		///
		/// Clamps the coordinates between 0 and 1.
		/// The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
		///
		virtual void clampToEdge() noexcept final;

		///
		/// \brief Clamps to the border.
		///
		/// Coordinates outside the range are now given a user-specified border color.
		///
		virtual void clampToBorder() noexcept final;

		///
		/// Makes the texture repeat over its verticies.
		///
		virtual void setRepeated() noexcept final;

		///
		/// Mirrors the texture.
		///
		virtual void setMirrored() noexcept final;

		///
		/// Set ansiotropic filtering level.
		///
		/// \param level 2, 4, 8, etc...
		///
		virtual void setAnisotropy(const int level) noexcept final;

		///
		/// Set filter when texture is downscaled in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		virtual void setMinifyFilter(const qs::TextureFilter& filter) noexcept final;

		///
		/// Set filter when texture would be scaled up in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		virtual void setMagnifyFilter(const qs::TextureFilter& filter) noexcept final;

		///
		/// \brief Get texture width.
		///
		/// Is cached for performance.
		///
		/// \return Width as int. int over unsigned for compat with float.
		///
		virtual const int getWidth() const noexcept final;

		///
		/// \brief Get texture height.
		///
		/// Is cached for performance.
		///
		/// \return Height as int. int over unsigned for compat with float.
		///
		virtual const int getHeight() const noexcept final;

		///
		/// Gets internal OpenGL id.
		///
		/// \return Const unsigned int.
		///
		virtual const unsigned int getGLTexture() const noexcept final;

	protected:
		///
		/// Constructor.
		///
		BaseTexture() noexcept;

	protected:
		///
		/// OpenGL texture handle.
		///
		unsigned int m_texture;

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