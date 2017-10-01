// This code is based off of https://github.com/seibelj/Allegro-Texture-Packer/tree/master/c
// That code was licensed as MIT. See tools/Allegro-Texture-Packer/LICENSE-allegrotexturepacker.txt in repo root for details.
// This is essentially a port to C++, with some additions to support string ids, physfs, and uses rengine3 coding style.
// Most of the logic belongs to the original author.

#ifndef RENGINE3_TEXTUREPACKER_HPP_
#define RENGINE3_TEXTUREPACKER_HPP_

#include <string>
#include <unordered_map>

#include <allegro5/bitmap.h>
#include <allegro5/config.h>

typedef int AL_PACKED_IMAGE_ID;

struct packed_image
{
	int x;
	int y;
	int w;
	int h;
	int file_idx;
};

struct packed_image_file
{
	char *file_name;
	ALLEGRO_BITMAP *bitmap;
};

namespace re
{
	class TexturePacker
	{
	public:
		///
		/// Initialize the texture packer.pack_file_name and all referenced
		/// packed image files must be in the root of pack_file_path
		///
		TexturePacker(const std::string& atlas);

		///
		/// Frees all memory used by the texture packer. Call this when you are finished
		///
		~TexturePacker();

		///
		/// Like al_draw_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_bitmap
		///
		void al_draw_packed_bitmap(const std::string& p_img_id_s, float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_bitmap
		///
		void al_draw_tinted_packed_bitmap(const std::string& p_img_id_s, ALLEGRO_COLOR tint,
			float dx, float dy, int flags);

		///
		/// Like al_draw_tinted_scaled_rotated_bitmap
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_draw_tinted_scaled_rotated_bitmap
		///
		void al_draw_tinted_scaled_rotated_packed_bitmap(const std::string& p_img_id_s,
			ALLEGRO_COLOR tint,
			float cx, float cy, float dx, float dy, float xscale, float yscale,
			float angle, int flags);

		///
		/// Calls al_create_sub_bitmap() properly and returns the bitmap of the
		/// packed image. You can then draw and manipulate the bitmap yourself.
		///
		/// Be aware that you need to manually destroy the created bitmap
		/// when you are finished with al_destroy_bitmap()
		///
		/// http://liballeg.org/a5docs/trunk/graphics.html#al_create_sub_bitmap
		///
		ALLEGRO_BITMAP *al_create_packed_bitmap(const std::string& p_img_id_s);

	private:
		ALLEGRO_CONFIG *pack_config;
		int total_packed_image_files;
		packed_image *packed_image_array;
		packed_image_file *packed_image_file_array;

		std::unordered_map<std::string, AL_PACKED_IMAGE_ID> m_strToID;
	};
}

#endif