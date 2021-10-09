///
/// GeomBuffer.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GEOMBUFFER_HPP_
#define GALAXY_GRAPHICS_GEOMBUFFER_HPP_

#include "galaxy/graphics/Shader.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Geometry buffer for deferred rendering.
		///
		class GeomBuffer final
		{
		public:
			///
			/// Type of data held in attachment buffer.
			///
			enum class AttachmentType : short
			{
				FLOAT,
				UNSIGNED_BYTE
			};

			///
			/// Constructor.
			///
			GeomBuffer();

			///
			/// Destructor.
			///
			~GeomBuffer();

			///
			/// Initialize gbuffer.
			///
			/// \param width Usually screen width.
			/// \param height Usually screen height.
			///
			void init(const int width, const int height);

			///
			/// Add a texture attachment to the gbuffer output.
			///
			/// \param count Number of attachments to add.
			/// \param type Type the attachments should be.
			///
			void add_attachments(const unsigned int count, const AttachmentType type);

			///
			/// Construct gbuffer.
			///
			void create();

			///
			/// Resize gbuffer.
			///
			/// \param width Usually screen width.
			/// \param height Usually screen height.
			///
			void resize(const int width, const int height);

			///
			/// \brief Destroy gbuffer.
			///
			/// Frees resources.
			///
			void destroy();

			///
			/// Clear framebuffer contents.
			///
			void prepare() noexcept;

			///
			/// Bind gbuffer for rendering to.
			///
			void bind() noexcept;

			///
			/// Unbind gbuffer.
			///
			void unbind() noexcept;

			///
			/// Get OpenGL framebuffer object.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int get_fbo() const noexcept;

			///
			/// Get framebuffer depth texture.
			///
			/// \return Const unsigned int.
			///
			[[nodiscard]] const unsigned int get_depth_tex() const noexcept;

			///
			/// Get all attachments.
			///
			/// \return Reference to const vector of OpenGL texture ids.
			///
			[[nodiscard]] const std::vector<unsigned int>& get_attachments() const noexcept;

		public:
			///
			/// Shader to apply when drawing geometry.
			///
			Shader m_shader;

		private:
			///
			/// Maximum attachments supported.
			///
			int m_max_attachments;

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
			unsigned int m_depth_tex;

			///
			/// Array of texture attachemnts.
			///
			std::vector<unsigned int> m_attachments;

			///
			/// Used colour attachments.
			///
			std::vector<unsigned int> m_used_colour_attachments;
		};
	} // namespace graphics
} // namespace galaxy

#endif