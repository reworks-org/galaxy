#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sl
{

    /** A read-only ZIP resource archive. */
    class Archive
    {
    public:
        Archive() = default;
        ~Archive() = default;

        Archive(const Archive &) = delete;
        Archive &operator=(const Archive &) = delete;
        /** Move the open archive state from another archive. */
        Archive(Archive &&) noexcept = default;
        /** Close this archive and take ownership of another archive's state. */
        Archive &operator=(Archive &&) noexcept = default;

        /** Open a ZIP archive from disk. */
        bool open(const std::string &path);
        /** Close the archive and release its directory and file data. */
        void close();
        /** Return whether an entry exists in the archive. */
        bool contains(const std::string &name) const;
        /** Return all file and directory entry names in archive order. */
        std::vector<std::string> entries() const;
        /** Read an entry into memory, returning an empty vector on failure. */
        std::vector<std::uint8_t> read(const std::string &name) const;

    private:
        struct Entry
        {
            std::string name;
            std::uint16_t method = 0;
            std::uint32_t compressedSize = 0;
            std::uint32_t uncompressedSize = 0;
            std::uint32_t localHeaderOffset = 0;
        };

        std::string path_;
        std::vector<Entry> entries_;
    };

} // namespace sl
