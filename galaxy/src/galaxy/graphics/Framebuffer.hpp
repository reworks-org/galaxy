///
/// Framebuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_FRAMEBUFFER_HPP_
#define GALAXY_GRAPHICS_FRAMEBUFFER_HPP_

#include <vector>

namespace galaxy
{
	namespace graphics
	{
		class Framebuffer final
		{
		public:
			///
			/// \brief Constructor.
			///
			/// You MUST call init() if you use this constructor.
			///
			Framebuffer() noexcept;

			///
			/// \brief Argument constructor.
			///
			/// You do not need to call init() if you use this constructor.
			///
			/// \param width Usually screen width.
			/// \param height Usually screen height.
			///
			Framebuffer(const int width, const int height) noexcept;

			///
			/// Move constructor.
			///
			Framebuffer(Framebuffer&&) noexcept;

			///
			/// Move assignment operator.
			///
			Framebuffer& operator=(Framebuffer&&) noexcept;

			///
			/// Destructor.
			///
			~Framebuffer();

			///
			/// \brief Init framebuffer data.
			///
			/// Only call if you used the default constructor.
			///
			/// \param width Usually screen width.
			/// \param height Usually screen height.
			///
			void init(const int width, const int height) noexcept;

			///
			/// Add a colour attachment to the framebuffer.
			///
			/// \param format Texture format. Usually GL_RGBA8.
			/// \param high_precision Is this storing positions or pixels. If just using plain textures, set at false. True is for geometry buffers.
			///
			void add_colour_attachment(const int format, const bool high_precision);

			///
			/// Add a colour renderbuffer to the framebuffer.
			///
			/// \param format Texture format. Usually GL_RGBA8.
			///
			void add_colour_renderbuffer(const int format);

			///
			/// Add a depth attachment to the framebuffer.
			///
			void add_depth_attachment();

			///
			/// Add a depth renderbuffer to the framebuffer.
			///
			void add_depth_renderbuffer();

			///
			/// Construct framebuffer.
			///
			void create();

			///
			/// Resize framebuffer.
			///
			/// \param width Usually screen width.
			/// \param height Usually screen height.
			///
			void resize(const int width, const int height);

			///
			/// Bind framebuffer for rendering to.
			///
			void bind() noexcept;

			///
			/// Unbind framebuffer.
			///
			void unbind() noexcept;

			///
			/// Get width.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_width() const noexcept;

			///
			/// Get height.
			///
			/// \return Const int.
			///
			[[nodiscard]] const int get_height() const noexcept;

			///
			/// \brief Get depth attachment.
			///
			/// Can only have depth attachment OR renderbuffer, not both.
			///
			/// \return Const unsigned int. 0 if invalid.
			///
			[[nodiscard]] const unsigned int get_depth_attachment() const noexcept;

			///
			/// \brief Get depth renderbuffer.
			///
			/// Can only have depth renderbuffer OR attachment, not both.
			///
			/// \return Const unsigned int. 0 if invalid.
			///
			[[nodiscard]] const unsigned int get_depth_renderbuffer() const noexcept;

			///
			/// Get all attachments.
			///
			/// \return Reference to const vector of OpenGL texture ids.
			///
			[[nodiscard]] const std::vector<unsigned int>& get_attachments() const noexcept;

			///
			/// Get all renderbuffer attachments.
			///
			/// \return Reference to const vector of OpenGL renderbuffer ids.
			///
			[[nodiscard]] const std::vector<unsigned int>& get_renderbuffers() const noexcept;

			///
			/// Get OpenGL framebuffer object.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int get_fbo() const noexcept;

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
			/// Width cache.
			///
			int m_width;

			///
			/// Height cache.
			///
			int m_height;

			///
			/// OpenGL framebuffer id.
			///
			unsigned int m_fbo;

			///
			/// OpenGL depth texture.
			///
			unsigned int m_depth_attachment;

			///
			/// OpenGL depth renderbuffer.
			///
			unsigned int m_depth_renderbuffer;

			///
			/// Maximum attachments supported.
			///
			unsigned int m_max_attachments;

			///
			/// Array of colour attachemnts.
			///
			std::vector<unsigned int> m_attachments;

			///
			/// Array of colour renderbuffers.
			///
			std::vector<unsigned int> m_renderbuffers;

			///
			/// Used colour attachments / renderbuffers.
			///
			std::vector<unsigned int> m_used_attachments;
		};
	} // namespace graphics
} // namespace galaxy

#endif