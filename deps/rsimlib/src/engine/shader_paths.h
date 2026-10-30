#pragma once

#include <filesystem>
#include <vector>

namespace sl::detail
{
    /**
     * Directories that may contain a `shaders/` folder, in lookup order:
     * 1. `$SIMLIB_DATA_DIR`
     * 2. next to the executable (`<exe>`, `<exe>/share/simlib`, `<exe>/../share/simlib`)
     * 3. the install prefix baked in at configure time
     * 4. the source and binary trees this library was built from
     */
    const std::vector<std::filesystem::path> &data_dirs();

    /**
     * Return the first `<data dir>/shaders` that contains @p probe (for example "glsl/default_2d.vert").
     * Falls back to the first candidate when nothing matches so callers still produce a sensible error path.
     */
    std::filesystem::path find_shader_root(const std::filesystem::path &probe);
} // namespace sl::detail
