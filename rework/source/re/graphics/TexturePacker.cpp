// This file has been modified for use in REngine3.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <allegro5/bitmap_io.h>
#include <allegro5/bitmap_draw.h>

#include "re/services/VFS.hpp"
#include "re/utility/Utils.hpp"
#include "re/services/ServiceLocator.hpp"

#include "TexturePacker.hpp"

namespace re
{
	TexturePacker::TexturePacker(const std::string& atlas)
	{
		pack_config = al_load_config_file_f(Locator::get<VFS>()->open(atlas, "rw"));

		// Load all the packed image files into bitmaps
		const char *total_files_str = al_get_config_value(pack_config, "file_metadata", "total_files");
		total_packed_image_files = atoi(total_files_str);
		packed_image_file_array = (struct packed_image_file*)calloc(total_packed_image_files, sizeof(struct packed_image_file));

		const char *c_all_files_str = al_get_config_value(pack_config, "file_metadata", "files");
		char *all_files_str = _strdup(c_all_files_str);

		char *img_file_name;
		const char delim[2] = ",";
		img_file_name = strtok(all_files_str, delim);
		int i = 0;

		while (img_file_name != NULL) {
			packed_image_file_array[i].file_name = (char*)calloc(strlen(img_file_name), sizeof(char));
			strcpy(packed_image_file_array[i].file_name, img_file_name);
			packed_image_file_array[i].bitmap = al_load_bitmap_f(Locator::get<VFS>()->open(std::string(img_file_name), "rw"), NULL);

			i++;
			img_file_name = strtok(NULL, delim);
		}

		free(all_files_str);

		// Create an array large enough to hold information for every packed image
		const char *total_images_str = al_get_config_value(pack_config, "file_metadata", "total_images");
		int total_images = atoi(total_images_str);
		packed_image_array = (struct packed_image*)calloc(total_images, sizeof(struct packed_image));

		ALLEGRO_CONFIG_SECTION* sec;
		std::string str_firstsec = Utils::nullToEmpty(al_get_first_config_section(pack_config, &sec));

		if (str_firstsec != "")
		{
			const char *id_str = al_get_config_value(pack_config, str_firstsec.c_str(), "id");
			const char *x_str = al_get_config_value(pack_config, str_firstsec.c_str(), "x");
			const char *y_str = al_get_config_value(pack_config, str_firstsec.c_str(), "y");
			const char *w_str = al_get_config_value(pack_config, str_firstsec.c_str(), "w");
			const char *h_str = al_get_config_value(pack_config, str_firstsec.c_str(), "h");
			const char *file_str = al_get_config_value(pack_config, str_firstsec.c_str(), "file");

			int id = atoi(id_str);

			// Iterate through packed_image_file_array to find the file_idx
			int i;
			int found = 0;
			for (i = 0; i < total_packed_image_files; i++) {
				if (strcmp(packed_image_file_array[i].file_name, file_str) == 0) {
					found = 1;
					packed_image_array[id].file_idx = i;
					break;
				}
			}

			packed_image_array[id].x = atoi(x_str);
			packed_image_array[id].y = atoi(y_str);
			packed_image_array[id].w = atoi(w_str);
			packed_image_array[id].h = atoi(h_str);

			m_strToID.emplace(str_firstsec, (AL_PACKED_IMAGE_ID)id);
		}

		bool looping = true;
		std::string str_secondsec = "";
		do
		{
			str_secondsec = Utils::nullToEmpty(al_get_next_config_section(&sec));
			if (str_secondsec != "")
			{
				const char *id_str = al_get_config_value(pack_config, str_secondsec.c_str(), "id");
				const char *x_str = al_get_config_value(pack_config, str_secondsec.c_str(), "x");
				const char *y_str = al_get_config_value(pack_config, str_secondsec.c_str(), "y");
				const char *w_str = al_get_config_value(pack_config, str_secondsec.c_str(), "w");
				const char *h_str = al_get_config_value(pack_config, str_secondsec.c_str(), "h");
				const char *file_str = al_get_config_value(pack_config, str_secondsec.c_str(), "file");

				int id = atoi(id_str);

				// Iterate through packed_image_file_array to find the file_idx
				int i;
				int found = 0;
				for (i = 0; i < total_packed_image_files; i++) {
					if (strcmp(packed_image_file_array[i].file_name, file_str) == 0) {
						found = 1;
						packed_image_array[id].file_idx = i;
						break;
					}
				}

				packed_image_array[id].x = atoi(x_str);
				packed_image_array[id].y = atoi(y_str);
				packed_image_array[id].w = atoi(w_str);
				packed_image_array[id].h = atoi(h_str);

				m_strToID.emplace(str_secondsec, (AL_PACKED_IMAGE_ID)id);
			}
			else
			{
				looping = false;
			}
		} 
		while (looping);
	}

	TexturePacker::~TexturePacker()
	{
		free(packed_image_array);

		int i;
		for (i = 0; i < total_packed_image_files; i++) {
			free(packed_image_file_array[i].file_name);
			al_destroy_bitmap(packed_image_file_array[i].bitmap);
		}
		free(packed_image_file_array);

		al_destroy_config(pack_config);
	}

	void TexturePacker::al_draw_packed_bitmap(const std::string& p_img_id_s, float dx, float dy, int flags)
	{
		int p_img_int = m_strToID[p_img_id_s];

		al_draw_bitmap_region(
			packed_image_file_array[packed_image_array[p_img_int].file_idx].bitmap,
			packed_image_array[p_img_int].x,
			packed_image_array[p_img_int].y,
			packed_image_array[p_img_int].w,
			packed_image_array[p_img_int].h,
			dx, dy, flags
		);
	}

	void TexturePacker::al_draw_tinted_packed_bitmap(const std::string& p_img_id_s, ALLEGRO_COLOR tint,
		float dx, float dy, int flags) 
	{
		int p_img_int = m_strToID[p_img_id_s];

		al_draw_tinted_bitmap_region(
			packed_image_file_array[packed_image_array[p_img_int].file_idx].bitmap,
			tint,
			packed_image_array[p_img_int].x,
			packed_image_array[p_img_int].y,
			packed_image_array[p_img_int].w,
			packed_image_array[p_img_int].h,
			dx, dy, flags
		);
	}

	void TexturePacker::al_draw_tinted_scaled_rotated_packed_bitmap(const std::string& p_img_id_s,
		ALLEGRO_COLOR tint,
		float cx, float cy, float dx, float dy, float xscale, float yscale,
		float angle, int flags) 
	{
		int p_img_int = m_strToID[p_img_id_s];

		al_draw_tinted_scaled_rotated_bitmap_region(
			packed_image_file_array[packed_image_array[p_img_int].file_idx].bitmap,
			packed_image_array[p_img_int].x,
			packed_image_array[p_img_int].y,
			packed_image_array[p_img_int].w,
			packed_image_array[p_img_int].h,
			tint,
			cx, cy, dx, dy, xscale, yscale,
			angle, flags
		);
	}

	ALLEGRO_BITMAP* TexturePacker::al_create_packed_bitmap(const std::string& p_img_id_s)
	{
		int p_img_int = m_strToID[p_img_id_s];

		return al_create_sub_bitmap(
			packed_image_file_array[packed_image_array[p_img_int].file_idx].bitmap,
			packed_image_array[p_img_int].x,
			packed_image_array[p_img_int].y,
			packed_image_array[p_img_int].w,
			packed_image_array[p_img_int].h
		);
	}
}