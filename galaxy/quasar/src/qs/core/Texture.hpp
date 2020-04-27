///
/// Texture.hpp
/// quasar
///
/// Apache 2.0 LICENSE.
/// Refer to LICENSE.txt for more details.
///

#ifndef QUASAR_TEXTURE_HPP_
#define QUASAR_TEXTURE_HPP_

#include <string>

#include "qs/core/Colours.hpp"
#include "qs/utils/TextureFilters.hpp"

///
/// Core namespace.
///
namespace qs
{
	///
	/// Holds an OpenGL texture information and data.
	///
	class Texture
	{
	public:
		///
		/// \brief Default constructor.
		///
		/// Remember to call load()!
		///
		Texture() noexcept;

		///
		/// \brief Argument constructor.
		///
		/// Calls load().
		/// Can throw exceptions.
		///
		/// \param file File on disk to load from.
		///
		explicit Texture(const std::string& file);

		///
		/// \brief Argument constructor.
		///
		/// Calls load().
		/// Can throw exceptions.
		///
		/// \param mem Memory buffer to load from. Not freed, you must free after.
		/// \param size Size of the buffer.
		///
		explicit Texture(const unsigned char* mem, const unsigned int size);

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
		void load(const std::string& file);

		///
		/// \brief Loads texture from memory.
		///
		/// Can throw exceptions.
		///
		/// \param mem Memory buffer to load from. Not freed, you must free after.
		/// \param size Size of the buffer.
		///
		void load(const unsigned char* mem, const unsigned int size);

		///
		/// Loads texture from OpenGL generated id.
		///
		/// \param id ID to use from OpenGL.
		/// \param width Width of texture.
		/// \param height Height of texture.
		///
		void load(const unsigned int id, const int width, const int height) noexcept;

		///
		/// Saves texture to file on disk.
		///
		/// \param path Path (including filename) to save file to.
		///				Do not include extension. So i.e. "textures/wall" to save to wall.png.
		///
		void save(const std::string& path) noexcept;
		
		///
		/// \brief Bind as active OpenGL texture.
		///
		/// Binds to GL_TEXTURE2D
		///
		virtual void bind() noexcept;

		///
		/// \brief Unbinds texture from GL_TEXTURE2D context.
		///
		/// Note technically unbinds any texture since sets GL_TEXTURE2D to NULL.
		///
		virtual void unbind() noexcept;

		///
		/// Makes the texture repeat over its verticies.
		///
		void setRepeated() noexcept;

		///
		/// Mirrors the texture.
		///
		void setMirrored() noexcept;

		///
		/// \brief Clamps texture to edges.
		///
		/// Clamps the coordinates between 0 and 1.
		/// The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.
		///
		void clampToEdge() noexcept;

		///
		/// \brief Clamps to the border.
		///
		/// Coordinates outside the range are now given a user-specified border color.
		///
		/// \param border Colour of the border.
		///
		void clampToBorder(protostar::Colour& border) noexcept;

		///
		/// Set ansiotropic filtering level.
		///
		/// \param level 2, 4, 8, etc...
		///
		void setAnisotropy(const int level) noexcept;

		///
		/// Set filter when texture is downscaled in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void setMinifyFilter(const qs::TextureFilter& filter) noexcept;

		///
		/// Set filter when texture would be scaled up in OpenGL.
		///
		/// \param filter Enum filter to apply to texture.
		///
		void setMagnifyFilter(const qs::TextureFilter& filter) noexcept;

		///
		/// \brief Get texture width.
		///
		/// Is cached for performance.
		///
		/// \return Width as int. int over unsigned for compat with float.
		///
		const int getWidth() const noexcept;

		///
		/// \brief Get texture height.
		///
		/// Is cached for performance.
		///
		/// \return Height as int. int over unsigned for compat with float.
		///
		const int getHeight() const noexcept;

		///
		/// Gets internal OpenGL id.
		///
		/// \return Const unsigned int.
		///
		const unsigned int getGLTexture() const noexcept;

	protected:
		///
		/// Internal OpenGL ID of texture.
		///
		unsigned int m_id;

		///
		/// Cached texture width.
		///
		int m_width;

		///
		/// Cached texture height.
		///
		int m_height;
	};
}

#endif