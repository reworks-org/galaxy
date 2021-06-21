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

#include "galaxy/graphics/TextureFilters.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Holds an OpenGL texture information and data.
		///
		class Texture
		{
		public:
			///
			/// Constructor.
			///
			Texture() noexcept;

			///
			/// Move constructor.
			///
			Texture(Texture&&) noexcept;

			///
			/// Move assignment operator.
			///
			Texture& operator=(Texture&&) noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Texture() noexcept;

			///
			/// \brief Loads texture from file.
			///
			/// Can throw exceptions.
			///
			/// \param file File on disk to load from.
			///
			void load(std::string_view file);

			///
			/// \brief Loads texture from memory.
			///
			/// Can throw exceptions.
			///
			/// \param buffer Memory buffer to load from. Not freed, you must free after.
			///
			void load_mem(std::span<unsigned char> buffer);

			///
			/// Saves texture to file on disk.
			///
			/// \param file_name Path and filename to save texture to. Does not need extension (it will be ignored).
			///
			void save(std::string_view file_name);

			///
			/// Activate texture context.
			///
			virtual void bind() noexcept;

			///
			/// Deactivate texture context.
			///
			virtual void unbind() noexcept;

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
			template<min_filter Filter>
			void set_minify_filter() noexcept;

			///
			/// Set filter when texture would be scaled up in OpenGL.
			///
			template<mag_filter Filter>
			void set_magnify_filter() noexcept;

			///
			/// Check if texture has been loaded.
			///
			/// \return True if load() or load_mem() has been successfully called.
			///
			[[nodiscard]] const bool is_loaded() const noexcept;

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
			/// Get Textures current Ansiotrophy level.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_aniso_level() noexcept;

			///
			/// Returns the filepath if the texture was loaded from disk.
			///
			/// \return Const std::string reference.
			///
			[[nodiscard]] const std::string& get_filepath() const noexcept;

			///
			/// Gets internal OpenGL id.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int gl_texture() const noexcept;

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
			/// Is texture loaded flag.
			///
			bool m_loaded;

			///
			/// Cached texture width.
			///
			int m_width;

			///
			/// Cached texture height.
			///
			int m_height;

			///
			/// Filepath cache.
			///
			std::string m_path;

			///
			/// OpenGL texture handle.
			///
			unsigned int m_texture;
		};

		template<min_filter Filter>
		inline void Texture::set_minify_filter() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filter::value);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		template<mag_filter Filter>
		inline void Texture::set_magnify_filter() noexcept
		{
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filter::value);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	} // namespace graphics
} // namespace galaxy

#endif