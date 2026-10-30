#pragma once

#include "draw.h"

#include <string>

namespace sl
{

    /** Options for load_dds(). */
    struct DdsOptions
    {
        /** Largest width or height to accept; 0 uses the renderer's limit. */
        int max_size = 0;
        /** Bilinear (and trilinear when the file has mip levels) sampling instead of nearest. */
        bool smooth = true;
        /** Anisotropic filtering for the GPU texture; 1.0 disables it. */
        float anisotropy = 1.0f;
        /** Allow block-compressed data to be uploaded as-is. Set false to always decode to RGBA on the CPU. */
        bool allow_compressed_upload = true;
    };

    /**
     * Load a .dds file (BC1/DXT1, BC3/DXT5 or BC7 via a DX10 header, or uncompressed 32-bit RGBA/BGRA; BC2/DXT3 is decoded to RGBA on the CPU).
     *
     * Block-compressed files are uploaded straight to the GPU with every mip level they contain, which keeps video
     * and system memory at a fraction of RGBA8 (BC1 is 1/8). If the file is wider than the size limit its largest
     * mip levels are skipped. The result has Bitmap::compressed set and no CPU pixels, so it can be drawn and
     * bound as a texture but not read or edited.
     *
     * Where the renderer cannot take the format, or the file has no smaller mips to fit within the limit, BC1/BC3
     * and uncompressed files are decoded (top level only) into an ordinary RGBA bitmap that uploads like load_bitmap().
     * Returns nullptr on failure; see last_error().
     */
    Bitmap *load_dds(const std::string &path, const DdsOptions &options = {});

} // namespace sl
