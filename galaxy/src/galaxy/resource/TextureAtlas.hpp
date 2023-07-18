///
/// TextureAtlas.hpp
/// galaxy
///
/// Refer to LICENSE.txt for more details.
///

#ifndef GALAXY_GRAPHICS_TEXTUREATLAS_HPP_
#define GALAXY_GRAPHICS_TEXTUREATLAS_HPP_

#include <span>
#include <string_view>

#include <robin_hood.h>

#include "galaxy/algorithm/RectPack.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/graphics/TexelRegion.hpp"
#include "galaxy/graphics/VertexArray.hpp"

namespace galaxy
{
	namespace resource
	{
		///
		/// Parses raw texture files and stiches them into a large altas.
		///
		class TextureAtlas final
		{
		public:
			///
			/// A sheet of textures in the atlas.
			///
			struct Sheet final
			{
				///
				/// Rectangle packing algorithm structure.
				///
				algorithm::RectPack m_packer;

				///
				/// Texture to combine to.
				///
				graphics::RenderTexture m_render_texture;

				///
				/// Flag to determine if this sheet has been created.
				///
				bool m_sheet_created = false;
			};

			///
			/// Information about textures stored in atlas.
			///
			struct Info final
			{
				///
				/// Region of the texture in the atlas.
				///
				graphics::iRect m_region;

				///
				/// Texture region in OpenGL texels.
				///
				graphics::TexelRegion m_texel_region;

				///
				/// Index of which atlas this sprite belongs to.
				///
				int m_index = 0;

				///
				/// Texture handle this texture belongs to.
				///
				int m_handle = 0;

				///
				/// Texture sheet width.
				///
				int m_sheet_width = 0;

				///
				/// Texture sheet height.
				///
				int m_sheet_height = 0;
			};

			///
			/// Constructor.
			///
			TextureAtlas();

			///
			/// Destructor.
			///
			virtual ~TextureAtlas();

			///
			/// Add a single file.
			///
			/// \param file Texture file in the vfs to add to atlas.
			///
			void add_file(std::string_view file);

			///
			/// Loads textures from a folder.
			///
			/// \param folder Folder located in the VFS.
			///
			void add_folder(std::string_view folder);

			///
			/// \brief Reload textures from folder.
			///
			/// Does nothing if load hasn't been called.
			///
			void reload();

			///
			/// \brief Save all created atlas' to disk.
			///
			/// WARNING: This function will slow down the more atlas' there are to save.
			///
			void save();

			///
			/// Clear all data.
			///
			void clear();

			///
			/// Check if atlas contains a texture.
			///
			/// \param key Identifier of texture to look for.
			///
			/// \return True if found.
			///
			[[nodiscard]] bool contains(const std::string& key);

			///
			/// Get data about texture in atlas.
			///
			/// \param key Identifier of texture to query.
			///
			/// \return TextureAtlas::Info, as an optional reference.
			///
			[[nodiscard]] meta::optional_ref<Info> query(const std::string& key);

			///
			/// Takes in a x positon texture coord and maps it to a texel.
			///
			/// \tparam Type Type of the variable.
			///
			/// \param x Position of texture coord.
			/// \param width Texture width.
			///
			/// \return OpenGL shader compatible texel.
			///
			template<meta::is_arithmetic Type>
			[[nodiscard]] static float map_x_texel(const Type x, const Type width);

			///
			/// Takes in a y positon texture coord and maps it to a texel.
			///
			/// \tparam Type Type of the variable.
			///
			/// \param y Position of texture coord.
			/// \param height Texture height.
			///
			/// \return OpenGL shader compatible texel.
			///
			template<meta::is_arithmetic Type>
			[[nodiscard]] static float map_y_texel(const Type y, const Type height);

			///
			/// Get a list of keys in the cache.
			///
			/// \return A vector of strings.
			///
			[[nodiscard]] meta::vector<std::string> keys();

		private:
			///
			/// Move constructor.
			///
			TextureAtlas(TextureAtlas&&) = delete;

			///
			/// Move assignment operator.
			///
			TextureAtlas& operator=(TextureAtlas&&) = delete;

			///
			/// Copy constructor.
			///
			TextureAtlas(const TextureAtlas&) = delete;

			///
			/// Copy assignment operator.
			///
			TextureAtlas& operator=(const TextureAtlas&) = delete;

			///
			/// Initialize atlas.
			///
			void init();

		private:
			///
			/// Texture folder in vfs.
			///
			std::string m_folder;

			///
			/// Max number of active textures allowed.
			///
			int m_max_bindings;

			///
			/// Maxinum size of an atlas texture.
			///
			int m_size;

			///
			/// Texture atlas sheets.
			///
			meta::vector<Sheet> m_sheets;

			///
			/// Index'd list of textures on a sheet.
			///
			robin_hood::unordered_flat_map<std::string, Info> m_data;

			///
			/// Default vertex array to use when building an atlas.
			///
			graphics::VertexArray m_vao;

			///
			/// Default transform to use when building an atlas.
			///
			components::Transform m_transform;
		};

		template<meta::is_arithmetic Type>
		inline float TextureAtlas::map_x_texel(const Type x, const Type width)
		{
			if constexpr (std::is_floating_point<Type>::value)
			{
				return x / width;
			}
			else
			{
				return static_cast<float>(x) / static_cast<float>(width);
			}
		}

		template<meta::is_arithmetic Type>
		inline float TextureAtlas::map_y_texel(const Type y, const Type height)
		{
			if constexpr (std::is_floating_point<Type>::value)
			{
				return y / height;
			}
			else
			{
				return static_cast<float>(y) / static_cast<float>(height);
			}
		}
	} // namespace resource
} // namespace galaxy

#endif