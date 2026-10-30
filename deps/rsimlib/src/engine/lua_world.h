#pragma once

#include <filesystem>
#include <functional>
#include <optional>
#include <string>

namespace sol
{
    class state;
}

namespace sl
{
    struct Bitmap;

    /** Kinds of retained world drawing a script can request. */
    enum class LuaWorldDraw
    {
        tilemap,
        fluid,
        voxel_view,
        terrain_view
    };

    /** Owns the tile maps, fluid grids, voxel worlds and terrains created by scripts, and the Lua tables that drive them. */
    class LuaWorld
    {
    public:
        using PathResolver = std::function<std::optional<std::filesystem::path>(const std::string &)>;
        /** Called when a script requests drawing; the canvas keeps one retained command per kind and id. */
        /** Finds a sprite or render target by script id; returns null when unknown. */
        using BitmapResolver = std::function<Bitmap *(const std::string &)>;
        using DrawRequest = std::function<void(LuaWorldDraw kind, const std::string &id)>;

        LuaWorld();
        ~LuaWorld();

        LuaWorld(const LuaWorld &) = delete;
        LuaWorld &operator=(const LuaWorld &) = delete;

        /** Register the tilemap, fluid, fluid3d, voxel and terrain tables. */
        void bind(sol::state &lua, PathResolver resolver, DrawRequest request, BitmapResolver bitmaps);
        /** Draw one retained request using the parameters most recently set by the script. */
        void draw(LuaWorldDraw kind, const std::string &id, Bitmap *target) const;
        /** Destroy every object created by scripts. */
        void clear();

    private:
        struct Implementation;
        Implementation *implementation_;
    };
}
