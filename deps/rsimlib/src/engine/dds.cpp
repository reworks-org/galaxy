#include "dds.h"
#include "error.h"
#include "renderer.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <new>
#include <vector>

namespace sl
{
    namespace
    {
        enum class DdsKind
        {
            bc1,
            bc2,
            bc3,
            bc7,
            rgba32
        };

        struct DdsInfo
        {
            int width = 0;
            int height = 0;
            int levels = 1;
            DdsKind kind = DdsKind::bc1;
            bool bgra = false;
            std::streamoff data_offset = 128;
        };

        std::uint32_t read_u32(const std::uint8_t *at)
        {
            std::uint32_t value;
            std::memcpy(&value, at, 4);
            return value;
        }

        bool parse_header(std::ifstream &in, DdsInfo &info, std::string &why)
        {
            std::uint8_t head[148] = {};
            in.read(reinterpret_cast<char *>(head), 128);
            if (in.gcount() != 128 || std::memcmp(head, "DDS ", 4) != 0)
            {
                why = "not a DDS file";
                return false;
            }
            info.height = static_cast<int>(read_u32(head + 12));
            info.width = static_cast<int>(read_u32(head + 16));
            const std::uint32_t flags = read_u32(head + 8);
            info.levels = (flags & 0x20000) ? std::max(1, static_cast<int>(read_u32(head + 28))) : 1;
            const std::uint32_t pixel_flags = read_u32(head + 80);
            char four[5] = {};
            std::memcpy(four, head + 84, 4);
            if (info.width <= 0 || info.height <= 0)
            {
                why = "invalid size";
                return false;
            }
            if (pixel_flags & 0x4)
            {
                if (!std::strcmp(four, "DXT1"))
                    info.kind = DdsKind::bc1;
                else if (!std::strcmp(four, "DXT3"))
                    info.kind = DdsKind::bc2;
                else if (!std::strcmp(four, "DXT5"))
                    info.kind = DdsKind::bc3;
                else if (!std::strcmp(four, "DX10"))
                {
                    in.read(reinterpret_cast<char *>(head + 128), 20);
                    if (in.gcount() != 20)
                    {
                        why = "truncated DX10 header";
                        return false;
                    }
                    info.data_offset = 148;
                    const std::uint32_t format = read_u32(head + 128);
                    if (format == 71 || format == 72)
                        info.kind = DdsKind::bc1;
                    else if (format == 74 || format == 75)
                        info.kind = DdsKind::bc2;
                    else if (format == 77 || format == 78)
                        info.kind = DdsKind::bc3;
                    else if (format == 98 || format == 99)
                        info.kind = DdsKind::bc7;
                    else
                    {
                        why = "unsupported DXGI format";
                        return false;
                    }
                }
                else
                {
                    why = "unsupported FourCC (need DXT1, DXT3, DXT5 or BC7)";
                    return false;
                }
            }
            else if (read_u32(head + 88) == 32)
            {
                info.kind = DdsKind::rgba32;
                info.bgra = read_u32(head + 92) == 0x00ff0000u;
            }
            else
            {
                why = "unsupported pixel format";
                return false;
            }
            return true;
        }

        detail::CompressedFormat to_compressed(DdsKind kind)
        {
            return kind == DdsKind::bc1 ? detail::CompressedFormat::bc1 : kind == DdsKind::bc3 ? detail::CompressedFormat::bc3 : detail::CompressedFormat::bc7;
        }

        void expand565(std::uint16_t v, std::uint8_t *out)
        {
            const int r = (v >> 11) & 31, g = (v >> 5) & 63, b = v & 31;
            out[0] = static_cast<std::uint8_t>((r << 3) | (r >> 2));
            out[1] = static_cast<std::uint8_t>((g << 2) | (g >> 4));
            out[2] = static_cast<std::uint8_t>((b << 3) | (b >> 2));
        }

        void decode_block(const std::uint8_t *blk, DdsKind kind, std::uint8_t out[16][4])
        {
            const bool bc3 = kind == DdsKind::bc3;
            const bool has_alpha_block = bc3 || kind == DdsKind::bc2;
            const std::uint8_t *colour = blk + (has_alpha_block ? 8 : 0);
            std::uint16_t c0, c1;
            std::memcpy(&c0, colour, 2);
            std::memcpy(&c1, colour + 2, 2);
            const bool four_colour = c0 > c1 || has_alpha_block;
            std::uint8_t pal[4][4];
            expand565(c0, pal[0]);
            expand565(c1, pal[1]);
            for (int k = 0; k < 3; ++k)
            {
                pal[2][k] = static_cast<std::uint8_t>(four_colour ? (2 * pal[0][k] + pal[1][k]) / 3 : (pal[0][k] + pal[1][k]) / 2);
                pal[3][k] = static_cast<std::uint8_t>(four_colour ? (pal[0][k] + 2 * pal[1][k]) / 3 : 0);
            }
            for (int k = 0; k < 4; ++k)
                pal[k][3] = (!has_alpha_block && !four_colour && k == 3) ? 0 : 255;
            std::uint32_t bits;
            std::memcpy(&bits, colour + 4, 4);
            std::uint8_t alpha[16];
            std::fill(alpha, alpha + 16, std::uint8_t{255});
            if (kind == DdsKind::bc2)
            {
                // Explicit alpha: 4 bits per texel, low nibble first.
                for (int i = 0; i < 16; ++i)
                {
                    const int nibble = (blk[i / 2] >> (4 * (i & 1))) & 15;
                    alpha[i] = static_cast<std::uint8_t>(nibble * 17);
                }
            }
            else if (bc3)
            {
                std::uint8_t a[8] = {blk[0], blk[1]};
                if (a[0] > a[1])
                    for (int k = 1; k <= 6; ++k)
                        a[k + 1] = static_cast<std::uint8_t>(((7 - k) * a[0] + k * a[1]) / 7);
                else
                {
                    for (int k = 1; k <= 4; ++k)
                        a[k + 1] = static_cast<std::uint8_t>(((5 - k) * a[0] + k * a[1]) / 5);
                    a[6] = 0;
                    a[7] = 255;
                }
                std::uint64_t alpha_bits = 0;
                for (int i = 0; i < 6; ++i)
                    alpha_bits |= static_cast<std::uint64_t>(blk[2 + i]) << (8 * i);
                for (int i = 0; i < 16; ++i)
                    alpha[i] = a[(alpha_bits >> (3 * i)) & 7];
            }
            for (int i = 0; i < 16; ++i)
            {
                const std::uint8_t *c = pal[(bits >> (2 * i)) & 3];
                out[i][0] = c[0];
                out[i][1] = c[1];
                out[i][2] = c[2];
                out[i][3] = alpha[i];
            }
        }

        Bitmap *decode_top_level(std::ifstream &in, const DdsInfo &info, std::string &why)
        {
            if (info.kind == DdsKind::bc7)
            {
                why = "BC7 needs a renderer with compressed texture support";
                return nullptr;
            }
            Bitmap *bitmap = create_bitmap(info.width, info.height);
            if (!bitmap)
            {
                why = "out of memory";
                return nullptr;
            }
            std::uint8_t *dst = bitmap->pixels.data();
            in.seekg(info.data_offset);
            if (info.kind == DdsKind::rgba32)
            {
                if (!in.read(reinterpret_cast<char *>(dst), static_cast<std::streamsize>(bitmap->pixels.size())))
                {
                    why = "truncated file";
                    destroy_bitmap(bitmap);
                    return nullptr;
                }
                if (info.bgra)
                    for (std::size_t i = 0; i < bitmap->pixels.size(); i += 4)
                        std::swap(dst[i], dst[i + 2]);
                return bitmap;
            }
            const int block_bytes = info.kind == DdsKind::bc1 ? 8 : 16;
            const int blocks_x = (info.width + 3) / 4, blocks_y = (info.height + 3) / 4;
            std::vector<std::uint8_t> row(static_cast<std::size_t>(blocks_x) * block_bytes);
            for (int by = 0; by < blocks_y; ++by)
            {
                if (!in.read(reinterpret_cast<char *>(row.data()), static_cast<std::streamsize>(row.size())))
                {
                    why = "truncated file";
                    destroy_bitmap(bitmap);
                    return nullptr;
                }
                for (int bx = 0; bx < blocks_x; ++bx)
                {
                    std::uint8_t texels[16][4];
                    decode_block(row.data() + static_cast<std::size_t>(bx) * block_bytes, info.kind, texels);
                    for (int i = 0; i < 16; ++i)
                    {
                        const int x = bx * 4 + i % 4, y = by * 4 + i / 4;
                        if (x < info.width && y < info.height)
                            std::memcpy(dst + (static_cast<std::size_t>(y) * info.width + x) * 4, texels[i], 4);
                    }
                }
            }
            return bitmap;
        }
    } // namespace

    Bitmap *load_dds(const std::string &path, const DdsOptions &options)
    {
        std::ifstream in(path, std::ios::binary);
        if (!in)
        {
            detail::set_error("Unable to open " + path);
            return nullptr;
        }
        DdsInfo info;
        std::string why;
        if (!parse_header(in, info, why))
        {
            detail::set_error("Unable to load " + path + ": " + why);
            return nullptr;
        }

        detail::Renderer *renderer = detail::active_renderer();
        const bool block_format = info.kind != DdsKind::rgba32 && info.kind != DdsKind::bc2;
        if (block_format && options.allow_compressed_upload && renderer &&
            renderer->supports_compressed_format(to_compressed(info.kind)))
        {
            const int limit = options.max_size > 0 ? options.max_size : renderer->max_texture_size();
            int skip = 0;
            while (skip < info.levels - 1 && std::max(info.width, info.height) >> skip > limit)
                ++skip;
            if (std::max(info.width, info.height) >> skip <= limit)
            {
                detail::CompressedTextureDesc desc;
                desc.width = std::max(info.width >> skip, 1);
                desc.height = std::max(info.height >> skip, 1);
                desc.format = to_compressed(info.kind);
                desc.mip_levels = info.levels - skip;
                desc.filter = options.smooth ? detail::TextureFilter::linear : detail::TextureFilter::nearest;
                desc.anisotropy = options.anisotropy;
                std::streamoff offset = info.data_offset;
                for (int level = 0; level < skip; ++level)
                    offset += static_cast<std::streamoff>(detail::compressed_level_byte_count(
                        desc.format, std::max(info.width >> level, 1), std::max(info.height >> level, 1)));
                const std::size_t bytes = detail::compressed_texture_byte_count(desc);
                std::vector<std::uint8_t> data;
                try
                {
                    data.resize(bytes);
                }
                catch (const std::exception &)
                {
                    detail::set_error("Unable to allocate DDS data for " + path);
                    return nullptr;
                }
                in.seekg(offset);
                if (!in.read(reinterpret_cast<char *>(data.data()), static_cast<std::streamsize>(bytes)))
                {
                    detail::set_error("Unable to load " + path + ": truncated file");
                    return nullptr;
                }
                std::uint32_t texture = 0;
                if (renderer->create_texture_compressed(desc, data.data(), texture))
                {
                    std::unique_ptr<Bitmap> bitmap(new (std::nothrow) Bitmap);
                    if (!bitmap)
                    {
                        renderer->destroy_texture(texture);
                        return nullptr;
                    }
                    bitmap->width = desc.width;
                    bitmap->height = desc.height;
                    bitmap->gpu_texture = texture;
                    bitmap->smooth = options.smooth;
                    bitmap->anisotropy = options.anisotropy;
                    bitmap->mipmaps = desc.mip_levels > 1;
                    bitmap->compressed = true;
                    return bitmap.release();
                }
            }
        }

        Bitmap *bitmap = decode_top_level(in, info, why);
        if (!bitmap)
        {
            detail::set_error("Unable to load " + path + ": " + why);
            return nullptr;
        }
        bitmap->smooth = options.smooth;
        bitmap->anisotropy = options.anisotropy;
        return bitmap;
    }

} // namespace sl
