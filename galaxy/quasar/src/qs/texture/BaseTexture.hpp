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
		/// Copy constructor.
		///
		BaseTexture(const BaseTexture&) noexcept = delete;

		///
		/// Move constructor.
		///
		BaseTexture(BaseTexture&&);

		///
		/// Copy assignment operator.
		///
		BaseTexture& operator=(const BaseTexture&) noexcept = delete;

		///
		/// Move assignment operator.
		///
		BaseTexture& operator=(BaseTexture&&);

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
		void save(std::string_view path);

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
		void clamp_to_edge() noexcept;

		///
		/// \brief Clamps to the border.
		///
		/// Coordinates outside the range are now given a user-specified border color.
		///
		void clamp_to_border() noexcept;

		///
		/// Makes the texture repeat over its verticies.
		///
		void set_repeated() noexcept;

		///
		/// Mirrors the texture.
		///
		void set_mirrored() noexcept;

		///
		/// Set ansiotropic filtering level.
		///
		/// \param level 2, 4, 8, etc...
		///
		void set_anisotropy(const unsigned int level) noexcept;

		///
		/// Set filter when texture is downscaled in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void set_minify_filter(const qs::TextureFilter& filter) noexcept;

		///
		/// Set filter when texture would be scaled up in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void set_magnify_filter(const qs::TextureFilter& filter) noexcept;

		///
		/// \brief Get texture width.
		///
		/// Is cached for performance.
		///
		/// \return Width as int. int over unsigned for compat with float.
		///
		[[nodiscard]] const int get_width() const noexcept;

		///
		/// \brief Get texture height.
		///
		/// Is cached for performance.
		///
		/// \return Height as int. int over unsigned for compat with float.
		///
		[[nodiscard]] const int get_height() const noexcept;

		///
		/// Gets internal OpenGL id.
		///
		/// \return Const unsigned int.
		///
		[[nodiscard]] const unsigned int gl_texture() const noexcept;

	protected:
		///
		/// Constructor.
		///
		BaseTexture() noexcept;

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
} // namespace qs

#endif