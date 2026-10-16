/** @file
 * @brief Implements bitmap storage, primitives, sprites, and render targets.
 */

#include "draw.h"

#include "display.h"
#include "error.h"
#include "gl2d.h"
#include "input.h"
#include "renderer.h"
#include "resource.h"

#include <SDL2/SDL_image.h>
#include <png.h>

#include <algorithm>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <cstdio>
#include <queue>
#include <vector>

namespace sl
{
	Bitmap *screen = nullptr;

	namespace
	{

		constexpr std::size_t bytes_per_pixel = 4;

		/** Check whether a bitmap has usable dimensions. */
		bool is_valid(const Bitmap *bitmap)
		{
			return bitmap && bitmap->width > 0 && bitmap->height > 0;
		}

		/** Check whether a bitmap is the display-owned screen. */
		bool is_screen(const Bitmap *bitmap)
		{
			return bitmap && bitmap->kind == BitmapKind::Screen;
		}

		std::vector<Bitmap *> active_target_stack;

		bool is_drawing_target(const Bitmap *bitmap)
		{
			if (!bitmap)
				return false;
			if (bitmap->kind == BitmapKind::Screen)
				return true;
			return !active_target_stack.empty() && bitmap == active_target_stack.back();
		}

		void sync_render_target(Bitmap *bitmap)
		{
			if (bitmap && bitmap->fbo != 0 && bitmap->ram_dirty)
				upload_bitmap(bitmap);
		}

		/** Return the byte offset of one RGBA pixel. */
		std::size_t pixel_offset(const Bitmap &bitmap, int x, int y)
		{
			return (static_cast<std::size_t>(y) * bitmap.width + x) * bytes_per_pixel;
		}

		/** Ensure a bitmap has an allocated OpenGL texture. */
		bool ensure_gpu_texture(Bitmap *bitmap)
		{
			if (!is_valid(bitmap))
			{
				return false;
			}
			if (bitmap->gpu_texture == 0)
			{
				detail::Renderer *renderer = detail::active_renderer();
				std::uint32_t texture = 0;
				if (!renderer || !renderer->create_texture({bitmap->width, bitmap->height, detail::TextureFilter::nearest}, texture))
				{
					return false;
				}
				bitmap->gpu_texture = texture;
				bitmap->gpu_dirty = !bitmap->pixels.empty();
			}
			return true;
		}

		/** Ensure a bitmap has current CPU-side pixel storage. */
		bool ensure_ram_pixels(Bitmap *bitmap)
		{
			if (!is_valid(bitmap))
			{
				return false;
			}
			if (is_screen(bitmap))
			{
				return download_bitmap(bitmap);
			}
			if (bitmap->pixels.empty())
			{
				bitmap->pixels.resize(static_cast<std::size_t>(bitmap->width) * bitmap->height * bytes_per_pixel);
				if (bitmap->gpu_texture != 0 && !bitmap->gpu_dirty)
				{
					return download_bitmap(bitmap);
				}
			}
			return true;
		}

		/** Convert an 8-bit colour to the 0-1 float components the shared shader expects. */
		void colour_components(Colour colour, float &r, float &g, float &b, float &a)
		{
			r = static_cast<float>(colour.red) / 255.0f;
			g = static_cast<float>(colour.green) / 255.0f;
			b = static_cast<float>(colour.blue) / 255.0f;
			a = static_cast<float>(colour.alpha) / 255.0f;
		}

		constexpr float pi = 3.14159265358979323846f;

		void draw_screen_ellipse(float x, float y, float radiusX, float radiusY, bool filled, Bitmap *texture, Colour colour, float thickness = 1.0f);
		void draw_screen_triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled, Bitmap *texture, Colour colour, float thickness = 1.0f);
		void draw_screen_rect(float left, float top, float right, float bottom, bool filled, Bitmap *texture, Colour colour, float thickness = 1.0f);
		void draw_screen_thick_line(float x1, float y1, float x2, float y2, float thickness, Bitmap *texture, Colour colour);

		/** Draw a bitmap region as a scaled and optionally flipped quad. */
		void draw_textured_quad(Bitmap *bitmap, int sourceX, int sourceY, int width, int height, float x, float y, int destinationWidth = -1, int destinationHeight = -1, bool flipHorizontal = false, bool flipVertical = false)
		{
			if (!upload_bitmap(bitmap))
			{
				return;
			}

			detail::gl2d_begin(screen_width(), screen_height());
			if (detail::Renderer *renderer = detail::active_renderer())
				renderer->set_premultiplied_alpha(bitmap->fbo != 0);

			const float leftTexture = static_cast<float>(sourceX) / bitmap->width;
			const float topTexture = static_cast<float>(sourceY) / bitmap->height;
			const float rightTexture = static_cast<float>(sourceX + width) / bitmap->width;
			const float bottomTexture = static_cast<float>(sourceY + height) / bitmap->height;
			const float right = x + static_cast<float>(destinationWidth < 0 ? width : destinationWidth);
			const float bottom = y + static_cast<float>(destinationHeight < 0 ? height : destinationHeight);
			const float textureLeft = flipHorizontal ? rightTexture : leftTexture;
			const float textureRight = flipHorizontal ? leftTexture : rightTexture;
			const float textureTop = flipVertical ? bottomTexture : topTexture;
			const float textureBottom = flipVertical ? topTexture : bottomTexture;

			const detail::GLVertex vertices[4] = {
				{x, y, textureLeft, textureTop, 1.0f, 1.0f, 1.0f, 1.0f},
				{right, y, textureRight, textureTop, 1.0f, 1.0f, 1.0f, 1.0f},
				{right, bottom, textureRight, textureBottom, 1.0f, 1.0f, 1.0f, 1.0f},
				{x, bottom, textureLeft, textureBottom, 1.0f, 1.0f, 1.0f, 1.0f},
			};
			detail::gl2d_submit(detail::PrimitiveType::triangle_fan, vertices, 4, bitmap->gpu_texture);
		}

		/** Draw a bitmap as a scaled quad rotated around its screen-space centre. */
		void draw_textured_quad_rotated(Bitmap *bitmap, float centerX, float centerY, float angleDegrees, int destinationWidth = -1, int destinationHeight = -1)
		{
			if (!upload_bitmap(bitmap))
			{
				return;
			}

			const float width = static_cast<float>(destinationWidth < 0 ? bitmap->width : destinationWidth);
			const float height = static_cast<float>(destinationHeight < 0 ? bitmap->height : destinationHeight);
			const float angleRadians = angleDegrees * pi / 180.0f;
			const float cosine = std::cos(angleRadians);
			const float sine = std::sin(angleRadians);
			const auto rotate = [centerX, centerY, cosine, sine](float x, float y)
			{
				return std::pair<float, float>{
					centerX + x * cosine - y * sine,
					centerY + x * sine + y * cosine};
			};

			const auto topLeft = rotate(-width * 0.5f, -height * 0.5f);
			const auto topRight = rotate(width * 0.5f, -height * 0.5f);
			const auto bottomRight = rotate(width * 0.5f, height * 0.5f);
			const auto bottomLeft = rotate(-width * 0.5f, height * 0.5f);

			detail::gl2d_begin(screen_width(), screen_height());
			if (detail::Renderer *renderer = detail::active_renderer())
				renderer->set_premultiplied_alpha(bitmap->fbo != 0);
			const detail::GLVertex vertices[4] = {
				{topLeft.first, topLeft.second, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f},
				{topRight.first, topRight.second, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f},
				{bottomRight.first, bottomRight.second, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
				{bottomLeft.first, bottomLeft.second, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
			};
			detail::gl2d_submit(detail::PrimitiveType::triangle_fan, vertices, 4, bitmap->gpu_texture);
		}

		/** Draw a bitmap region rotated clockwise around a pivot given in destination pixels, scaled to the destination size; (x, y) is the unrotated top-left. */
		void draw_textured_region_rotated(Bitmap *bitmap, int sourceX, int sourceY, int width, int height, float x, float y, float destinationWidth, float destinationHeight, float pivotX, float pivotY, float angleDegrees)
		{
			if (!upload_bitmap(bitmap))
			{
				return;
			}

			const float angleRadians = angleDegrees * pi / 180.0f;
			const float cosine = std::cos(angleRadians);
			const float sine = std::sin(angleRadians);
			const float originX = x + pivotX;
			const float originY = y + pivotY;
			const auto rotate = [originX, originY, cosine, sine](float dx, float dy)
			{
				return std::pair<float, float>{
					originX + dx * cosine - dy * sine,
					originY + dx * sine + dy * cosine};
			};

			const auto topLeft = rotate(-pivotX, -pivotY);
			const auto topRight = rotate(destinationWidth - pivotX, -pivotY);
			const auto bottomRight = rotate(destinationWidth - pivotX, destinationHeight - pivotY);
			const auto bottomLeft = rotate(-pivotX, destinationHeight - pivotY);

			const float leftTexture = static_cast<float>(sourceX) / bitmap->width;
			const float topTexture = static_cast<float>(sourceY) / bitmap->height;
			const float rightTexture = static_cast<float>(sourceX + width) / bitmap->width;
			const float bottomTexture = static_cast<float>(sourceY + height) / bitmap->height;

			detail::gl2d_begin(screen_width(), screen_height());
			if (detail::Renderer *renderer = detail::active_renderer())
				renderer->set_premultiplied_alpha(bitmap->fbo != 0);
			const detail::GLVertex vertices[4] = {
				{topLeft.first, topLeft.second, leftTexture, topTexture, 1.0f, 1.0f, 1.0f, 1.0f},
				{topRight.first, topRight.second, rightTexture, topTexture, 1.0f, 1.0f, 1.0f, 1.0f},
				{bottomRight.first, bottomRight.second, rightTexture, bottomTexture, 1.0f, 1.0f, 1.0f, 1.0f},
				{bottomLeft.first, bottomLeft.second, leftTexture, bottomTexture, 1.0f, 1.0f, 1.0f, 1.0f},
			};
			detail::gl2d_submit(detail::PrimitiveType::triangle_fan, vertices, 4, bitmap->gpu_texture);
		}

		/** Render a plain or textured line directly to the screen with optional thickness. */
		void draw_screen_thick_line(float x1, float y1, float x2, float y2, float thickness, Bitmap *texture, Colour colour)
		{
			if (thickness <= 1.0f)
			{
				float red = 1.0f, green = 1.0f, blue = 1.0f, alpha = 1.0f;
				std::uint32_t glTexture = 0;
				if (texture)
				{
					if (is_drawing_target(texture) || !upload_bitmap(texture))
						return;
					glTexture = texture->gpu_texture;
				}
				else
				{
					colour_components(colour, red, green, blue, alpha);
				}
				detail::gl2d_begin(screen_width(), screen_height());
				if (detail::Renderer *renderer = detail::active_renderer())
					renderer->set_premultiplied_alpha(texture && texture->fbo != 0);
				const detail::GLVertex vertices[2] = {
					{x1, y1, 0.0f, 0.0f, red, green, blue, alpha},
					{x2, y2, 1.0f, 1.0f, red, green, blue, alpha},
				};
				detail::gl2d_submit(detail::PrimitiveType::lines, vertices, 2, glTexture);
				return;
			}

			const float dx = x2 - x1;
			const float dy = y2 - y1;
			const float len = std::sqrt(dx * dx + dy * dy);
			if (len < 1e-6f)
			{
				draw_screen_ellipse(x1, y1, thickness * 0.5f, thickness * 0.5f, true, texture, colour);
				return;
			}

			const float nx = -dy / len;
			const float ny = dx / len;
			const float hx = nx * (thickness * 0.5f);
			const float hy = ny * (thickness * 0.5f);

			float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
			std::uint32_t glTexture = 0;
			if (texture)
			{
				if (is_drawing_target(texture) || !upload_bitmap(texture))
					return;
				glTexture = texture->gpu_texture;
			}
			else
			{
				colour_components(colour, r, g, b, a);
			}

			detail::gl2d_begin(screen_width(), screen_height());
			if (detail::Renderer *renderer = detail::active_renderer())
				renderer->set_premultiplied_alpha(texture && texture->fbo != 0);

			const detail::GLVertex quad[4] = {
				{x1 - hx, y1 - hy, 0.0f, 0.0f, r, g, b, a},
				{x1 + hx, y1 + hy, 1.0f, 0.0f, r, g, b, a},
				{x2 + hx, y2 + hy, 1.0f, 1.0f, r, g, b, a},
				{x2 - hx, y2 - hy, 0.0f, 1.0f, r, g, b, a},
			};
			detail::gl2d_submit(detail::PrimitiveType::triangle_fan, quad, 4, glTexture);
		}

		/** Render a plain or textured ellipse directly to the screen. */
		void draw_screen_ellipse(float x, float y, float radiusX, float radiusY, bool filled, Bitmap *texture, Colour colour, float thickness)
		{
			std::uint32_t glTexture = 0;
			if (texture)
			{
				if (is_drawing_target(texture) || !upload_bitmap(texture))
				{
					return;
				}
				glTexture = texture->gpu_texture;
			}
			float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
			if (!texture)
			{
				colour_components(colour, r, g, b, a);
			}

			const int segments = std::max(16, std::min(256, static_cast<int>(std::max(radiusX, radiusY) * 2.0f)));
			detail::gl2d_begin(screen_width(), screen_height());
			if (detail::Renderer *renderer = detail::active_renderer())
				renderer->set_premultiplied_alpha(texture && texture->fbo != 0);

			if (filled)
			{
				std::vector<detail::GLVertex> vertices;
				vertices.reserve(static_cast<std::size_t>(segments) + 2);
				vertices.push_back({x, y, 0.5f, 0.5f, r, g, b, a});
				for (int index = 0; index <= segments; ++index)
				{
					const float angle = 2.0f * pi * index / segments;
					const float u = 0.5f + 0.5f * std::cos(angle);
					const float v = 0.5f + 0.5f * std::sin(angle);
					vertices.push_back({x + radiusX * std::cos(angle), y + radiusY * std::sin(angle), u, v, r, g, b, a});
				}
				detail::gl2d_submit(detail::PrimitiveType::triangle_fan, vertices.data(), static_cast<int>(vertices.size()), glTexture);
			}
			else if (thickness <= 1.0f)
			{
				std::vector<detail::GLVertex> vertices;
				vertices.reserve(static_cast<std::size_t>(segments));
				for (int index = 0; index < segments; ++index)
				{
					const float angle = 2.0f * pi * index / segments;
					const float u = 0.5f + 0.5f * std::cos(angle);
					const float v = 0.5f + 0.5f * std::sin(angle);
					vertices.push_back({x + radiusX * std::cos(angle), y + radiusY * std::sin(angle), u, v, r, g, b, a});
				}
				detail::gl2d_submit(detail::PrimitiveType::line_loop, vertices.data(), static_cast<int>(vertices.size()), glTexture);
			}
			else
			{
				const float rOutX = radiusX + thickness * 0.5f;
				const float rOutY = radiusY + thickness * 0.5f;
				const float rInX = std::max(0.0f, radiusX - thickness * 0.5f);
				const float rInY = std::max(0.0f, radiusY - thickness * 0.5f);
				if (rInX <= 0.0f || rInY <= 0.0f)
				{
					draw_screen_ellipse(x, y, rOutX, rOutY, true, texture, colour);
					return;
				}

				std::vector<detail::GLVertex> vertices;
				vertices.reserve(static_cast<std::size_t>(segments) * 6);
				for (int index = 0; index < segments; ++index)
				{
					const float angle0 = 2.0f * pi * index / segments;
					const float angle1 = 2.0f * pi * (index + 1) / segments;
					const float cos0 = std::cos(angle0), sin0 = std::sin(angle0);
					const float cos1 = std::cos(angle1), sin1 = std::sin(angle1);

					const detail::GLVertex o0{x + rOutX * cos0, y + rOutY * sin0, 0.5f + 0.5f * cos0, 0.5f + 0.5f * sin0, r, g, b, a};
					const detail::GLVertex i0{x + rInX * cos0, y + rInY * sin0, 0.5f + 0.5f * cos0, 0.5f + 0.5f * sin0, r, g, b, a};
					const detail::GLVertex o1{x + rOutX * cos1, y + rOutY * sin1, 0.5f + 0.5f * cos1, 0.5f + 0.5f * sin1, r, g, b, a};
					const detail::GLVertex i1{x + rInX * cos1, y + rInY * sin1, 0.5f + 0.5f * cos1, 0.5f + 0.5f * sin1, r, g, b, a};

					vertices.push_back(o0);
					vertices.push_back(i0);
					vertices.push_back(i1);
					vertices.push_back(o0);
					vertices.push_back(i1);
					vertices.push_back(o1);
				}
				detail::gl2d_submit(detail::PrimitiveType::triangles, vertices.data(), static_cast<int>(vertices.size()), glTexture);
			}
		}

		/** Render a plain or textured triangle directly to the screen. */
		void draw_screen_triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool filled, Bitmap *texture, Colour colour, float thickness)
		{
			if (filled || thickness <= 1.0f)
			{
				std::uint32_t glTexture = 0;
				if (texture)
				{
					if (is_drawing_target(texture) || !upload_bitmap(texture))
					{
						return;
					}
					glTexture = texture->gpu_texture;
				}
				float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
				if (!texture)
				{
					colour_components(colour, r, g, b, a);
				}

				detail::gl2d_begin(screen_width(), screen_height());
				if (detail::Renderer *renderer = detail::active_renderer())
					renderer->set_premultiplied_alpha(texture && texture->fbo != 0);
				const detail::GLVertex vertices[3] = {
					{x1, y1, 0.0f, 0.0f, r, g, b, a},
					{x2, y2, 1.0f, 0.0f, r, g, b, a},
					{x3, y3, 0.5f, 1.0f, r, g, b, a},
				};
				detail::gl2d_submit(filled ? detail::PrimitiveType::triangles : detail::PrimitiveType::line_loop, vertices, 3, glTexture);
				return;
			}

			draw_screen_thick_line(x1, y1, x2, y2, thickness, texture, colour);
			draw_screen_thick_line(x2, y2, x3, y3, thickness, texture, colour);
			draw_screen_thick_line(x3, y3, x1, y1, thickness, texture, colour);
		}

		/** Render a plain or textured rectangle directly to the screen. */
		void draw_screen_rect(float left, float top, float right, float bottom, bool filled, Bitmap *texture, Colour colour, float thickness)
		{
			if (left > right)
				std::swap(left, right);
			if (top > bottom)
				std::swap(top, bottom);

			if (filled || thickness <= 1.0f)
			{
				std::uint32_t glTexture = 0;
				if (texture)
				{
					if (is_drawing_target(texture) || !upload_bitmap(texture))
					{
						return;
					}
					glTexture = texture->gpu_texture;
				}
				float r = 1.0f, g = 1.0f, b = 1.0f, a = 1.0f;
				if (!texture)
				{
					colour_components(colour, r, g, b, a);
				}

				detail::gl2d_begin(screen_width(), screen_height());
				if (detail::Renderer *renderer = detail::active_renderer())
					renderer->set_premultiplied_alpha(texture && texture->fbo != 0);
				const detail::GLVertex vertices[4] = {
					{left, top, 0.0f, 0.0f, r, g, b, a},
					{right, top, 1.0f, 0.0f, r, g, b, a},
					{right, bottom, 1.0f, 1.0f, r, g, b, a},
					{left, bottom, 0.0f, 1.0f, r, g, b, a},
				};
				detail::gl2d_submit(filled ? detail::PrimitiveType::triangle_fan : detail::PrimitiveType::line_loop, vertices, 4, glTexture);
				return;
			}

			const float w = right - left;
			const float h = bottom - top;
			if (thickness >= w * 0.5f || thickness >= h * 0.5f)
			{
				draw_screen_rect(left, top, right, bottom, true, texture, colour);
				return;
			}

			draw_screen_rect(left, top, right, top + thickness, true, texture, colour);
			draw_screen_rect(left, bottom - thickness, right, bottom, true, texture, colour);
			draw_screen_rect(left, top + thickness, left + thickness, bottom - thickness, true, texture, colour);
			draw_screen_rect(right - thickness, top + thickness, right, bottom - thickness, true, texture, colour);
		}

		/** Rasterize a line into a bitmap using integer coordinates and optional thickness. */
		void draw_line(Bitmap *bitmap, int x1, int y1, int x2, int y2, Colour colour, float thickness = 1.0f)
		{
			if (!bitmap)
				return;
			if (thickness <= 1.0f)
			{
				const int deltaX = std::abs(x2 - x1);
				const int stepX = x1 < x2 ? 1 : -1;
				const int deltaY = -std::abs(y2 - y1);
				const int stepY = y1 < y2 ? 1 : -1;
				int error = deltaX + deltaY;
				for (;;)
				{
					putpixel(bitmap, x1, y1, colour);
					if (x1 == x2 && y1 == y2)
					{
						return;
					}
					const int doubledError = error * 2;
					if (doubledError >= deltaY)
					{
						error += deltaY;
						x1 += stepX;
					}
					if (doubledError <= deltaX)
					{
						error += deltaX;
						y1 += stepY;
					}
				}
			}
			else
			{
				const float radius = (thickness - 1.0f) * 0.5f;
				const int deltaX = std::abs(x2 - x1);
				const int stepX = x1 < x2 ? 1 : -1;
				const int deltaY = -std::abs(y2 - y1);
				const int stepY = y1 < y2 ? 1 : -1;
				int error = deltaX + deltaY;
				for (;;)
				{
					circlefill(bitmap, static_cast<float>(x1), static_cast<float>(y1), radius, colour);
					if (x1 == x2 && y1 == y2)
					{
						return;
					}
					const int doubledError = error * 2;
					if (doubledError >= deltaY)
					{
						error += deltaY;
						x1 += stepX;
					}
					if (doubledError <= deltaX)
					{
						error += deltaX;
						y1 += stepY;
					}
				}
			}
		}

	} // namespace

	/** Create a RAM-backed bitmap. */
	Bitmap *create_bitmap(int width, int height)
	{
		if (width <= 0 || height <= 0)
		{
			sl::detail::set_error("Bitmap dimensions must be positive");
			return nullptr;
		}
		Bitmap *bitmap = new Bitmap;
		bitmap->width = width;
		bitmap->height = height;
		bitmap->pixels.resize(static_cast<std::size_t>(width) * height * bytes_per_pixel);
		bitmap->ram_dirty = true;
		return bitmap;
	}

	/** Create a GPU-backed bitmap. */
	Bitmap *create_video_bitmap(int width, int height)
	{
		Bitmap *bitmap = create_bitmap(width, height);
		if (!bitmap || !upload_bitmap(bitmap))
		{
			destroy_bitmap(bitmap);
			return nullptr;
		}
		bitmap->pixels.clear();
		bitmap->pixels.shrink_to_fit();
		bitmap->ram_dirty = false;
		return bitmap;
	}

	/** Create an offscreen render target: a GPU texture with a framebuffer attached. */
	Bitmap *create_render_target(int width, int height)
	{
		if (width <= 0 || height <= 0)
		{
			sl::detail::set_error("Render target dimensions must be positive");
			return nullptr;
		}

		detail::Renderer *renderer = detail::active_renderer();
		std::uint32_t texture = 0;
		std::uint32_t fbo = 0;
		if (!renderer || !renderer->create_render_target(width, height, texture, fbo))
		{
			sl::detail::set_error("Unable to create framebuffer for render target");
			return nullptr;
		}

		Bitmap *bitmap = new Bitmap;
		bitmap->width = width;
		bitmap->height = height;
		bitmap->gpu_texture = texture;
		bitmap->fbo = fbo;
		return bitmap;
	}

	/** Redirect subsequent GPU drawing to a render target's framebuffer. */
	bool begin_render_target(Bitmap *target)
	{
		if (!target || target->fbo == 0)
		{
			return false;
		}
		detail::Renderer *renderer = detail::active_renderer();
		std::string error;
		if (!renderer || !renderer->begin_render_target(target->fbo, target->width, target->height, error))
		{
			if (!error.empty())
				sl::detail::set_error(error);
			return false;
		}
		detail::set_render_target_size(target->width, target->height);
		active_target_stack.push_back(target);
		return true;
	}

	/** Stop rendering to a target and restore drawing to the window. */
	void end_render_target()
	{
		if (!active_target_stack.empty())
			active_target_stack.pop_back();
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			std::string error;
			renderer->end_render_target(error);
			if (!error.empty())
				sl::detail::set_error(error);
		}
		detail::set_render_target_size(
			active_target_stack.empty() ? 0 : active_target_stack.back()->width,
			active_target_stack.empty() ? 0 : active_target_stack.back()->height);
	}

	/** Clear the currently bound render target. */
	void clear_render_target(Colour colour)
	{
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			renderer->clear_frame(
				static_cast<float>(colour.red) / 255.0f,
				static_cast<float>(colour.green) / 255.0f,
				static_cast<float>(colour.blue) / 255.0f,
				static_cast<float>(colour.alpha) / 255.0f);
		}
	}

	/** Load an image file into a bitmap. */
	Bitmap *load_bitmap_from_surface(SDL_Surface *loaded)
	{
		if (!loaded)
		{
			return nullptr;
		}

		SDL_Surface *rgba = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA32, 0);
		SDL_FreeSurface(loaded);
		if (!rgba)
		{
			sl::detail::set_error(SDL_GetError());
			return nullptr;
		}

		Bitmap *bitmap = create_bitmap(rgba->w, rgba->h);
		if (bitmap)
		{
			const std::size_t rowBytes = static_cast<std::size_t>(rgba->w) * bytes_per_pixel;
			const auto *sourcePixels = static_cast<const Uint8 *>(rgba->pixels);
			for (int y = 0; y < rgba->h; ++y)
			{
				std::memcpy(bitmap->pixels.data() + static_cast<std::size_t>(y) * rowBytes, sourcePixels + static_cast<std::size_t>(y) * rgba->pitch, rowBytes);
			}
		}
		SDL_FreeSurface(rgba);
		return bitmap;
	}

	Bitmap *load_bitmap(const std::string &path)
	{
		SDL_Surface *loaded = IMG_Load(path.c_str());
		if (!loaded)
			sl::detail::set_error(IMG_GetError());
		return load_bitmap_from_surface(loaded);
	}

	Bitmap *load_bitmap_from_memory(const std::uint8_t *data, std::size_t size)
	{
		if (!data || size == 0 || size > std::numeric_limits<int>::max())
			return nullptr;
		SDL_RWops *rw = SDL_RWFromConstMem(data, static_cast<int>(size));
		if (!rw)
		{
			sl::detail::set_error(SDL_GetError());
			return nullptr;
		}
		SDL_Surface *loaded = IMG_Load_RW(rw, 1);
		if (!loaded)
			sl::detail::set_error(IMG_GetError());
		return load_bitmap_from_surface(loaded);
	}

	Bitmap *load_bitmap(const Archive &archive, const std::string &name)
	{
		const auto bytes = archive.read(name);
		return load_bitmap_from_memory(bytes.data(), bytes.size());
	}

	/** Save a bitmap as an uncompressed PNG. */
	bool save_bitmap(Bitmap *bitmap, const std::string &path)
	{
		if (!bitmap || path.empty() || !acquire_bitmap(bitmap))
		{
			return false;
		}

		std::filesystem::path outputPath(path);
		if (outputPath.extension().empty())
		{
			outputPath += ".png";
		}

		FILE *file = std::fopen(outputPath.string().c_str(), "wb");
		if (!file)
		{
			sl::detail::set_error("Unable to open bitmap output file");
			return false;
		}

		png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
		png_infop info = png ? png_create_info_struct(png) : nullptr;
		if (!png || !info)
		{
			png_destroy_write_struct(png ? &png : nullptr, info ? &info : nullptr);
			std::fclose(file);
			return false;
		}

		if (setjmp(png_jmpbuf(png)))
		{
			png_destroy_write_struct(&png, &info);
			std::fclose(file);
			return false;
		}

		png_init_io(png, file);
		png_set_compression_level(png, 0);
		png_set_filter(png, 0, PNG_FILTER_NONE);
		png_set_IHDR(
			png,
			info,
			static_cast<png_uint_32>(bitmap->width),
			static_cast<png_uint_32>(bitmap->height),
			8,
			PNG_COLOR_TYPE_RGBA,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE,
			PNG_FILTER_TYPE_BASE);

		const std::size_t rowBytes = static_cast<std::size_t>(bitmap->width) * bytes_per_pixel;
		png_write_info(png, info);
		for (int y = 0; y < bitmap->height; ++y)
		{
			png_write_row(png, bitmap->pixels.data() + static_cast<std::size_t>(y) * rowBytes);
		}
		png_write_end(png, info);

		png_destroy_write_struct(&png, &info);
		std::fclose(file);
		return true;
	}

	/** Destroy a bitmap and its GPU resources. */
	void destroy_bitmap(Bitmap *bitmap)
	{
		if (!bitmap || is_screen(bitmap))
		{
			return;
		}
		detail::Renderer *renderer = detail::active_renderer();
		if (renderer && bitmap->fbo != 0)
		{
			renderer->destroy_render_target(bitmap->gpu_texture, bitmap->fbo);
		}
		else if (renderer && bitmap->gpu_texture != 0)
		{
			renderer->destroy_texture(bitmap->gpu_texture);
		}
		delete bitmap;
	}

	/** Make a bitmap's pixels available in RAM. */
	bool acquire_bitmap(Bitmap *bitmap)
	{
		if (is_screen(bitmap))
		{
			return download_bitmap(bitmap);
		}
		return ensure_ram_pixels(bitmap);
	}

	/** Upload a bitmap's modified RAM pixels to the GPU. */
	bool release_bitmap(Bitmap *bitmap)
	{
		if (is_screen(bitmap))
		{
			return false;
		}
		return upload_bitmap(bitmap);
	}

	/** Fill a bitmap with one colour. */
	void clear_to_colour(Bitmap *bitmap, Colour colour)
	{
		if (is_drawing_target(bitmap))
		{
			if (detail::Renderer *renderer = detail::active_renderer())
			{
				renderer->clear_frame(
					static_cast<float>(colour.red) / 255.0f,
					static_cast<float>(colour.green) / 255.0f,
					static_cast<float>(colour.blue) / 255.0f,
					static_cast<float>(colour.alpha) / 255.0f);
			}
			return;
		}
		if (!ensure_ram_pixels(bitmap))
		{
			return;
		}
		const std::uint32_t packed = static_cast<std::uint32_t>(colour.red) |
									 (static_cast<std::uint32_t>(colour.green) << 8) |
									 (static_cast<std::uint32_t>(colour.blue) << 16) |
									 (static_cast<std::uint32_t>(colour.alpha) << 24);
		std::uint32_t *dst = reinterpret_cast<std::uint32_t *>(bitmap->pixels.data());
		const std::size_t count = bitmap->pixels.size() / 4;
		std::fill_n(dst, count, packed);
		bitmap->ram_dirty = true;
	}

	/** Set one pixel in a bitmap. */
	void putpixel(Bitmap *bitmap, int x, int y, Colour colour)
	{
		if (is_drawing_target(bitmap))
		{
			if (x < 0 || x >= bitmap->width || y < 0 || y >= bitmap->height)
			{
				return;
			}
			float r, g, b, a;
			colour_components(colour, r, g, b, a);
			detail::gl2d_begin(screen_width(), screen_height());
			const detail::GLVertex vertex{static_cast<float>(x) + 0.5f, static_cast<float>(y) + 0.5f, 0.0f, 0.0f, r, g, b, a};
			detail::gl2d_submit(detail::PrimitiveType::points, &vertex, 1);
			return;
		}
		if (!ensure_ram_pixels(bitmap) || x < 0 || x >= bitmap->width || y < 0 || y >= bitmap->height)
		{
			return;
		}
		const std::size_t offset = pixel_offset(*bitmap, x, y);
		bitmap->pixels[offset] = colour.red;
		bitmap->pixels[offset + 1] = colour.green;
		bitmap->pixels[offset + 2] = colour.blue;
		bitmap->pixels[offset + 3] = colour.alpha;
		bitmap->ram_dirty = true;
	}

	/** Read one pixel from a bitmap. */
	Colour getpixel(Bitmap *bitmap, int x, int y)
	{
		if (!ensure_ram_pixels(bitmap) || x < 0 || x >= bitmap->width || y < 0 || y >= bitmap->height)
		{
			return {};
		}
		const std::size_t offset = pixel_offset(*bitmap, x, y);
		return {bitmap->pixels[offset], bitmap->pixels[offset + 1], bitmap->pixels[offset + 2], bitmap->pixels[offset + 3]};
	}

	/** Draw many single-pixel points in one batched GPU submission (e.g. GPU particle systems). */
	void draw_points(Bitmap *bitmap, const PointVertex *points, int count)
	{
		if (!is_drawing_target(bitmap) || !points || count <= 0)
		{
			return;
		}

		// Reused across calls so drawing 100k+ points per frame does not reallocate.
		static std::vector<detail::GLVertex> vertices;
		vertices.resize(static_cast<std::size_t>(count));
		for (int i = 0; i < count; ++i)
		{
			float r, g, b, a;
			colour_components(points[i].colour, r, g, b, a);
			vertices[static_cast<std::size_t>(i)] = detail::GLVertex{points[i].x, points[i].y, 0.0f, 0.0f, r, g, b, a};
		}

		detail::gl2d_begin(screen_width(), screen_height());
		detail::gl2d_submit(detail::PrimitiveType::points, vertices.data(), count);
	}

	/** Select the blend mode used when compositing subsequent draws onto the screen. */
	void set_blend_mode(BlendMode mode)
	{
		if (detail::Renderer *renderer = detail::active_renderer())
		{
			renderer->set_blend_mode(mode);
		}
	}

	/** Draw an outline circle. */
	void circle(Bitmap *bitmap, float x, float y, float radius, Colour colour, float thickness)
	{
		if (!bitmap || radius < 0.0f)
			return;
		if (is_drawing_target(bitmap))
		{
			draw_screen_ellipse(x, y, radius, radius, false, nullptr, colour, thickness);
			return;
		}
		const int ix = static_cast<int>(std::lround(x));
		const int iy = static_cast<int>(std::lround(y));
		const float halfThick = (thickness - 1.0f) * 0.5f;
		for (int degrees = 0; degrees < 360; ++degrees)
		{
			const float angle = degrees * pi / 180.0f;
			const float px = ix + radius * std::cos(angle);
			const float py = iy + radius * std::sin(angle);
			if (thickness <= 1.0f)
				putpixel(bitmap, static_cast<int>(std::lround(px)), static_cast<int>(std::lround(py)), colour);
			else
				circlefill(bitmap, px, py, halfThick, colour);
		}
		sync_render_target(bitmap);
	}

	/** Draw a filled circle. */
	void circlefill(Bitmap *bitmap, float x, float y, float radius, Colour colour)
	{
		if (!bitmap || radius < 0.0f)
			return;
		if (is_drawing_target(bitmap))
		{
			draw_screen_ellipse(x, y, radius, radius, true, nullptr, colour);
			return;
		}
		const int ix = static_cast<int>(std::lround(x));
		const int iy = static_cast<int>(std::lround(y));
		const int iradius = static_cast<int>(std::lround(radius));
		for (int offsetY = -iradius; offsetY <= iradius; ++offsetY)
		{
			const int halfWidth = static_cast<int>(std::sqrt(iradius * iradius - offsetY * offsetY));
			for (int offsetX = -halfWidth; offsetX <= halfWidth; ++offsetX)
				putpixel(bitmap, ix + offsetX, iy + offsetY, colour);
		}
		sync_render_target(bitmap);
	}

	/** Draw an outline rectangle. */
	void rect(Bitmap *bitmap, float left, float top, float right, float bottom, Colour colour, float thickness)
	{
		if (is_drawing_target(bitmap))
		{
			draw_screen_rect(left, top, right, bottom, false, nullptr, colour, thickness);
			return;
		}
		const int ileft = static_cast<int>(std::lround(left));
		const int itop = static_cast<int>(std::lround(top));
		const int iright = static_cast<int>(std::lround(right));
		const int ibottom = static_cast<int>(std::lround(bottom));
		draw_line(bitmap, ileft, itop, iright, itop, colour, thickness);
		draw_line(bitmap, iright, itop, iright, ibottom, colour, thickness);
		draw_line(bitmap, iright, ibottom, ileft, ibottom, colour, thickness);
		draw_line(bitmap, ileft, ibottom, ileft, itop, colour, thickness);
		sync_render_target(bitmap);
	}

	/** Draw a filled rectangle. */
	void rectfill(Bitmap *bitmap, float left, float top, float right, float bottom, Colour colour)
	{
		if (is_drawing_target(bitmap))
		{
			const float w = static_cast<float>(screen_width());
			const float h = static_cast<float>(screen_height());
			left = std::clamp(left, 0.0f, w);
			right = std::clamp(right, 0.0f, w);
			top = std::clamp(top, 0.0f, h);
			bottom = std::clamp(bottom, 0.0f, h);
			draw_screen_rect(left, top, right, bottom, true, nullptr, colour);
			return;
		}
		if (!ensure_ram_pixels(bitmap))
		{
			return;
		}
		const int ileft = std::clamp(static_cast<int>(std::lround(left)), 0, bitmap->width);
		const int iright = std::clamp(static_cast<int>(std::lround(right)), 0, bitmap->width);
		const int itop = std::clamp(static_cast<int>(std::lround(top)), 0, bitmap->height);
		const int ibottom = std::clamp(static_cast<int>(std::lround(bottom)), 0, bitmap->height);
		for (int y = itop; y < ibottom; ++y)
		{
			for (int x = ileft; x < iright; ++x)
			{
				putpixel(bitmap, x, y, colour);
			}
		}
		sync_render_target(bitmap);
	}

	/** Draw an outline ellipse. */
	void ellipse(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Colour colour, float thickness)
	{
		if (!bitmap || radiusX < 0.0f || radiusY < 0.0f)
			return;
		if (is_drawing_target(bitmap))
		{
			draw_screen_ellipse(x, y, radiusX, radiusY, false, nullptr, colour, thickness);
			return;
		}
		const int ix = static_cast<int>(std::lround(x));
		const int iy = static_cast<int>(std::lround(y));
		const float halfThick = (thickness - 1.0f) * 0.5f;
		for (int degrees = 0; degrees < 360; ++degrees)
		{
			const float angle = degrees * pi / 180.0f;
			const float px = ix + radiusX * std::cos(angle);
			const float py = iy + radiusY * std::sin(angle);
			if (thickness <= 1.0f)
				putpixel(bitmap, static_cast<int>(std::lround(px)), static_cast<int>(std::lround(py)), colour);
			else
				circlefill(bitmap, px, py, halfThick, colour);
		}
		sync_render_target(bitmap);
	}

	/** Draw a filled ellipse. */
	void ellipsefill(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Colour colour)
	{
		if (!bitmap || radiusX < 0.0f || radiusY < 0.0f)
			return;
		if (is_drawing_target(bitmap))
		{
			draw_screen_ellipse(x, y, radiusX, radiusY, true, nullptr, colour);
			return;
		}
		const int ix = static_cast<int>(std::lround(x));
		const int iy = static_cast<int>(std::lround(y));
		const int iradiusY = static_cast<int>(std::lround(radiusY));
		for (int offsetY = -iradiusY; offsetY <= iradiusY; ++offsetY)
		{
			const float ratio = iradiusY == 0 ? 0.0f : static_cast<float>(offsetY) / iradiusY;
			const int halfWidth = static_cast<int>(std::sqrt(std::max(0.0f, 1.0f - ratio * ratio)) * radiusX);
			for (int offsetX = -halfWidth; offsetX <= halfWidth; ++offsetX)
				putpixel(bitmap, ix + offsetX, iy + offsetY, colour);
		}
		sync_render_target(bitmap);
	}

	/** Draw an outline triangle. */
	void triangle(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Colour colour, float thickness)
	{
		if (is_drawing_target(bitmap))
		{
			draw_screen_triangle(x1, y1, x2, y2, x3, y3, false, nullptr, colour, thickness);
			return;
		}
		draw_line(bitmap, static_cast<int>(std::lround(x1)), static_cast<int>(std::lround(y1)), static_cast<int>(std::lround(x2)), static_cast<int>(std::lround(y2)), colour, thickness);
		draw_line(bitmap, static_cast<int>(std::lround(x2)), static_cast<int>(std::lround(y2)), static_cast<int>(std::lround(x3)), static_cast<int>(std::lround(y3)), colour, thickness);
		draw_line(bitmap, static_cast<int>(std::lround(x3)), static_cast<int>(std::lround(y3)), static_cast<int>(std::lround(x1)), static_cast<int>(std::lround(y1)), colour, thickness);
		sync_render_target(bitmap);
	}

	/** Draw a filled triangle. */
	void trianglefill(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Colour colour)
	{
		if (is_drawing_target(bitmap))
		{
			draw_screen_triangle(x1, y1, x2, y2, x3, y3, true, nullptr, colour);
			return;
		}
		const int ix1 = static_cast<int>(std::lround(x1));
		const int iy1 = static_cast<int>(std::lround(y1));
		const int ix2 = static_cast<int>(std::lround(x2));
		const int iy2 = static_cast<int>(std::lround(y2));
		const int ix3 = static_cast<int>(std::lround(x3));
		const int iy3 = static_cast<int>(std::lround(y3));
		const int minimumX = std::min({ix1, ix2, ix3});
		const int maximumX = std::max({ix1, ix2, ix3});
		const int minimumY = std::min({iy1, iy2, iy3});
		const int maximumY = std::max({iy1, iy2, iy3});
		const int area = (ix2 - ix1) * (iy3 - iy1) - (iy2 - iy1) * (ix3 - ix1);
		if (area == 0)
			return;
		for (int y = minimumY; y <= maximumY; ++y)
			for (int x = minimumX; x <= maximumX; ++x)
			{
				const int a = (ix2 - ix1) * (y - iy1) - (iy2 - iy1) * (x - ix1);
				const int b = (ix3 - ix2) * (y - iy2) - (iy3 - iy2) * (x - ix2);
				const int c = (ix1 - ix3) * (y - iy3) - (iy1 - iy3) * (x - ix3);
				if ((a >= 0 && b >= 0 && c >= 0) || (a <= 0 && b <= 0 && c <= 0))
					putpixel(bitmap, x, y, colour);
			}
		sync_render_target(bitmap);
	}

	/** Draw a line between two points with optional thickness. */
	void line(Bitmap *bitmap, float x1, float y1, float x2, float y2, Colour colour, float thickness)
	{
		if (!bitmap)
		{
			return;
		}
		if (is_drawing_target(bitmap))
		{
			draw_screen_thick_line(x1, y1, x2, y2, thickness, nullptr, colour);
			return;
		}
		draw_line(bitmap, static_cast<int>(std::lround(x1)), static_cast<int>(std::lround(y1)), static_cast<int>(std::lround(x2)), static_cast<int>(std::lround(y2)), colour, thickness);
		sync_render_target(bitmap);
	}

	int calc_spline(const int points[8], int npts, int *xout, int *yout)
	{
		if (!points || npts <= 0 || !xout || !yout)
			return 0;
		const float x0 = static_cast<float>(points[0]);
		const float y0 = static_cast<float>(points[1]);
		const float x1 = static_cast<float>(points[2]);
		const float y1 = static_cast<float>(points[3]);
		const float x2 = static_cast<float>(points[4]);
		const float y2 = static_cast<float>(points[5]);
		const float x3 = static_cast<float>(points[6]);
		const float y3 = static_cast<float>(points[7]);

		for (int i = 0; i < npts; ++i)
		{
			const float t = npts > 1 ? static_cast<float>(i) / static_cast<float>(npts - 1) : 0.0f;
			const float omt = 1.0f - t;
			const float omt2 = omt * omt;
			const float omt3 = omt2 * omt;
			const float t2 = t * t;
			const float t3 = t2 * t;

			const float px = omt3 * x0 + 3.0f * omt2 * t * x1 + 3.0f * omt * t2 * x2 + t3 * x3;
			const float py = omt3 * y0 + 3.0f * omt2 * t * y1 + 3.0f * omt * t2 * y2 + t3 * y3;

			xout[i] = static_cast<int>(std::lround(px));
			yout[i] = static_cast<int>(std::lround(py));
		}
		return npts;
	}

	int calc_spline(const float points[8], int npts, float *xout, float *yout)
	{
		if (!points || npts <= 0 || !xout || !yout)
			return 0;
		const float x0 = points[0];
		const float y0 = points[1];
		const float x1 = points[2];
		const float y1 = points[3];
		const float x2 = points[4];
		const float y2 = points[5];
		const float x3 = points[6];
		const float y3 = points[7];

		for (int i = 0; i < npts; ++i)
		{
			const float t = npts > 1 ? static_cast<float>(i) / static_cast<float>(npts - 1) : 0.0f;
			const float omt = 1.0f - t;
			const float omt2 = omt * omt;
			const float omt3 = omt2 * omt;
			const float t2 = t * t;
			const float t3 = t2 * t;

			xout[i] = omt3 * x0 + 3.0f * omt2 * t * x1 + 3.0f * omt * t2 * x2 + t3 * x3;
			yout[i] = omt3 * y0 + 3.0f * omt2 * t * y1 + 3.0f * omt * t2 * y2 + t3 * y3;
		}
		return npts;
	}

	void arc(Bitmap *bitmap, float x, float y, float startAngle, float endAngle, float radius, Colour colour, float thickness)
	{
		if (!bitmap || radius < 0.0f)
			return;

		// Convert angles to radians (0 degrees = right, 90 degrees = bottom in screen coords)
		constexpr float deg_to_rad = 3.14159265358979323846f / 180.0f;
		float a1 = startAngle * deg_to_rad;
		float a2 = endAngle * deg_to_rad;
		while (a2 < a1)
			a2 += 3.14159265358979323846f * 2.0f;

		const float angleRange = a2 - a1;
		const int segments = std::max(8, static_cast<int>(std::ceil(angleRange * radius / 4.0f)));

		float prev_x = x + std::cos(a1) * radius;
		float prev_y = y + std::sin(a1) * radius;
		const float step = angleRange / static_cast<float>(segments);
		for (int i = 1; i <= segments; ++i)
		{
			const float ang = a1 + step * static_cast<float>(i);
			const float curr_x = x + std::cos(ang) * radius;
			const float curr_y = y + std::sin(ang) * radius;
			line(bitmap, prev_x, prev_y, curr_x, curr_y, colour, thickness);
			prev_x = curr_x;
			prev_y = curr_y;
		}
	}

	void spline(Bitmap *bitmap, const int points[8], Colour colour, float thickness)
	{
		if (!bitmap || !points)
			return;
		float fpts[8];
		for (int i = 0; i < 8; ++i)
			fpts[i] = static_cast<float>(points[i]);
		spline(bitmap, fpts, colour, thickness);
	}

	void spline(Bitmap *bitmap, const float points[8], Colour colour, float thickness)
	{
		if (!bitmap || !points)
			return;
		constexpr int segments = 32;
		float xout[segments + 1];
		float yout[segments + 1];
		calc_spline(points, segments + 1, xout, yout);

		for (int i = 0; i < segments; ++i)
		{
			line(bitmap, xout[i], yout[i], xout[i + 1], yout[i + 1], colour, thickness);
		}
	}

	void do_line(Bitmap *bitmap, int x1, int y1, int x2, int y2, int d, const PixelProc &proc)
	{
		if (!proc)
			return;
		const int deltaX = std::abs(x2 - x1);
		const int stepX = x1 < x2 ? 1 : -1;
		const int deltaY = -std::abs(y2 - y1);
		const int stepY = y1 < y2 ? 1 : -1;
		int error = deltaX + deltaY;
		for (;;)
		{
			proc(bitmap, x1, y1, d);
			if (x1 == x2 && y1 == y2)
			{
				return;
			}
			const int doubledError = error * 2;
			if (doubledError >= deltaY)
			{
				error += deltaY;
				x1 += stepX;
			}
			if (doubledError <= deltaX)
			{
				error += deltaX;
				y1 += stepY;
			}
		}
	}

	void do_circle(Bitmap *bitmap, int x, int y, int radius, int d, const PixelProc &proc)
	{
		if (!proc || radius < 0)
			return;
		int cx = 0;
		int cy = radius;
		int p = 1 - radius;

		auto plot = [&](int px, int py)
		{
			proc(bitmap, x + px, y + py, d);
			if (px != 0)
				proc(bitmap, x - px, y + py, d);
			if (py != 0)
				proc(bitmap, x + px, y - py, d);
			if (px != 0 && py != 0)
				proc(bitmap, x - px, y - py, d);
			if (px != py)
			{
				proc(bitmap, x + py, y + px, d);
				if (px != 0)
					proc(bitmap, x + py, y - px, d);
				if (py != 0)
					proc(bitmap, x - py, y + px, d);
				if (px != 0 && py != 0)
					proc(bitmap, x - py, y - px, d);
			}
		};

		plot(cx, cy);
		while (cx < cy)
		{
			cx++;
			if (p < 0)
			{
				p += 2 * cx + 1;
			}
			else
			{
				cy--;
				p += 2 * (cx - cy) + 1;
			}
			plot(cx, cy);
		}
	}

	void do_ellipse(Bitmap *bitmap, int x, int y, int radiusX, int radiusY, int d, const PixelProc &proc)
	{
		if (!proc || radiusX < 0 || radiusY < 0)
			return;
		long rx2 = static_cast<long>(radiusX) * radiusX;
		long ry2 = static_cast<long>(radiusY) * radiusY;
		long two_rx2 = 2 * rx2;
		long two_ry2 = 2 * ry2;
		long px = 0;
		long py = two_rx2 * radiusY;

		auto plot = [&](int ex, int ey)
		{
			proc(bitmap, x + ex, y + ey, d);
			if (ex != 0)
				proc(bitmap, x - ex, y + ey, d);
			if (ey != 0)
				proc(bitmap, x + ex, y - ey, d);
			if (ex != 0 && ey != 0)
				proc(bitmap, x - ex, y - ey, d);
		};

		// Region 1
		int cx = 0;
		int cy = radiusY;
		long p = static_cast<long>(std::lround(ry2 - (rx2 * radiusY) + (0.25 * rx2)));
		plot(cx, cy);

		while (px < py)
		{
			cx++;
			px += two_ry2;
			if (p < 0)
			{
				p += ry2 + px;
			}
			else
			{
				cy--;
				py -= two_rx2;
				p += ry2 + px - py;
			}
			plot(cx, cy);
		}

		// Region 2
		p = static_cast<long>(std::lround(ry2 * (cx + 0.5) * (cx + 0.5) + rx2 * (cy - 1) * (cy - 1) - rx2 * ry2));
		while (cy > 0)
		{
			cy--;
			py -= two_rx2;
			if (p > 0)
			{
				p += rx2 - py;
			}
			else
			{
				cx++;
				px += two_ry2;
				p += rx2 - py + px;
			}
			plot(cx, cy);
		}
	}

	void do_arc(Bitmap *bitmap, int x, int y, float startAngle, float endAngle, float radius, int d, const PixelProc &proc)
	{
		if (!proc || radius < 0.0f)
			return;
		constexpr float deg_to_rad = 3.14159265358979323846f / 180.0f;
		float a1 = startAngle * deg_to_rad;
		float a2 = endAngle * deg_to_rad;
		while (a2 < a1)
			a2 += 3.14159265358979323846f * 2.0f;

		const float angleRange = a2 - a1;
		const int segments = std::max(8, static_cast<int>(std::ceil(angleRange * radius / 4.0f)));
		const float step = angleRange / static_cast<float>(segments);

		int prev_x = x + static_cast<int>(std::lround(std::cos(a1) * radius));
		int prev_y = y + static_cast<int>(std::lround(std::sin(a1) * radius));

		for (int i = 1; i <= segments; ++i)
		{
			const float ang = a1 + step * static_cast<float>(i);
			const int curr_x = x + static_cast<int>(std::lround(std::cos(ang) * radius));
			const int curr_y = y + static_cast<int>(std::lround(std::sin(ang) * radius));
			do_line(bitmap, prev_x, prev_y, curr_x, curr_y, d, proc);
			prev_x = curr_x;
			prev_y = curr_y;
		}
	}

	void do_spline(Bitmap *bitmap, const int points[8], int d, const PixelProc &proc)
	{
		if (!proc || !points)
			return;
		constexpr int segments = 32;
		int xout[segments + 1];
		int yout[segments + 1];
		calc_spline(points, segments + 1, xout, yout);

		for (int i = 0; i < segments; ++i)
		{
			do_line(bitmap, xout[i], yout[i], xout[i + 1], yout[i + 1], d, proc);
		}
	}

	/** Flood-fill an enclosed area of a bitmap starting at (x, y) with a replacement colour. */
	void flood_fill(Bitmap *bitmap, int x, int y, Colour colour)
	{
		if (!bitmap || !ensure_ram_pixels(bitmap))
			return;
		if (x < 0 || x >= bitmap->width || y < 0 || y >= bitmap->height)
			return;

		const std::size_t stride = static_cast<std::size_t>(bitmap->width) * bytes_per_pixel;
		const std::size_t start_offset = static_cast<std::size_t>(y) * stride + static_cast<std::size_t>(x) * bytes_per_pixel;
		const Colour target{
			bitmap->pixels[start_offset + 0],
			bitmap->pixels[start_offset + 1],
			bitmap->pixels[start_offset + 2],
			bitmap->pixels[start_offset + 3]};

		if (target.red == colour.red && target.green == colour.green &&
			target.blue == colour.blue && target.alpha == colour.alpha)
		{
			return;
		}

		std::queue<std::pair<int, int>> queue;
		queue.push({x, y});

		while (!queue.empty())
		{
			const auto [px, py] = queue.front();
			queue.pop();

			if (px < 0 || px >= bitmap->width || py < 0 || py >= bitmap->height)
				continue;

			const std::size_t offset = static_cast<std::size_t>(py) * stride + static_cast<std::size_t>(px) * bytes_per_pixel;
			if (bitmap->pixels[offset + 0] != target.red ||
				bitmap->pixels[offset + 1] != target.green ||
				bitmap->pixels[offset + 2] != target.blue ||
				bitmap->pixels[offset + 3] != target.alpha)
			{
				continue;
			}

			bitmap->pixels[offset + 0] = colour.red;
			bitmap->pixels[offset + 1] = colour.green;
			bitmap->pixels[offset + 2] = colour.blue;
			bitmap->pixels[offset + 3] = colour.alpha;

			queue.push({px + 1, py});
			queue.push({px - 1, py});
			queue.push({px, py + 1});
			queue.push({px, py - 1});
		}

		bitmap->ram_dirty = true;
		sync_render_target(bitmap);
	}

	/** Draw a textured circle outline. */
	void circle(Bitmap *bitmap, float x, float y, float radius, Bitmap *texture, float thickness)
	{
		if (is_drawing_target(bitmap) && radius >= 0.0f)
			draw_screen_ellipse(x, y, radius, radius, false, texture, {}, thickness);
	}
	/** Draw a textured filled circle. */
	void circlefill(Bitmap *bitmap, float x, float y, float radius, Bitmap *texture)
	{
		if (is_drawing_target(bitmap) && radius >= 0.0f)
			draw_screen_ellipse(x, y, radius, radius, true, texture, {});
	}
	/** Draw a textured rectangle outline. */
	void rect(Bitmap *bitmap, float left, float top, float right, float bottom, Bitmap *texture, float thickness)
	{
		if (is_drawing_target(bitmap))
			draw_screen_rect(left, top, right, bottom, false, texture, {}, thickness);
	}
	/** Draw a textured filled rectangle. */
	void rectfill(Bitmap *bitmap, float left, float top, float right, float bottom, Bitmap *texture)
	{
		if (is_drawing_target(bitmap))
			draw_screen_rect(left, top, right, bottom, true, texture, {});
	}
	/** Draw a textured ellipse outline. */
	void ellipse(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Bitmap *texture, float thickness)
	{
		if (is_drawing_target(bitmap) && radiusX >= 0.0f && radiusY >= 0.0f)
			draw_screen_ellipse(x, y, radiusX, radiusY, false, texture, {}, thickness);
	}
	/** Draw a textured filled ellipse. */
	void ellipsefill(Bitmap *bitmap, float x, float y, float radiusX, float radiusY, Bitmap *texture)
	{
		if (is_drawing_target(bitmap) && radiusX >= 0.0f && radiusY >= 0.0f)
			draw_screen_ellipse(x, y, radiusX, radiusY, true, texture, {});
	}
	/** Draw a textured triangle outline. */
	void triangle(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Bitmap *texture, float thickness)
	{
		if (is_drawing_target(bitmap))
			draw_screen_triangle(x1, y1, x2, y2, x3, y3, false, texture, {}, thickness);
	}
	/** Draw a textured filled triangle. */
	void trianglefill(Bitmap *bitmap, float x1, float y1, float x2, float y2, float x3, float y3, Bitmap *texture)
	{
		if (is_drawing_target(bitmap))
			draw_screen_triangle(x1, y1, x2, y2, x3, y3, true, texture, {});
	}

	/** Copy a rectangular bitmap region without scaling. */
	void blit(Bitmap *source, Bitmap *destination, int sourceX, int sourceY, float destinationX, float destinationY, int width, int height)
	{
		if (is_drawing_target(destination))
		{
			if (!source || is_drawing_target(source))
			{
				return;
			}
			const int clippedSourceX = std::max(sourceX, 0);
			const int clippedSourceY = std::max(sourceY, 0);
			const int clippedWidth = std::min(width - (clippedSourceX - sourceX), source->width - clippedSourceX);
			const int clippedHeight = std::min(height - (clippedSourceY - sourceY), source->height - clippedSourceY);
			if (clippedWidth > 0 && clippedHeight > 0)
			{
				draw_textured_quad(source, clippedSourceX, clippedSourceY, clippedWidth, clippedHeight, destinationX + static_cast<float>(clippedSourceX - sourceX), destinationY + static_cast<float>(clippedSourceY - sourceY));
			}
			return;
		}
		if (!ensure_ram_pixels(source) || !ensure_ram_pixels(destination) || width <= 0 || height <= 0)
		{
			return;
		}
		const int idestinationX = static_cast<int>(std::lround(destinationX));
		const int idestinationY = static_cast<int>(std::lround(destinationY));
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const int fromX = sourceX + x;
				const int fromY = sourceY + y;
				const int toX = idestinationX + x;
				const int toY = idestinationY + y;
				if (fromX >= 0 && fromX < source->width && fromY >= 0 && fromY < source->height && toX >= 0 && toX < destination->width && toY >= 0 && toY < destination->height)
				{
					const std::size_t sourceOffset = pixel_offset(*source, fromX, fromY);
					const std::size_t destinationOffset = pixel_offset(*destination, toX, toY);
					std::memcpy(destination->pixels.data() + destinationOffset, source->pixels.data() + sourceOffset, bytes_per_pixel);
				}
			}
		}
		destination->ram_dirty = true;
	}

	/** Copy a bitmap region while skipping transparent pixels. */
	void masked_blit(Bitmap *source, Bitmap *destination, int sourceX, int sourceY, float destinationX, float destinationY, int width, int height)
	{
		if (!source || !destination || width <= 0 || height <= 0 || !ensure_ram_pixels(source))
		{
			return;
		}
		const int idestinationX = static_cast<int>(std::lround(destinationX));
		const int idestinationY = static_cast<int>(std::lround(destinationY));
		if (is_drawing_target(destination))
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					const int sourcePixelX = sourceX + x;
					const int sourcePixelY = sourceY + y;
					if (sourcePixelX < 0 || sourcePixelX >= source->width || sourcePixelY < 0 || sourcePixelY >= source->height)
						continue;
					const std::size_t offset = pixel_offset(*source, sourcePixelX, sourcePixelY);
					if (source->pixels[offset + 3] != 0)
						putpixel(destination, idestinationX + x, idestinationY + y, {source->pixels[offset], source->pixels[offset + 1], source->pixels[offset + 2], source->pixels[offset + 3]});
				}
			}
			return;
		}
		if (!ensure_ram_pixels(destination))
			return;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				const int sourcePixelX = sourceX + x;
				const int sourcePixelY = sourceY + y;
				const int destinationPixelX = idestinationX + x;
				const int destinationPixelY = idestinationY + y;
				if (sourcePixelX < 0 || sourcePixelX >= source->width || sourcePixelY < 0 || sourcePixelY >= source->height || destinationPixelX < 0 || destinationPixelX >= destination->width || destinationPixelY < 0 || destinationPixelY >= destination->height)
					continue;
				const std::size_t sourceOffset = pixel_offset(*source, sourcePixelX, sourcePixelY);
				if (source->pixels[sourceOffset + 3] != 0)
					std::memcpy(destination->pixels.data() + pixel_offset(*destination, destinationPixelX, destinationPixelY), source->pixels.data() + sourceOffset, bytes_per_pixel);
			}
		}
		destination->ram_dirty = true;
	}

	/** Scale and copy a bitmap region. */
	void stretch_blit(Bitmap *source, Bitmap *destination, int sourceX, int sourceY, int sourceWidth, int sourceHeight, float destinationX, float destinationY, int destinationWidth, int destinationHeight)
	{
		if (!source || !destination || sourceWidth <= 0 || sourceHeight <= 0 || destinationWidth <= 0 || destinationHeight <= 0)
			return;
		if (is_drawing_target(destination))
		{
			draw_textured_quad(source, sourceX, sourceY, sourceWidth, sourceHeight, destinationX, destinationY, destinationWidth, destinationHeight);
			return;
		}
		if (!ensure_ram_pixels(source) || !ensure_ram_pixels(destination))
			return;
		const int idestinationX = static_cast<int>(std::lround(destinationX));
		const int idestinationY = static_cast<int>(std::lround(destinationY));
		for (int y = 0; y < destinationHeight; ++y)
		{
			for (int x = 0; x < destinationWidth; ++x)
			{
				const int sourcePixelX = sourceX + x * sourceWidth / destinationWidth;
				const int sourcePixelY = sourceY + y * sourceHeight / destinationHeight;
				const int destinationPixelX = idestinationX + x;
				const int destinationPixelY = idestinationY + y;
				if (sourcePixelX < 0 || sourcePixelX >= source->width || sourcePixelY < 0 || sourcePixelY >= source->height || destinationPixelX < 0 || destinationPixelX >= destination->width || destinationPixelY < 0 || destinationPixelY >= destination->height)
					continue;
				std::memcpy(destination->pixels.data() + pixel_offset(*destination, destinationPixelX, destinationPixelY), source->pixels.data() + pixel_offset(*source, sourcePixelX, sourcePixelY), bytes_per_pixel);
			}
		}
		destination->ram_dirty = true;
	}

	/** Create a bitmap containing a copied rectangular region. */
	Bitmap *create_sub_bitmap(Bitmap *parent, int x, int y, int width, int height)
	{
		if (!parent || width <= 0 || height <= 0 || !ensure_ram_pixels(parent))
			return nullptr;
		Bitmap *bitmap = create_bitmap(width, height);
		if (!bitmap)
			return nullptr;
		blit(parent, bitmap, x, y, 0, 0, width, height);
		return bitmap;
	}

	/** Upload a bitmap's RAM pixels to its GPU texture. */
	bool upload_bitmap(Bitmap *bitmap)
	{
		if (is_screen(bitmap))
		{
			return false;
		}
		if (!ensure_gpu_texture(bitmap))
		{
			return false;
		}
		if (!bitmap->ram_dirty || bitmap->pixels.empty())
		{
			return true;
		}
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer || !renderer->upload_texture(bitmap->gpu_texture, bitmap->width, bitmap->height, bitmap->pixels.data()))
		{
			return false;
		}
		bitmap->ram_dirty = false;
		bitmap->gpu_dirty = false;
		return true;
	}

	/** Download a bitmap's GPU texture into RAM pixels. */
	bool download_bitmap(Bitmap *bitmap)
	{
		if (!is_valid(bitmap))
		{
			return false;
		}
		bitmap->pixels.resize(static_cast<std::size_t>(bitmap->width) * bitmap->height * bytes_per_pixel);
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer)
		{
			return false;
		}

		bool success = false;
		if (is_screen(bitmap))
		{
			success = renderer->download_render_target(0, bitmap->width, bitmap->height, bitmap->pixels.data());
		}
		else if (bitmap->fbo != 0)
		{
			success = renderer->download_render_target(bitmap->fbo, bitmap->width, bitmap->height, bitmap->pixels.data());
		}
		else if (bitmap->gpu_texture != 0)
		{
			success = renderer->download_texture(bitmap->gpu_texture, bitmap->width, bitmap->height, bitmap->pixels.data());
		}

		if (success)
		{
			bitmap->ram_dirty = false;
			bitmap->gpu_dirty = false;
		}
		return success;
	}

	/** Bind a bitmap's GPU texture to an active texture unit (0..N-1). Pass nullptr to unbind. */
	bool bind_texture(unsigned int unit, Bitmap *bitmap)
	{
		detail::Renderer *renderer = detail::active_renderer();
		if (!renderer)
		{
			return false;
		}
		if (!bitmap)
		{
			renderer->bind_texture_unit(unit, 0);
			return true;
		}
		if (!upload_bitmap(bitmap))
		{
			return false;
		}
		renderer->bind_texture_unit(unit, bitmap->gpu_texture);
		return true;
	}
	namespace
	{
		Bitmap *cursorBitmap = nullptr;
		int cursorOffsetX = 0;
		int cursorOffsetY = 0;
	} // namespace

	void set_mouse_bitmap(Bitmap *mouse_bitmap, int offset_x, int offset_y)
	{
		cursorBitmap = mouse_bitmap;
		cursorOffsetX = offset_x;
		cursorOffsetY = offset_y;
		detail::refresh_mouse_cursor();
	}

	namespace detail
	{
		Bitmap *mouse_bitmap()
		{
			return cursorBitmap;
		}

		void draw_mouse_cursor()
		{
			// only on the window, never into an offscreen target
			if (!cursorBitmap || cursorBitmap == screen || cursorBitmap->fbo != 0)
				return;
			draw_sprite(cursorBitmap, static_cast<float>(mouse_x() - cursorOffsetX), static_cast<float>(mouse_y() - cursorOffsetY));
		}
	} // namespace detail

	/** Draw a bitmap at a screen position. */
	void draw_sprite(Bitmap *bitmap, float x, float y)
	{
		if (!bitmap || is_drawing_target(bitmap) || screen_width() <= 0 || screen_height() <= 0)
		{
			return;
		}
		const bool flipVertical = bitmap->fbo != 0 && graphics_backend() == GraphicsBackend::opengl;
		draw_textured_quad(bitmap, 0, 0, bitmap->width, bitmap->height, x, y, -1, -1, false, flipVertical);
	}
	/** Draw a stretched sprite. */
	void draw_sprite_stretched(Bitmap *bitmap, float x, float y, int width, int height)
	{
		if (!bitmap || is_drawing_target(bitmap) || screen_width() <= 0 || screen_height() <= 0)
		{
			return;
		}
		draw_textured_quad(bitmap, 0, 0, bitmap->width, bitmap->height, x, y, width, height);
	}
	/** Draw a sprite rotated around its centre. */
	void draw_sprite_rotated(Bitmap *bitmap, float centerX, float centerY, float angleDegrees)
	{
		if (!bitmap || is_drawing_target(bitmap) || screen_width() <= 0 || screen_height() <= 0)
		{
			return;
		}
		draw_textured_quad_rotated(bitmap, centerX, centerY, angleDegrees);
	}

	/** Draw a stretched sprite rotated around its centre. */
	void draw_sprite_rotated_stretched(Bitmap *bitmap, float centerX, float centerY, float angleDegrees, int width, int height)
	{
		if (!bitmap || is_drawing_target(bitmap) || screen_width() <= 0 || screen_height() <= 0 || width <= 0 || height <= 0)
		{
			return;
		}
		draw_textured_quad_rotated(bitmap, centerX, centerY, angleDegrees, width, height);
	}
	/** Draw a bitmap region scaled to a destination size, rotated clockwise around a pivot offset (in destination pixels). */
	void draw_sprite_region_rotated(Bitmap *bitmap, int sourceX, int sourceY, int width, int height, float x, float y, float destinationWidth, float destinationHeight, float pivotX, float pivotY, float angleDegrees)
	{
		if (!bitmap || is_drawing_target(bitmap) || screen_width() <= 0 || screen_height() <= 0 || width <= 0 || height <= 0 || destinationWidth <= 0 || destinationHeight <= 0)
		{
			return;
		}
		draw_textured_region_rotated(bitmap, sourceX, sourceY, width, height, x, y, destinationWidth, destinationHeight, pivotX, pivotY, angleDegrees);
	}

	/** Draw a horizontally flipped bitmap. */
	void draw_sprite_h_flip(Bitmap *bitmap, float x, float y)
	{
		if (bitmap && !is_drawing_target(bitmap) && screen_width() > 0 && screen_height() > 0)
			draw_textured_quad(bitmap, 0, 0, bitmap->width, bitmap->height, x, y, -1, -1, true, false);
	}

	/** Draw a vertically flipped bitmap. */
	void draw_sprite_v_flip(Bitmap *bitmap, float x, float y)
	{
		if (bitmap && !is_drawing_target(bitmap) && screen_width() > 0 && screen_height() > 0)
			draw_textured_quad(bitmap, 0, 0, bitmap->width, bitmap->height, x, y, -1, -1, false, true);
	}

	namespace detail
	{

		/** Create the display-owned screen bitmap. */
		void initialise_screen(int width, int height)
		{
			destroy_screen();
			screen = new Bitmap;
			screen->width = width;
			screen->height = height;
			screen->kind = BitmapKind::Screen;
		}

		/** Resize the display-owned screen bitmap. */
		void resize_screen(int width, int height)
		{
			if (!screen)
			{
				initialise_screen(width, height);
				return;
			}
			screen->width = width;
			screen->height = height;
			screen->pixels.clear();
			screen->ram_dirty = false;
		}

		/** Destroy the display-owned screen bitmap. */
		void destroy_screen()
		{
			delete screen;
			screen = nullptr;
		}

	} // namespace detail

} // namespace sl
