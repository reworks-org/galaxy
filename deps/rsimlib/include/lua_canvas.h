#pragma once

#include "draw.h"
#include "lua_runtime.h"

#include <string>

namespace sl
{
    /** Owns a Lua runtime and exposes bitmap drawing operations to scripts. */
    class LuaCanvas
    {
    public:
        /** Create an uninitialised canvas. */
        LuaCanvas();
        /** Release the runtime and canvas resources. */
        ~LuaCanvas();

        LuaCanvas(const LuaCanvas &) = delete;
        LuaCanvas &operator=(const LuaCanvas &) = delete;

        /** Create the Lua runtime and register the canvas API. */
        void initialise(LuaRuntime::OutputHandler output_handler = {});
        /** Return whether the runtime and canvas bindings are ready. */
        bool is_initialised() const;
        /** Execute a Lua source string and return its result. */
        LuaScriptResult run_text(const std::string &script);
        /** Load and execute a Lua script file. */
        LuaScriptResult run_file(const std::string &path);
        /** Dispatch a stable application-defined key name to Lua on_keypress(key), when present. */
        LuaScriptResult dispatch_keypress(const std::string &key);
        /** Access the Lua runtime owned by this canvas. */
        LuaRuntime &runtime();
        /** Set the base directory used to resolve script asset paths. */
        void set_asset_root(const std::string &path);
        /** Render the script's current canvas contents to a bitmap. */
        void render(Bitmap *target) const;
        /** Clear all pixels from the script canvas. */
        void clear();
        /** Reset the runtime and canvas to their initial state. */
        void reset();

    private:
        struct Implementation;
        Implementation *implementation_;
    };
}