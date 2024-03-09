///
/// TextureArray.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_GL_TEXTUREARRAY_HPP_
#define GALAXY_GRAPHICS_GL_TEXTUREARRAY_HPP_

#include "galaxy/graphics/gl/Texture.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace graphics
	{
		///
		/// \brief OpenGL texture array.
		///
		/// Example shader:
		/// https://github.com/fendevel/Guide-to-Modern-OpenGL-Functions?tab=readme-ov-file#texture-atlases-vs-arrays
		///
		class TextureArray final : public Texture
		{
		  public:
			///
			/// Constructor.
			///
			TextureArray();

			///
			/// Move constructor.
			///
			TextureArray(TextureArray&&);

			///
			/// Move assignment operator.
			///
			TextureArray& operator=(TextureArray&&);

			///
			/// Destructor.
			///
			virtual ~TextureArray();

			///
			/// \brief Add a texture to the texture array.
			///
			/// All textures must be the same size.
			///
			/// \param file Texture to load in vfs.
			///
			/// \return True if texture array created successfully.
			///
			[[nodiscard]] bool add(const std::string& file);

			///
			/// Creates the array texture.
			///
			/// \param width Shared width of all textures.
			/// \param height Shared height of all textures.
			///
			void build(const int width, const int height);

			///
			/// Creates an array texture from a tileset.
			///
			/// \param file Texture in VFS to load.
			/// \param tile_size Size of the tile i.e. Width/Height.
			///
			/// \return True if texture loaded successfully.
			///
			[[nodiscard]] bool build_from_tileset(const std::string& file, const int tile_size);

			///
			/// Activate context.
			///
			void bind() override;

			///
			/// Deactivate context.
			///
			void unbind() override;

		  private:
			///
			/// Data from adding a file to the texture array.
			/// Not kept after building.
			///
			meta::vector<unsigned char*> m_data;
		};
	} // namespace graphics
} // namespace galaxy

#endif
