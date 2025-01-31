///
/// RenderTexture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "galaxy/core/Settings.hpp"

#include "RenderTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		RenderTexture::RenderTexture() noexcept
		{
			id = 0;

			texture.id      = 0;
			texture.width   = 0;
			texture.height  = 0;
			texture.mipmaps = 1;
			texture.format  = 0;

			depth.id      = 0;
			depth.width   = 0;
			depth.height  = 0;
			depth.mipmaps = 1;
			depth.format  = 0;
		}

		RenderTexture::RenderTexture(RenderTexture&& t) noexcept
		{
			this->unload();
			this->id = t.id;

			this->texture.id      = t.texture.id;
			this->texture.width   = t.texture.width;
			this->texture.height  = t.texture.height;
			this->texture.mipmaps = t.texture.mipmaps;
			this->texture.format  = t.texture.format;

			this->depth.id      = t.depth.id;
			this->depth.width   = t.depth.width;
			this->depth.height  = t.depth.height;
			this->depth.mipmaps = t.depth.mipmaps;
			this->depth.format  = t.depth.format;

			t.id         = 0;
			t.texture.id = 0;
			t.depth.id   = 0;
		}

		RenderTexture& RenderTexture::operator=(RenderTexture&& t) noexcept
		{
			if (this != &t)
			{
				this->unload();
				this->id = t.id;

				this->texture.id      = t.texture.id;
				this->texture.width   = t.texture.width;
				this->texture.height  = t.texture.height;
				this->texture.mipmaps = t.texture.mipmaps;
				this->texture.format  = t.texture.format;

				this->depth.id      = t.depth.id;
				this->depth.width   = t.depth.width;
				this->depth.height  = t.depth.height;
				this->depth.mipmaps = t.depth.mipmaps;
				this->depth.format  = t.depth.format;

				t.id         = 0;
				t.texture.id = 0;
				t.depth.id   = 0;
			}

			return *this;
		}

		RenderTexture::~RenderTexture() noexcept
		{
			unload();
		}

		void RenderTexture::create(const int width, const int height)
		{
			const auto rt = ::LoadRenderTexture(width, height);
			set(rt);
		}

		bool RenderTexture::is_valid() const noexcept
		{
			return ::IsRenderTextureValid(*this);
		}

		void RenderTexture::unload() noexcept
		{
			if (is_valid())
			{
				::UnloadRenderTexture(*this);
				id = 0;
			}
		}

		void RenderTexture::set_texture_filter(const ray::TextureFilter filter) noexcept
		{
			::SetTextureFilter(texture, static_cast<int>(filter));
		}

		void RenderTexture::set_texture_wrap(const ray::TextureWrap wrap) noexcept
		{
			::SetTextureWrap(texture, static_cast<int>(wrap));
		}

		void RenderTexture::set(const ::RenderTexture& tex) noexcept
		{
			this->unload();
			this->id = tex.id;

			this->texture.id      = tex.texture.id;
			this->texture.width   = tex.texture.width;
			this->texture.height  = tex.texture.height;
			this->texture.mipmaps = tex.texture.mipmaps;
			this->texture.format  = tex.texture.format;

			this->depth.id      = tex.depth.id;
			this->depth.width   = tex.depth.width;
			this->depth.height  = tex.depth.height;
			this->depth.mipmaps = tex.depth.mipmaps;
			this->depth.format  = tex.depth.format;

			::GenTextureMipmaps(&texture);

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
