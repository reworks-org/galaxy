///
/// RenderTexture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_RENDERTEXTURE_HPP_
#define GALAXY_GRAPHICS_RENDERTEXTURE_HPP_

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
		/// Framebuffer target.
		///
		class RenderTexture : public ::RenderTexture
		{
		public:
			///
			/// Constructor.
			///
			RenderTexture() noexcept;

			///
			/// Move constructor.
			///
			RenderTexture(RenderTexture&&) noexcept;

			///
			/// Move assignment operator.
			///
			RenderTexture& operator=(RenderTexture&&) noexcept;

			///
			/// Destructor.
			///
			~RenderTexture() noexcept;

			///
			/// Load texture for rendering (framebuffer).
			///
			/// \param width Width of the texture.
			/// \param height Height of the texture.
			///
			void create(const int width, const int height);

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
			/// Configure loaded texture.
			///
			/// \param tex Raylib texture handle.
			///
			void set(const ::RenderTexture& tex) noexcept;

		private:
			///
			/// Copy constructor.
			///
			RenderTexture(const RenderTexture&) = delete;

			///
			/// Copy assignment operator.
			///
			RenderTexture& operator=(const RenderTexture&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
