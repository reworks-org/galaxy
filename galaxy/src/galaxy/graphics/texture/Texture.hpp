///
/// Texture.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTURE_TEXTURE_HPP_
#define GALAXY_GRAPHICS_TEXTURE_TEXTURE_HPP_

#include <span>

#include "galaxy/graphics/texture/BaseTexture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Holds an OpenGL texture information and data.
		///
		class Texture : public BaseTexture
		{
		public:
			///
			/// \brief Default constructor.
			///
			/// Remember to call load()!
			///
			Texture() noexcept;

			///
			/// Virtual destructor.
			///
			virtual ~Texture() noexcept = default;

			///
			/// \brief Loads texture from file.
			///
			/// Can throw exceptions.
			///
			/// \param file File on disk to load from.
			///
			void load(std::string_view file);

			///
			/// \brief Loads texture from memory.
			///
			/// Can throw exceptions.
			///
			/// \param mem Memory buffer to load from. Not freed, you must free after.
			///
			void load_mem(std::span<unsigned char> buffer);

			///
			/// Loads texture from OpenGL generated id.
			///
			/// \param id ID to use from OpenGL.
			/// \param width Width of texture.
			/// \param height Height of texture.
			///
			void load(const unsigned int id, const int width, const int height) noexcept;

			///
			/// Load based off params from glTexImage2D.
			///
			/// \param level https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			/// \param internalformat https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			/// \param width https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			/// \param height https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			/// \param border https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			/// \param format https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			/// \param type https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			/// \param pixels https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml.
			///
			void load(int level, int internalformat, int width, int height, int border, unsigned int format, unsigned int type, const void* pixels) noexcept;

			///
			/// Activate texture context.
			///
			void bind() noexcept override;

			///
			/// Deactivate texture context.
			///
			void unbind() noexcept override;

		private:
			///
			/// Copy constructor.
			///
			Texture(const Texture&) = delete;

			///
			/// Copy assignment operator.
			///
			Texture& operator=(const Texture&) = delete;
		};
	} // namespace graphics
} // namespace galaxy

#endif