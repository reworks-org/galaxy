///
/// Texture2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE2D_HPP_
#define GALAXY_GRAPHICS_TEXTURE2D_HPP_

#include "galaxy/graphics/Image.hpp"
#include "galaxy/graphics/Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief GPU texture.
		///
		/// For 2D texture.
		///
		class Texture2D final : public Texture
		{
		public:
			///
			/// Constructor.
			///
			Texture2D() noexcept;

			///
			/// Move constructor.
			///
			Texture2D(Texture2D&&) noexcept;

			///
			/// Move assignment operator.
			///
			[[nodiscard]]
			Texture2D& operator=(Texture2D&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Texture2D() noexcept;

			///
			/// Load Texture2D from file into GPU memory (VRAM).
			///
			/// \param file File of the Texture2D in the vfs.
			///
			void load(const std::string& file) noexcept;

			///
			/// Load Texture2D from image data.
			///
			/// \param image Image data.
			///
			void load(const Image& image) noexcept;

		private:
			///
			/// Copy constructor.
			///
			Texture2D(const Texture2D&) = delete;

			///
			/// Copy assignment operator.
			///
			Texture2D& operator=(const Texture2D&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#endif
