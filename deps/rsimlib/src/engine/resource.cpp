/** @file
 * @brief Implements the read-only ZIP resource archive reader.
 */

#include "resource.h"

#include "error.h"

#include <zlib.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <limits>

namespace sl
{
    namespace
    {

        constexpr std::uint32_t localSignature = 0x04034b50;
        constexpr std::uint32_t centralSignature = 0x02014b50;
        constexpr std::uint32_t endSignature = 0x06054b50;
        constexpr std::uint16_t storedMethod = 0;
        constexpr std::uint16_t deflateMethod = 8;

        std::uint16_t read16(const std::vector<std::uint8_t> &bytes, std::size_t offset)
        {
            return static_cast<std::uint16_t>(bytes[offset] | (bytes[offset + 1] << 8));
        }

        std::uint32_t read32(const std::vector<std::uint8_t> &bytes, std::size_t offset)
        {
            return static_cast<std::uint32_t>(bytes[offset]) |
                   (static_cast<std::uint32_t>(bytes[offset + 1]) << 8) |
                   (static_cast<std::uint32_t>(bytes[offset + 2]) << 16) |
                   (static_cast<std::uint32_t>(bytes[offset + 3]) << 24);
        }

        bool range_valid(std::size_t offset, std::size_t size, std::size_t total)
        {
            return offset <= total && size <= total - offset;
        }

        std::vector<std::uint8_t> read_file(const std::string &path)
        {
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file)
                return {};
            const std::streamoff size = file.tellg();
            if (size < 0 || static_cast<std::uintmax_t>(size) > std::numeric_limits<std::size_t>::max())
                return {};
            std::vector<std::uint8_t> bytes(static_cast<std::size_t>(size));
            file.seekg(0);
            if (!bytes.empty() && !file.read(reinterpret_cast<char *>(bytes.data()), size))
                return {};
            return bytes;
        }

    } // namespace

    bool Archive::open(const std::string &path)
    {
        close();
        const auto bytes = read_file(path);
        if (bytes.size() < 22)
        {
            sl::detail::set_error("Invalid or empty ZIP archive");
            return false;
        }

        const std::size_t searchStart = bytes.size() > 22 + 65535 ? bytes.size() - (22 + 65535) : 0;
        std::size_t end = bytes.size() - 22;
        for (;;)
        {
            if (read32(bytes, end) == endSignature)
                break;
            if (end == searchStart)
            {
                sl::detail::set_error("ZIP end-of-directory record not found");
                return false;
            }
            --end;
        }

        const std::uint16_t entryCount = read16(bytes, end + 10);
        const std::uint32_t directorySize = read32(bytes, end + 12);
        const std::uint32_t directoryOffset = read32(bytes, end + 16);
        if (!range_valid(directoryOffset, directorySize, bytes.size()) || read16(bytes, end + 8) != entryCount)
        {
            sl::detail::set_error("Invalid ZIP central directory");
            return false;
        }

        std::size_t cursor = directoryOffset;
        for (std::uint16_t index = 0; index < entryCount; ++index)
        {
            if (!range_valid(cursor, 46, bytes.size()) || read32(bytes, cursor) != centralSignature)
            {
                close();
                sl::detail::set_error("Invalid ZIP central directory entry");
                return false;
            }
            const std::uint16_t nameSize = read16(bytes, cursor + 28);
            const std::uint16_t extraSize = read16(bytes, cursor + 30);
            const std::uint16_t commentSize = read16(bytes, cursor + 32);
            const std::size_t recordSize = 46ull + nameSize + extraSize + commentSize;
            if (!range_valid(cursor, recordSize, bytes.size()))
            {
                close();
                sl::detail::set_error("Truncated ZIP central directory entry");
                return false;
            }
            Entry entry;
            entry.name.assign(reinterpret_cast<const char *>(bytes.data() + cursor + 46), nameSize);
            entry.method = read16(bytes, cursor + 10);
            entry.compressedSize = read32(bytes, cursor + 20);
            entry.uncompressedSize = read32(bytes, cursor + 24);
            entry.localHeaderOffset = read32(bytes, cursor + 42);
            entries_.push_back(std::move(entry));
            cursor += recordSize;
        }
        path_ = path;
        return true;
    }

    void Archive::close()
    {
        path_.clear();
        entries_.clear();
    }

    bool Archive::contains(const std::string &name) const
    {
        return std::any_of(entries_.begin(), entries_.end(), [&name](const Entry &entry)
                           { return entry.name == name; });
    }

    std::vector<std::string> Archive::entries() const
    {
        std::vector<std::string> names;
        names.reserve(entries_.size());
        for (const Entry &entry : entries_)
            names.push_back(entry.name);
        return names;
    }

    std::vector<std::uint8_t> Archive::read(const std::string &name) const
    {
        const auto found = std::find_if(entries_.begin(), entries_.end(), [&name](const Entry &entry)
                                        { return entry.name == name; });
        if (found == entries_.end())
        {
            sl::detail::set_error("ZIP entry not found: " + name);
            return {};
        }
        const auto bytes = read_file(path_);
        const std::size_t local = found->localHeaderOffset;
        if (!range_valid(local, 30, bytes.size()) || read32(bytes, local) != localSignature)
        {
            sl::detail::set_error("Invalid ZIP local header");
            return {};
        }
        const std::uint16_t nameSize = read16(bytes, local + 26);
        const std::uint16_t extraSize = read16(bytes, local + 28);
        const std::size_t dataOffset = local + 30ull + nameSize + extraSize;
        if (!range_valid(dataOffset, found->compressedSize, bytes.size()))
        {
            sl::detail::set_error("Truncated ZIP entry");
            return {};
        }
        const auto *source = bytes.data() + dataOffset;
        if (found->method == storedMethod)
        {
            return {source, source + found->compressedSize};
        }
        if (found->method != deflateMethod)
        {
            sl::detail::set_error("Unsupported ZIP compression method");
            return {};
        }
        std::vector<std::uint8_t> result(found->uncompressedSize);
        z_stream stream{};
        stream.next_in = const_cast<Bytef *>(source);
        stream.avail_in = found->compressedSize;
        stream.next_out = result.data();
        stream.avail_out = found->uncompressedSize;
        if (inflateInit2(&stream, -MAX_WBITS) != Z_OK || inflate(&stream, Z_FINISH) != Z_STREAM_END || stream.total_out != found->uncompressedSize)
        {
            inflateEnd(&stream);
            sl::detail::set_error("Unable to decompress ZIP entry");
            return {};
        }
        inflateEnd(&stream);
        return result;
    }

} // namespace sl
