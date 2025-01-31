///
/// Texture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_HPP_
#define GALAXY_GRAPHICS_TEXTURE_HPP_

#include <string>

#include <raylib.hpp>

#include "galaxy/platform/Pragma.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
#endif

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief GPU texture.
		///
		/// For 2D and cubemap textures.
		///
		class Texture : public ::Texture
		{
		public:
			///
			/// Check if a texture is valid (loaded in GPU).
			///
			/// \return bool True if the texture is valid, false otherwise.
			///
			[[nodiscard]]
			bool is_valid() const noexcept;

			///
			/// Unload texture from GPU memory (VRAM).
			///
			void unload() noexcept;

			///
			/// Update GPU texture with new data.
			///
			/// \param pixels Pointer to the new pixel data.
			///
			void update(const void* pixels) noexcept;

			///
			/// Update GPU texture rectangle with new data.
			///
			/// \param rec Rectangle to update.
			/// \param pixels Pointer to the new pixel data.
			///
			void update_rec(const Rectangle& rec, const void* pixels) noexcept;

			///
			/// Set texture scaling filter mode.
			///
			/// \param filter Filter mode.
			///
			void set_texture_filter(const ray::TextureFilter filter) noexcept;

			///
			/// Set texture wrapping mode.
			///
			/// \param wrap Wrapping mode.
			///
			void set_texture_wrap(const ray::TextureWrap wrap) noexcept;

		protected:
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
			/// Destructor.
			///
			virtual ~Texture() noexcept;

			///
			/// Configure loaded texture.
			///
			/// \param tex Raylib texture handle.
			///
			void set(const ::Texture& tex) noexcept;

		private:
			///
			/// Copy constructor.
			///
			Texture(const Texture&) = delete;

			///
			/// Copy assignment operator.
			///
			Texture& operator=(const Texture&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
