///
/// Image.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_IMAGE_HPP_
#define GALAXY_GRAPHICS_IMAGE_HPP_

#include <string>
#include <span>

#include <raylib.hpp>

#include "galaxy/graphics/Colour.hpp"
#include "galaxy/platform/Pragma.hpp"
#include "galaxy/graphics/Rectangle.hpp"

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_PUSH
GALAXY_DISABLE_WARNING(26434)
#endif

namespace galaxy
{
	namespace graphics
	{
		///
		/// CPU side pixel data.
		///
		class Image final : public ::Image
		{
		public:
			///
			/// Constructor.
			///
			Image() noexcept;

			///
			/// Raylib constructor.
			///
			Image(const ::Image& img) noexcept;

			///
			/// Move constructor.
			///
			Image(Image&&) noexcept;

			///
			/// Move assignment operator.
			///
			[[nodiscard]]
			Image& operator=(Image&&) noexcept;

			///
			/// Destructor.
			///
			~Image() noexcept;

			///
			/// Load image from file into CPU memory (RAM).
			///
			/// \param file File path of the image.
			///
			void load(const std::string& file);

			///
			/// Load image from RAW file data.
			///
			/// \param file File path of the raw image.
			/// \param width Width of the image.
			/// \param height Height of the image.
			/// \param format Format of the image.
			/// \param header_size Size of the header.
			///
			void load_raw(const std::string& file, const int width, const int height, const ray::PixelFormat format, const int header_size);

			///
			/// Load image from memory buffer.
			///
			/// \param file_type File type of the image.
			/// \param file_data Pointer to the memory buffer.
			///
			void load_from_memory(const std::string& file_type, std::span<unsigned char> file_data);

			///
			/// Load image from screen buffer and (screenshot).
			///
			void load_from_screen();

			///
			/// Check if an image is valid (data and parameters).
			///
			/// \return True if the image is valid, false otherwise.
			///
			[[nodiscard]]
			bool is_valid() const noexcept;

			///
			/// Unload image from CPU memory (RAM).
			///
			void unload() noexcept;

			///
			/// Export image data to file, returns true on success.
			///
			/// \param file File path to save the image.
			///
			void save(const std::string& file);

			///
			/// Create an image from another image piece.
			///
			/// \param img Source image.
			/// \param rec Rectangle specifying the piece of the source image.
			///
			/// \return Created image piece.
			///
			[[nodiscard]]
			Image from_src(const Image& img, const Rectangle& rec) noexcept;

			///
			/// Convert image data to desired format.
			///
			/// \param new_format Desired format.
			///
			void set_format(const int new_format) noexcept;

			///
			/// Convert image to POT (power-of-two).
			///
			/// \param fill Fill colour.
			///
			void to_pot(const Colour& fill) noexcept;

			///
			/// Crop an image to a defined rectangle.
			///
			/// \param crop Rectangle to crop.
			///
			void crop(const Rectangle& crop) noexcept;

			///
			/// Crop image depending on alpha value.
			///
			/// \param threshold Alpha value threshold.
			///
			void alpha_crop(const float threshold) noexcept;

			///
			/// Clear alpha channel to desired colour.
			///
			/// \param colour Colour to clear the alpha channel to.
			/// \param threshold Alpha value threshold.
			///
			void alpha_clear(const Colour& colour, const float threshold) noexcept;

			///
			/// Apply alpha mask to image.
			///
			/// \param alpha_mask Alpha mask to apply.
			///
			void alpha_mask(const Image& alpha_mask) noexcept;

			///
			/// Premultiply alpha channel.
			///
			void alpha_premultiply() noexcept;

			///
			/// Apply Gaussian blur using a box blur approximation.
			///
			/// \param blur_size Size of the blur.
			///
			void blur_gaussian(const int blur_size) noexcept;

			///
			/// Apply custom square convolution kernel to image.
			///
			/// \param kernel Pointer to the convolution kernel.
			/// \param kernel_size Size of the kernel.
			///
			void kernel_convolution(const float* kernel, const int kernel_size) noexcept;

			///
			/// Resize image (Bicubic scaling algorithm).
			///
			/// \param new_width New width of the image.
			/// \param new_height New height of the image.
			///
			void resize(const int new_width, const int new_height) noexcept;

			///
			/// Resize image (Nearest-Neighbor scaling algorithm).
			///
			/// \param new_width New width of the image.
			/// \param new_height New height of the image.
			///
			void resize_nn(const int new_width, const int new_height) noexcept;

			///
			/// Resize canvas and fill with colour.
			///
			/// \param new_width New width of the canvas.
			/// \param new_height New height of the canvas.
			/// \param offset_x X offset.
			/// \param offset_y Y offset.
			/// \param fill Fill colour.
			///
			void resize_canvas(const int new_width, const int new_height, const int offset_x, const int offset_y, const Colour& fill) noexcept;

			///
			/// Dither image data to 16bpp or lower (Floyd-Steinberg dithering).
			///
			/// \param r_bpp Red bit-per-pixel.
			/// \param g_bpp Green bit-per-pixel.
			/// \param b_bpp Blue bit-per-pixel.
			/// \param a_bpp Alpha bit-per-pixel.
			///
			void dither(const int r_bpp, const int g_bpp, const int b_bpp, const int a_bpp) noexcept;

			///
			/// Flip image vertically.
			///
			void flip_vertical() noexcept;

			///
			/// Flip image horizontally.
			///
			void flip_horizontal() noexcept;

			///
			/// Rotate image by input angle in degrees (-359 to 359).
			///
			/// \param degrees Angle in degrees.
			///
			void rotate(const int degrees) noexcept;

			///
			/// Rotate image clockwise 90deg.
			///
			void rotate_cw() noexcept;

			///
			/// Rotate image counter-clockwise 90deg.
			///
			void rotate_ccw() noexcept;

			///
			/// Modify image colour: tint.
			///
			/// \param colour Tint colour.
			///
			void colour_tint(const Colour& colour) noexcept;

			///
			/// Modify image colour: invert.
			///
			void colour_invert() noexcept;

			///
			/// Modify image colour: grayscale.
			///
			void colour_grayscale() noexcept;

			///
			/// Modify image colour: contrast (-100 to 100).
			///
			/// \param contrast Contrast level.
			///
			void colour_contrast(const float contrast) noexcept;

			///
			/// Modify image colour: brightness (-255 to 255).
			///
			/// \param brightness Brightness level.
			///
			void colour_brightness(const int brightness) noexcept;

			///
			/// Modify image colour: replace colour.
			///
			/// \param colour Colour to replace.
			/// \param replace Colour to replace with.
			///
			void colour_replace(const Colour& colour, const Colour& replace) noexcept;

		private:
			///
			/// Copy constructor.
			///
			Image(const Image&) = delete;

			///
			/// Copy assignment operator.
			///
			Image& operator=(const Image&) = delete;

			///
			/// Configure loaded image.
			///
			/// \param img Raylib image object.
			///
			void set(const ::Image& img) noexcept;
		};
	} // namespace graphics
} // namespace galaxy

#ifdef GALAXY_WIN_PLATFORM
GALAXY_DISABLE_WARNING_POP
#endif

#endif
