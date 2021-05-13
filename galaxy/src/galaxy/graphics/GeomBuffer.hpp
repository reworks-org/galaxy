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
		class GeomBuffer final
		{
		public:
			enum class AttachmentType : short
			{
				FLOAT,
				UNSIGNED_BYTE
			};

			GeomBuffer();
			~GeomBuffer();

			void init(const int width, const int height);
			void add_attachments(const unsigned int count, const AttachmentType type);
			void create();
			void resize(const int width, const int height);
			void destroy();

			void clear_framebuffer();
			void bind();
			void unbind();

			[[nodiscard]] const unsigned int get_fbo() const noexcept;
			[[nodiscard]] const std::vector<unsigned int>& get_objects() const noexcept;

		public:
			Shader m_shader;

		private:
			int m_max_attachments;

			int m_width;
			int m_height;

			unsigned int m_fbo;
			unsigned int m_rbo;

			std::vector<unsigned int> m_buffer_objects;
			std::vector<unsigned int> m_used_attachments;
		};
	} // namespace graphics
} // namespace galaxy

#endif