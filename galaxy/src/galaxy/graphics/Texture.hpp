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
			Texture() noexcept;

			///
			/// Argument Constructor.
			///
			/// \param file File on disk to load from.
			///
			Texture(std::string_view file) noexcept;

			///
			/// Move constructor.
			///
			Texture(Texture&&) noexcept;

			///
			/// Move assignment operator.
			///
			Texture& operator=(Texture&&) noexcept;

			///
			/// Destructor.
			///
			~Texture() noexcept;

			///
			///  Loads texture from file.
			///
			/// \param file File on disk to load from.
			///
			void load(std::string_view file);

			///
			/// Loads texture from memory.
			///
			/// \param buffer Memory buffer to load from.
			///
			void load_mem(std::span<unsigned char> buffer);

			///
			/// Saves texture to file on disk.
			///
			/// \param filepath Path and filename to save texture to. Does not need extension (it will be ignored).
			///
			void save(const std::string& filepath);

			///
			/// Activate texture context.
			///
			void bind() noexcept;

			///
			/// Deactivate texture context.
			///
			void unbind() noexcept;

			///
			/// Set texture mode.
			///
			/// \param mode I.e. GL_REPEAT, GL_CLAMP_TO_EDGE, etc.
			///
			void set_mode(const TextureModes mode) noexcept;

			///
			/// Set filter when texture is (up/down)scaled in OpenGL.
			///
			/// \param filter Filtering to use for min/mag.
			///
			void set_filter(const TextureFilters filter) noexcept;

			///
			/// Set ansiotropic filtering level.
			///
			/// \param level 2, 4, 8, etc...
			///
			void set_anisotropy(const int level) noexcept;

			///
			/// \brief Get texture width.
			///
			/// Is cached for performance.
			///
			/// \return Width as int. int over unsigned for compat with float.
			///
			[[nodiscard]] int get_width() const noexcept;

			///
			/// \brief Get texture height.
			///
			/// Is cached for performance.
			///
			/// \return Height as int. int over unsigned for compat with float.
			///
			[[nodiscard]] int get_height() const noexcept;

			///
			/// Get Textures current Ansiotrophy level.
			///
			/// \return Const int.
			///
			[[nodiscard]] int get_aniso_level() noexcept;

			///
			/// Gets internal OpenGL id.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] unsigned int gl_texture() const noexcept;

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