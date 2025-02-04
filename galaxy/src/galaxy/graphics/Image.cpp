///
/// Image.cpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#include "Image.hpp"

namespace galaxy
{
	namespace graphics
	{
		Image::Image() noexcept
		{
			data    = nullptr;
			width   = 0;
			height  = 0;
			mipmaps = 1;
			format  = static_cast<int>(PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
		}

		Image::Image(const ::Image& img) noexcept
		{
			set(img);
		}

		Image::Image(Image&& i) noexcept
		{
			this->unload();

			this->data    = i.data;
			this->width   = i.width;
			this->height  = i.height;
			this->mipmaps = i.mipmaps;
			this->format  = static_cast<int>(PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

			i.data = nullptr;
		}

		Image& Image::operator=(Image&& i) noexcept
		{
			if (this != &i)
			{
				this->unload();

				this->data    = i.data;
				this->width   = i.width;
				this->height  = i.height;
				this->mipmaps = i.mipmaps;
				this->format  = static_cast<int>(PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

				i.data = nullptr;
			}

			return *this;
		}

		Image::~Image() noexcept
		{
			unload();
		}

		void Image::load(const std::string& file)
		{
			set(::LoadImage(file.c_str()));
		}

		void Image::load_raw(const std::string& file, const int width, const int height, const ray::PixelFormat format, const int header_size)
		{
			set(::LoadImageRaw(file.c_str(), width, height, static_cast<int>(format), header_size));
		}

		void Image::load_from_memory(const std::string& file_type, std::span<unsigned char> file_data)
		{
			set(::LoadImageFromMemory(file_type.c_str(), file_data.data(), static_cast<int>(file_data.size())));
		}

		void Image::load_from_screen()
		{
			set(::LoadImageFromScreen());
		}

		bool Image::is_valid() const noexcept
		{
			return ::IsImageValid(*this);
		}

		void Image::unload() noexcept
		{
			::UnloadImage(*this);
			data = nullptr;
		}

		void Image::save(const std::string& file)
		{
			::ExportImage(*this, file.c_str());
		}

		Image Image::from_src(const Image& img, const Rectangle& rec) noexcept
		{
			const auto rimg = ::ImageFromImage(img, rec);

			Image image;
			image.data    = rimg.data;
			image.width   = rimg.width;
			image.height  = rimg.height;
			image.mipmaps = rimg.mipmaps;
			image.format  = rimg.format;

			return image;
		}

		void Image::set_format(const int new_format) noexcept
		{
			::ImageFormat(this, new_format);
		}

		void Image::to_pot(const Colour& fill) noexcept
		{
			::ImageToPOT(this, fill);
		}

		void Image::crop(const Rectangle& crop) noexcept
		{
			::ImageCrop(this, crop);
		}

		void Image::alpha_crop(const float threshold) noexcept
		{
			::ImageAlphaCrop(this, threshold);
		}

		void Image::alpha_clear(const Colour& colour, const float threshold) noexcept
		{
			::ImageAlphaClear(this, colour, threshold);
		}

		void Image::alpha_mask(const Image& alpha_mask) noexcept
		{
			::ImageAlphaMask(this, alpha_mask);
		}

		void Image::alpha_premultiply() noexcept
		{
			::ImageAlphaPremultiply(this);
		}

		void Image::blur_gaussian(const int blur_size) noexcept
		{
			::ImageBlurGaussian(this, blur_size);
		}

		void Image::kernel_convolution(const float* kernel, const int kernel_size) noexcept
		{
			::ImageKernelConvolution(this, kernel, kernel_size);
		}

		void Image::resize(const int new_width, const int new_height) noexcept
		{
			::ImageResize(this, new_width, new_height);
		}

		void Image::resize_nn(const int new_width, const int new_height) noexcept
		{
			::ImageResizeNN(this, new_width, new_height);
		}

		void Image::resize_canvas(const int new_width, const int new_height, const int offset_x, const int offset_y, const Colour& fill) noexcept
		{
			::ImageResizeCanvas(this, new_width, new_height, offset_x, offset_y, fill);
		}

		void Image::dither(const int r_bpp, const int g_bpp, const int b_bpp, const int a_bpp) noexcept
		{
			::ImageDither(this, r_bpp, g_bpp, b_bpp, a_bpp);
		}

		void Image::flip_vertical() noexcept
		{
			::ImageFlipVertical(this);
		}

		void Image::flip_horizontal() noexcept
		{
			::ImageFlipHorizontal(this);
		}

		void Image::rotate(const int degrees) noexcept
		{
			::ImageRotate(this, degrees);
		}

		void Image::rotate_cw() noexcept
		{
			::ImageRotateCW(this);
		}

		void Image::rotate_ccw() noexcept
		{
			::ImageRotateCCW(this);
		}

		void Image::colour_tint(const Colour& colour) noexcept
		{
			::ImageColorTint(this, colour);
		}

		void Image::colour_invert() noexcept
		{
			::ImageColorInvert(this);
		}

		void Image::colour_grayscale() noexcept
		{
			::ImageColorGrayscale(this);
		}

		void Image::colour_contrast(const float contrast) noexcept
		{
			::ImageColorContrast(this, contrast);
		}

		void Image::colour_brightness(const int brightness) noexcept
		{
			::ImageColorBrightness(this, brightness);
		}

		void Image::colour_replace(const Colour& colour, const Colour& replace) noexcept
		{
			::ImageColorReplace(this, colour, replace);
		}

		void Image::set(const ::Image& img) noexcept
		{
			this->data    = img.data;
			this->width   = img.width;
			this->height  = img.height;
			this->mipmaps = img.mipmaps;
			this->format  = img.format;
		}
	} // namespace graphics
} // namespace galaxy
