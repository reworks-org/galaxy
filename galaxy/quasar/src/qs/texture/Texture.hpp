///
/// Texture.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTURE_HPP_
#define QUASAR_TEXTURE_HPP_

#include <string>

#include "qs/texture/BaseTexture.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Holds an OpenGL texture information and data.
	///
	class Texture : public qs::BaseTexture
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Remember to call load()!
		///
		Texture();

		///
		/// Virtual destructor.
		///
		virtual ~Texture() noexcept;

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
		/// \param size Size of the buffer.
		///
		void load(const pr::not_nullptr auto mem, const pr::positive_uint auto size);

		///
		/// Loads texture from OpenGL generated id.
		///
		/// \param id ID to use from OpenGL.
		/// \param width Width of texture.
		/// \param height Height of texture.
		///
		void load(const pr::positive_uint auto id, const pr::positive_int auto width, const pr::positive_int auto height);

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
		void load(pr::positive_int auto level, pr::positive_int auto internalformat, pr::positive_int auto width, pr::positive_int auto height, pr::positive_int auto border, pr::positive_uint auto format, pr::positive_uint auto type, const pr::not_nullptr_void auto pixels);

		///
		/// Activate texture context.
		///
		void bind() noexcept override;

		///
		/// Deactivate texture context.
		///
		void unbind() noexcept override;
	};
} // namespace qs

#endif