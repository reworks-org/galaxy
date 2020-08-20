///
/// Texture.hpp
/// quasar
///
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTURE_HPP_
#define QUASAR_TEXTURE_HPP_

#include <span>
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
		///
		void load(std::span<unsigned char> buffer);

		///
		/// Loads texture from OpenGL generated id.
		///
		/// \param id ID to use from OpenGL.
		/// \param width Width of texture.
		/// \param height Height of texture.
		///
		void load(const pr::positive_integer auto id, const pr::positive_integer auto width, const pr::positive_integer auto height);

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
		void load(pr::positive_integer auto level, pr::positive_integer auto internalformat, pr::positive_integer auto width, pr::positive_integer auto height, pr::positive_integer auto border, pr::positive_integer auto format, pr::positive_integer auto type, const pr::valid_void_ptr auto pixels);

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