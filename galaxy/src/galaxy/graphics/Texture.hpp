///
/// Texture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_HPP_
#define GALAXY_GRAPHICS_TEXTURE_HPP_

#include <span>
#include <string_view>

#include "galaxy/graphics/TextureModes.hpp"
#include "galaxy/graphics/TextureFilters.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// OpenGL 2D Texture.
		///
		class Texture final
		{
		public:
			///
			/// Constructor.
			///
			Texture();

			///
			/// Argument Constructor.
			///
			/// \param file File on disk to load from.
			///
			Texture(std::string_view file);

			///
			/// Move constructor.
			///
			Texture(Texture&&);

			///
			/// Move assignment operator.
			///
			Texture& operator=(Texture&&);

			///
			/// Destructor.
			///
			~Texture();

			///
			///  Loads texture from file.
			///
			/// \param file File on disk to load from.
			///
			/// \return True if texture loaded successfully.
			///
			[[maybe_unused]] bool load(std::string_view file);

			///
			/// \brief Loads texture from memory.
			///
			/// Loads from standard unsigned char array.
			///
			/// \param buffer Memory buffer to load from.
			///
			/// \return True if texture loaded successfully.
			///
			[[maybe_unused]] bool load_mem(std::span<unsigned char> buffer);

			///
			/// \brief Load raw pixel data from memory.
			///
			/// You need to ensure you call mode/filter/ansio to configure properly.
			///
			/// \param width Width of texture.
			/// \param height Height of texture.
			/// \param storage_format Opengl TextureStorage format.
			/// \param pixel_format Opengl Pixel data format.
			/// \param type Type of data i.e. GL_FLOAT, GL_UNSIGNED_BYTE, etc.
			/// \param buffer Raw pixel data.
			///
			void load_raw(const int width, const int height, unsigned int storage_format, unsigned int pixel_format, unsigned int type, void* buffer);

			///
			/// Saves texture to file on disk.
			///
			/// \param filepath Path and filename to save texture to. Does not need extension (it will be ignored).
			///
			void save(std::string_view filepath);

			///
			/// Activate texture context.
			///
			void bind();

			///
			/// Deactivate texture context.
			///
			void unbind();

			///
			/// Set texture mode.
			///
			/// \param mode I.e. GL_REPEAT, GL_CLAMP_TO_EDGE, etc.
			///
			void set_mode(const TextureModes mode);

			///
			/// Set filter when texture is (up/down)scaled in OpenGL.
			///
			/// \param filter Filtering to use for min/mag.
			///
			void set_filter(const TextureFilters filter);

			///
			/// Set ansiotropic filtering level.
			///
			/// \param level 1 to 16.
			///
			void set_anisotropy(const int level);

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as int.
			///
			[[nodiscard]] int get_width() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as int.
			///
			[[nodiscard]] int get_height() const;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as float.
			///
			[[nodiscard]] float get_widthf() const;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as float.
			///
			[[nodiscard]] float get_heightf() const;

			///
			/// Get Textures current Ansiotrophy level.
			///
			/// \return Const int.
			///
			[[nodiscard]] int get_aniso_level();

			///
			/// Gets internal OpenGL id.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] unsigned int handle() const;

		private:
			///
			/// Copy constructor.
			///
			Texture(const Texture&) = delete;

			///
			/// Copy assignment operator.
			///
			Texture& operator=(const Texture&) = delete;

		protected:
			///
			/// Cached texture width.
			///
			int m_width;

			///
			/// Cached texture height.
			///
			int m_height;

			///
			/// OpenGL texture handle.
			///
			unsigned int m_texture;
		};
	} // namespace graphics
} // namespace galaxy

#endif