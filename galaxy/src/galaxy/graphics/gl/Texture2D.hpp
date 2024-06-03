///
/// Texture2D.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_TEXTURE2D_HPP_
#define GALAXY_GRAPHICS_GL_TEXTURE2D_HPP_

#include "galaxy/graphics/gl/Texture.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// Standard opengl texture.
		///
		class Texture2D final : public Texture
		{
		  public:
			///
			/// Constructor.
			///
			Texture2D();

			///
			/// Move constructor.
			///
			Texture2D(Texture2D&&);

			///
			/// Move assignment operator.
			///
			Texture2D& operator=(Texture2D&&);

			///
			/// Destructor.
			///
			virtual ~Texture2D();

			///
			/// Load a texture from vfs.
			///
			/// \param file Texture in VFS to load.
			///
			/// \return True if texture loaded successfully.
			///
			[[nodiscard]] bool load(const std::string& file);

			///
			/// \brief Loads texture from memory.
			///
			/// Loads from standard 256 byte array.
			///
			/// \param buffer Memory buffer to load from.
			///
			/// \return True if texture loaded successfully.
			///
			[[nodiscard]] bool load_mem(std::span<std::uint8_t> buffer);

			///
			/// Saves texture to file on disk.
			///
			/// \param file Path and filename to save texture to. Does not need extension (it will be ignored).
			///
			void save(std::string_view file);

			///
			/// Deletes texture data and configuration in OpenGL and creates new texture handle.
			///
			void recreate();

			///
			/// Activate context.
			///
			void bind() override;

			///
			/// Deactivate context.
			///
			void unbind() override;
		};
	} // namespace graphics
} // namespace galaxy

#endif
