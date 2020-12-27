///
/// BaseTexture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_BASETEXTURE_HPP_
#define GALAXY_GRAPHICS_TEXTURE_BASETEXTURE_HPP_

#include <string_view>

#include "galaxy/graphics/texture/TextureFilters.hpp"

namespace galaxy
{
	namespace graphics
	{
		class BaseTexture
		{
		public:
			///
			/// Copy constructor.
			///
			BaseTexture(const BaseTexture&) = delete;

			///
			/// Move constructor.
			///
			BaseTexture(BaseTexture&&);

			///
			/// Copy assignment operator.
			///
			BaseTexture& operator=(const BaseTexture&) = delete;

			///
			/// Move assignment operator.
			///
			BaseTexture& operator=(BaseTexture&&);

			///
			/// Virtual destructor.
			///
			virtual ~BaseTexture();

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
			virtual void bind() = 0;

			///
			/// Deactivate texture context.
			///
			virtual void unbind() = 0;

			///
			/// \brief Clamps texture to edges.
			///
			/// Clamps the coordinates between 0 and 1.
			/// The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
			///
			void clamp_to_edge();

			///
			/// \brief Clamps to the border.
			///
			/// Coordinates outside the range are now given a user-specified border color.
			///
			void clamp_to_border();

			///
			/// Makes the texture repeat over its verticies.
			///
			void set_repeated();

			///
			/// Mirrors the texture.
			///
			void set_mirrored();

			///
			/// Set ansiotropic filtering level.
			///
			/// \param level 2, 4, 8, etc...
			///
			void set_anisotropy(const unsigned int level);

			///
			/// Set filter when texture is downscaled in OpenGL.
			///
			/// \param filter Enum filter to apply to texture.
			///
			void set_minify_filter(const TextureFilter& filter);

			///
			/// Set filter when texture would be scaled up in OpenGL.
			///
			/// \param filter Enum filter to apply to texture.
			///
			void set_magnify_filter(const TextureFilter& filter);

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as int. int over unsigned for compat with float.
			///
			[[nodiscard]] const int get_height() const;

			///
			/// Get Textures current Ansiotrophy level.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_aniso_level();

			///
			/// Gets internal OpenGL id.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int gl_texture() const;

		protected:
			///
			/// Constructor.
			///
			BaseTexture();

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
	} // namespace graphics
} // namespace galaxy

#endif