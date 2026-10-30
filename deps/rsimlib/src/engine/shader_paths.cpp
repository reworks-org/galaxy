#include "shader_paths.h"

#include <SDL3/SDL.h>

#include <system_error>

namespace sl::detail
{
    namespace
    {
        std::vector<std::filesystem::path> build_data_dirs()
        {
            std::vector<std::filesystem::path> dirs;
            auto add = [&dirs](const std::filesystem::path &path)
            {
                if (!path.empty())
                    dirs.push_back(path.lexically_normal());
            };

            if (const char *env = SDL_getenv("SIMLIB_DATA_DIR"))
                add(env);

            if (const char *base = SDL_GetBasePath())
            {
                const std::filesystem::path exe_dir = base;
                add(exe_dir);
                add(exe_dir / "share" / "simlib");
                add(exe_dir / ".." / "share" / "simlib");
            }

#ifdef SIMLIB_INSTALL_DATA_DIR
            add(SIMLIB_INSTALL_DATA_DIR);
#endif
#ifdef SIMLIB_BUILD_SOURCE_DIR
            add(SIMLIB_BUILD_SOURCE_DIR);
#endif
#ifdef SIMLIB_BUILD_BINARY_DIR
            add(SIMLIB_BUILD_BINARY_DIR);
#endif
            return dirs;
        }
    }

    const std::vector<std::filesystem::path> &data_dirs()
    {
        static const std::vector<std::filesystem::path> dirs = build_data_dirs();
        return dirs;
    }

    std::filesystem::path find_shader_root(const std::filesystem::path &probe)
    {
        std::error_code ec;
        for (const auto &dir : data_dirs())
        {
            if (std::filesystem::exists(dir / "shaders" / probe, ec))
                return dir / "shaders";
        }
        const auto &dirs = data_dirs();
        return dirs.empty() ? std::filesystem::path("shaders") : dirs.front() / "shaders";
    }
} // namespace sl::detail
