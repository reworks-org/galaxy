#include "shader_paths.h"

const char* blurSource =
#include "../../shaders/glsl/blur.frag"
;

const char* bright_passSource =
#include "../../shaders/glsl/bright_pass.frag"
;

const char* chromatic_aberrationSource =
#include "../../shaders/glsl/chromatic_aberration.frag"
;

const char* colour_adjustSource =
#include "../../shaders/glsl/colour_adjust.frag"
;

const char* compositeSource =
#include "../../shaders/glsl/composite.frag"
;

const char* crtSource =
#include "../../shaders/glsl/crt.frag"
;

const char* default_2d_fragSource =
#include "../../shaders/glsl/default_2d.frag"
;

const char* default_2d_vertSource =
#include "../../shaders/glsl/default_2d.vert"
;

const char* ditherSource =
#include "../../shaders/glsl/dither.frag"
;

const char* ex3d_fragSource =
#include "../../shaders/glsl/ex3d.frag"
;

const char* ex3d_vertSource =
#include "../../shaders/glsl/ex3d.vert"
;

const char* film_grainSource =
#include "../../shaders/glsl/film_grain.frag"
;

const char* fullscreenSource =
#include "../../shaders/glsl/fullscreen.vert"
;

const char* heat_hazeSource =
#include "../../shaders/glsl/heat_haze.frag"
;

const char* lightingSource =
#include "../../shaders/glsl/lighting.frag"
;

const char* light_cullSource =
#include "../../shaders/glsl/light_cull.comp"
;

const char* mesh3d_fragSource =
#include "../../shaders/glsl/mesh3d.frag"
;

const char* mesh3d_vertSource =
#include "../../shaders/glsl/mesh3d.vert"
;

const char* mesh3d_instancedSource =
#include "../../shaders/glsl/mesh3d_instanced.vert"
;

const char* pixelateSource =
#include "../../shaders/glsl/pixelate.frag"
;

const char* radial_blurSource =
#include "../../shaders/glsl/radial_blur.frag"
;

const char* shockwaveSource =
#include "../../shaders/glsl/shockwave.frag"
;

const char* sky_fragSource =
#include "../../shaders/glsl/sky.frag"
;

const char* sky_vertSource =
#include "../../shaders/glsl/sky.vert"
;

const char* terrain_fragSource =
#include "../../shaders/glsl/terrain.frag"
;

const char* terrain_vertSource =
#include "../../shaders/glsl/terrain.vert"
;

const char* tintSource =
#include "../../shaders/glsl/tint.frag"
;

const char* vignetteSource =
#include "../../shaders/glsl/vignette.frag"
;


// 2. Getter Function
std::string get_shader(const char* name)
{
    const auto as_str = std::string(name);

    if (as_str == "blur.frag")
    {
        return std::string(blurSource);
    }
    if (as_str == "bright_pass.frag")
    {
        return std::string(bright_passSource);
    }
    if (as_str == "chromatic_aberration.frag")
    {
        return std::string(chromatic_aberrationSource);
    }
    if (as_str == "colour_adjust.frag")
    {
        return std::string(colour_adjustSource);
    }
    if (as_str == "composite.frag")
    {
        return std::string(compositeSource);
    }
    if (as_str == "crt.frag")
    {
        return std::string(crtSource);
    }
    if (as_str == "default_2d.frag")
    {
        return std::string(default_2d_fragSource);
    }
    if (as_str == "default_2d.vert")
    {
        return std::string(default_2d_vertSource);
    }
    if (as_str == "dither.frag")
    {
        return std::string(ditherSource);
    }
    if (as_str == "ex3d.frag")
    {
        return std::string(ex3d_fragSource);
    }
    if (as_str == "ex3d.vert")
    {
        return std::string(ex3d_vertSource);
    }
    if (as_str == "film_grain.frag")
    {
        return std::string(film_grainSource);
    }
    if (as_str == "fullscreen.vert")
    {
        return std::string(fullscreenSource);
    }
    if (as_str == "heat_haze.frag")
    {
        return std::string(heat_hazeSource);
    }
    if (as_str == "lighting.frag")
    {
        return std::string(lightingSource);
    }
    if (as_str == "light_cull.comp")
    {
        return std::string(light_cullSource);
    }
    if (as_str == "mesh3d.frag")
    {
        return std::string(mesh3d_fragSource);
    }
    if (as_str == "mesh3d.vert")
    {
        return std::string(mesh3d_vertSource);
    }
    if (as_str == "mesh3d_instanced.vert")
    {
        return std::string(mesh3d_instancedSource);
    }
    if (as_str == "pixelate.frag")
    {
        return std::string(pixelateSource);
    }
    if (as_str == "radial_blur.frag")
    {
        return std::string(radial_blurSource);
    }
    if (as_str == "shockwave.frag")
    {
        return std::string(shockwaveSource);
    }
    if (as_str == "sky.frag")
    {
        return std::string(sky_fragSource);
    }
    if (as_str == "sky.vert")
    {
        return std::string(sky_vertSource);
    }
    if (as_str == "terrain.frag")
    {
        return std::string(terrain_fragSource);
    }
    if (as_str == "terrain.vert")
    {
        return std::string(terrain_vertSource);
    }
    if (as_str == "tint.frag")
    {
        return std::string(tintSource);
    }
    if (as_str == "vignette.frag")
    {
        return std::string(vignetteSource);
    }

    return ""; // Fallback to prevent undefined behavior if a shader isn't found
}
