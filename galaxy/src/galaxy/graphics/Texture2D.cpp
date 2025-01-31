///
/// Texture2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Texture2D.hpp"

namespace galaxy
{
	namespace graphics
	{
		Texture2D::Texture2D() noexcept
			: Texture {}
		{
		}

		Texture2D::Texture2D(Texture2D&& t) noexcept
			: Texture(std::move(t))
		{
		}

		Texture2D& Texture2D::operator=(Texture2D&& t) noexcept
		{
			if (this != &t)
			{
				Texture::operator=(std::move(t));
			}

			return *this;
		}

		Texture2D::~Texture2D() noexcept
		{
		}

		void Texture2D::load(const std::string& file) noexcept
		{
			const auto tex = ::LoadTexture(file.c_str());
			configure(tex);
		}

		void Texture2D::load(const Image& image) noexcept
		{
			const auto tex = ::LoadTextureFromImage(image);
			configure(tex);
		}
	} // namespace graphics
} // namespace galaxy
