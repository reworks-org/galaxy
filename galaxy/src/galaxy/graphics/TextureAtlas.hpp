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

#include <nlohmann/json_fwd.hpp>
#include <robin_hood.h>

#include "galaxy/algorithm/RectPack.hpp"
#include "galaxy/components/Transform.hpp"
#include "galaxy/graphics/RenderTexture.hpp"
#include "galaxy/meta/Memory.hpp"

namespace galaxy
{
	namespace graphics
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
				RenderTexture m_render_texture;
			};

			///
			/// Information about textures stored in atlas.
			///
			struct Info final
			{
				///
				/// Region of the texture in the atlas.
				///
				iRect m_region;

				///
				/// Index of which atlas this sprite belongs to.
				///
				int m_index;
			};

			///
			/// Constructor.
			///
			TextureAtlas();

			///
			/// File constructor.
			///
			/// \param files List of files to load.
			///
			TextureAtlas(std::span<std::string> files);

			///
			/// JSON constructor.
			///
			/// \param json JSON data to load from.
			///
			TextureAtlas(const nlohmann::json& json);

			///
			/// Move constructor.
			///
			TextureAtlas(TextureAtlas&&) noexcept;

			///
			/// Move assignment operator.
			///
			TextureAtlas& operator=(TextureAtlas&&) noexcept;

			///
			/// Destructor.
			///
			~TextureAtlas();

			///
			/// Add a single file.
			///
			/// \param file Texture file in the vfs to add to atlas.
			///
			void add(std::string_view file);

			///
			/// Add a group of files.
			///
			/// \param files List of files to load.
			///
			void add(std::span<std::string> files);

			///
			/// Add from a JSON definition.
			///
			/// \param json JSON data to load from.
			///
			void add_from_json(const nlohmann::json& json);

			///
			/// \brief Save all created atlas' to disk.
			///
			/// WARNING: This function will slow down the more atlas' there are to save.
			///
			void save();

			///
			/// Bind all atlas'.
			///
			void bind() noexcept;

			///
			/// Unbind all.
			///
			void unbind() noexcept;

			///
			/// Check if atlas contains a texture.
			///
			/// \param key Identifier of texture to look for.
			///
			/// \return True if found.
			///
			[[nodiscard]] bool contains(const std::string& key) noexcept;

			///
			/// Get data about texture in atlas.
			///
			/// \param key Identifier of texture to query.
			///
			/// \return TextureAtlas::Info, as an optional reference.
			///
			[[nodiscard]] meta::OptionalRef<Info> query(const std::string& key) noexcept;

		private:
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
			std::vector<Sheet> m_sheets;

			///
			/// Index'd list of textures on a sheet.
			///
			robin_hood::unordered_flat_map<std::string, Info> m_data;

			///
			/// Default vertex array to use when building an atlas.
			///
			VertexArray m_va;

			///
			/// Default transform to use when building an atlas.
			///
			components::Transform m_transform;
		};
	} // namespace graphics
} // namespace galaxy

#endif