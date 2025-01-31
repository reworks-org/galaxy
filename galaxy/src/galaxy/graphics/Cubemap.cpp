///
/// Cubemap.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Cubemap.hpp"

namespace galaxy
{
	namespace graphics
	{
		Cubemap::Cubemap() noexcept
			: Texture {}
		{
		}

		Cubemap::Cubemap(Cubemap&& t) noexcept
			: Texture(std::move(t))
		{
		}

		Cubemap& Cubemap::operator=(Cubemap&& t) noexcept
		{
			if (this != &t)
			{
				Texture::operator=(std::move(t));
			}

			return *this;
		}

		Cubemap::~Cubemap() noexcept
		{
		}

		void Cubemap::load(const Image& image, const ray::CubemapLayout layout) noexcept
		{
			const auto cube = ::LoadTextureCubemap(image, static_cast<int>(layout));
			set(cube);
		}
	} // namespace graphics
} // namespace galaxy
