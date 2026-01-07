///
/// Framebuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_FRAMEBUFFER_HPP_
#define GALAXY_GRAPHICS_GL_FRAMEBUFFER_HPP_

#include "galaxy/graphics/Colour.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// OpenGL framebuffer to render to.
		///
		class Framebuffer final
		{
		  public:
			///
			/// Constructor.
			///
			Framebuffer();

			///
			/// Move constructor.
			///
			Framebuffer(Framebuffer&&);

			///
			/// Move assignment operator.
			///
			Framebuffer& operator=(Framebuffer&&);

			///
			/// Destructor.
			///
			~Framebuffer();

			///
			/// Add a colour attachment to the framebuffer.
			///
			/// \param width Usually screen width or size of render texture.
			/// \param height Usually screen height or size of render texture.
			///
			void add_colour_attachment(const int width, const int height);

			///
			/// Adds a depth and stencil supported renderbuffer to the framebuffer.
			///
			void add_depth_stencil_renderbuffer();

			///
			/// Add an attachment to store an integer.
			///
			void add_storage_attachment();

			///
			/// Construct framebuffer from attachments and renderbuffers.
			///
			void create();

			///
			/// Read storage buffer.
			///
			/// \param x Horizontal pixel pos.
			/// \param y Vertical pixel pos.
			///
			/// \returns Integer read at pixel coords.
			///
			[[nodiscard]] int read_storagebuffer(const int x, const int y);

			///
			/// Clear storagebuffer.
			///
			/// \param val Value to clear buffer to.
			///
			void clear_storagebuffer(const int val);

			///
			/// Set clear colour.
			///
			/// \param col Colour to clear framebuffer to.
			///
			void set_clear_colour(const graphics::Colour& col);

			///
			/// Bind framebuffer for rendering to.
			///
			/// \param clear Should the framebuffer clear attachments.
			///
			void begin(const bool clear = true);

			///
			/// Unbind framebuffer.
			///
			void end();

			///
			/// Clear framebuffer attachments.
			///
			void clear();

			///
			/// Clear a specific attachment.
			///
			/// \param index Specific index of attachment to clear.
			///
			void clear(const unsigned int index);

			///
			/// Completely delete objects.
			///
			void reset();

			///
			/// Get width.
			///
			/// \return Integer.
			///
			[[nodiscard]] int width() const;

			///
			/// Get height.
			///
			/// \return Integer.
			///
			[[nodiscard]] int height() const;

			///
			/// Get colour attachment texture id.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int texture() const;

			///
			/// Get OpenGL framebuffer object.
			///
			/// \return Unsigned int.
			///
			[[nodiscard]] unsigned int id() const;

		  private:
			///
			/// Copy constructor.
			///
			Framebuffer(const Framebuffer&) = delete;

			///
			/// Copy assignment operator.
			///
			Framebuffer& operator=(const Framebuffer&) = delete;

		  private:
			///
			/// OpenGL handle.
			///
			unsigned int m_id;

			///
			/// Width cache.
			///
			int m_width;

			///
			/// Height cache.
			///
			int m_height;

			///
			/// Colour attachment id.
			///
			unsigned int m_colour;

			///
			/// Renderbuffer attachment.
			///
			unsigned int m_renderbuffer;

			///
			/// Storage attachment id.
			///
			unsigned int m_storageattach;

			///
			/// Clear colour.
			///
			std::array<float, 4> m_clear_colour;
		};
	} // namespace graphics
} // namespace galaxy

#endif
