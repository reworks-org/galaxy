///
/// Texture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/Settings.hpp"

#include "Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		Texture::Texture() noexcept
		{
			id      = 0;
			width   = 0;
			height  = 0;
			mipmaps = 1;
			format  = static_cast<int>(PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		}

		Texture::Texture(Texture&& t) noexcept
		{
			this->unload();

			this->id      = t.id;
			this->width   = t.width;
			this->height  = t.height;
			this->mipmaps = t.mipmaps;
			this->format  = t.format;

			t.id = 0;
		}

		Texture& Texture::operator=(Texture&& t) noexcept
		{
			if (this != &t)
			{
				this->unload();

				this->id      = t.id;
				this->width   = t.width;
				this->height  = t.height;
				this->mipmaps = t.mipmaps;
				this->format  = t.format;

				t.id = 0;
			}

			return *this;
		}

		Texture::~Texture() noexcept
		{
			unload();
		}

		bool Texture::is_valid() const noexcept
		{
			return ::IsTextureValid(*this);
		}

		void Texture::unload() noexcept
		{
			if (is_valid())
			{
				::UnloadTexture(*this);
				id = 0;
			}
		}

		void Texture::update(const void* pixels) noexcept
		{
			::UpdateTexture(*this, pixels);
		}

		void Texture::update_rec(const Rectangle& rec, const void* pixels) noexcept
		{
			::UpdateTextureRec(*this, rec, pixels);
		}

		void Texture::set_texture_filter(const ray::TextureFilter filter) noexcept
		{
			::SetTextureFilter(*this, static_cast<int>(filter));
		}

		void Texture::set_texture_wrap(const ray::TextureWrap wrap) noexcept
		{
			::SetTextureWrap(*this, static_cast<int>(wrap));
		}

		void Texture::set(const ::Texture& tex) noexcept
		{
			id      = tex.id;
			width   = tex.width;
			height  = tex.height;
			mipmaps = tex.mipmaps;
			format  = tex.format;

			::GenTextureMipmaps(this);

			if (settings::ansio_level() < 2)
			{
				set_texture_filter(ray::TextureFilter::TEXTURE_FILTER_POINT);
			}
			else if (settings::ansio_level() < 4)
			{
				set_texture_filter(ray::TextureFilter::TEXTURE_FILTER_TRILINEAR);
			}
			else if (settings::ansio_level() < 8)
			{
				set_texture_filter(ray::TextureFilter::TEXTURE_FILTER_ANISOTROPIC_4X);
			}
			else if (settings::ansio_level() < 16)
			{
				set_texture_filter(ray::TextureFilter::TEXTURE_FILTER_ANISOTROPIC_8X);
			}
			else
			{
				set_texture_filter(ray::TextureFilter::TEXTURE_FILTER_ANISOTROPIC_16X);
			}
		}
	} // namespace graphics
} // namespace galaxy
