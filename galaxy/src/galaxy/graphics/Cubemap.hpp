///
/// Cubemap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_CUBEMAP_HPP_
#define GALAXY_GRAPHICS_CUBEMAP_HPP_

#include "galaxy/graphics/Texture.hpp"
#include "galaxy/graphics/Image.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief GPU texture.
		///
		/// For cubemap texture.
		///
		class Cubemap final : public Texture
		{
		public:
			///
			/// Constructor.
			///
			Cubemap() noexcept;

			///
			/// Move constructor.
			///
			Cubemap(Cubemap&&) noexcept;

			///
			/// Move assignment operator.
			///
			Cubemap& operator=(Cubemap&&) noexcept;

			///
			/// Destructor.
			///
			virtual ~Cubemap() noexcept;

			///
			/// Load cubemap from image, multiple image cubemap layouts supported.
			///
			/// \param image Image data.
			/// \param layout Cubemap layout.
			///
			void load(const Image& image, const ray::CubemapLayout layout) noexcept;

		private:
			///
			/// Copy constructor.
			///
			Cubemap(const Cubemap&) = delete;

			///
			/// Copy assignment operator.
			///
			Cubemap& operator=(const Cubemap&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#endif
