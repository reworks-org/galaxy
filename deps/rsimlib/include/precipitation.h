#pragma once

/** @file
 * @brief Rain, hail and snow for 2D scenes and 3D surfaces, with presets, smooth weather changes, splashes,
 * snow accumulation, a lens/haze overlay and looping weather sound.
 */

#include "atmosphere.h"
#include "audio.h"
#include "draw.h"
#include "scene3d.h"
#include "surface3d.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <functional>
#include <random>
#include <vector>

namespace sl
{

    /** The kind of precipitation. */
    enum class PrecipitationType
    {
        none,
        rain,
        hail,
        snow
    };

    /** Named strengths for precipitation_preset(). */
    enum class PrecipitationLevel
    {
        light,
        moderate,
        heavy,
        storm
    };

    /** Settings shared by Precipitation2D and Precipitation3D. */
    struct PrecipitationConfig
    {
        PrecipitationType type = PrecipitationType::rain;
        /** 0 (nothing) to 1 (the heaviest fall); scales how many particles are alive. */
        float intensity = 0.5f;
        /** Wind: pixels per second (x, y) in 2D, metres per second along world x and z in 3D. */
        glm::vec2 wind{0.0f, 0.0f};
        /** Fall speed in pixels or metres per second; 0 uses the type's default. */
        float fall_speed = 0.0f;
        /** Multiplies particle size. */
        float size = 1.0f;
        /** Particle colour; an alpha of 0 uses the type's default colour. */
        Colour colour{0, 0, 0, 0};
        /** Spawn rain ripples and hail bounces where particles land. */
        bool splashes = true;
    };

    /** The intensity (0-1) that a PrecipitationLevel stands for. */
    float precipitation_level_intensity(PrecipitationLevel level);

    /**
     * A ready-made configuration: intensity from the level, wind that stiffens with it (storms are gusty and
     * slanted), larger flakes in light snow and so on. Wind is in 2D units (pixels/s); scale it for 3D (see
     * Precipitation3D::wind_from_clouds()).
     */
    PrecipitationConfig precipitation_preset(PrecipitationType type, PrecipitationLevel level);

    /**
     * Moves smoothly between weather settings. Changing the type fades the old precipitation out, then the new
     * one in; changing only intensity, wind, size or colour blends. Both Precipitation2D and Precipitation3D own
     * one as `weather`.
     */
    class PrecipitationController
    {
    public:
        /** Jump to `config` immediately. */
        void set_immediately(const PrecipitationConfig &config);
        /** Head for `target`; each fade takes `seconds` (0 snaps). */
        void transition_to(const PrecipitationConfig &target, float seconds = 3.0f);
        /** Advance the blend. */
        void update(float dt_seconds);

        /** What should be falling right now. The type switches only once the old fall has faded out. */
        const PrecipitationConfig &current() const { return current_; }
        const PrecipitationConfig &target() const { return target_; }
        /** True while the current settings differ from the target. */
        bool transitioning() const;

    private:
        PrecipitationConfig current_{PrecipitationType::none, 0.0f};
        PrecipitationConfig target_{PrecipitationType::none, 0.0f};
        float seconds_ = 0.0f;
    };

    /** Default particle colour for a type, used when PrecipitationConfig::colour has alpha 0. */
    Colour precipitation_default_colour(PrecipitationType type);

    /**
     * Rain, hail or snow falling over a rectangle of the screen. Particles live in screen space with a random depth
     * (far ones are small, slow and dim), so a scrolling game stays convincing by calling scroll() with the camera's
     * movement. Rain leaves ripples, hail bounces, and snow can pile up along the ground.
     */
    class Precipitation2D
    {
    public:
        explicit Precipitation2D(int width = 0, int height = 0);

        /** The weather; call weather.transition_to() or weather.set_immediately(). */
        PrecipitationController weather;

        /** Region to fill, in screen pixels. Defaults to the whole screen. */
        void set_area(float x, float y, float width, float height);
        /** Ground as a constant line; particles land on it. Defaults to the bottom of the area. */
        void set_ground(float y);
        /** Ground as a function of x (platforms, hills). Particles land where it returns a y at or above them. */
        void set_ground(std::function<float(float)> ground_y);
        /** Particles at 1.0 intensity (before scaling by the area; default 1500). */
        void set_max_particles(int count);
        /** Heap snow along the ground; the deepest pile in pixels. 0 turns accumulation off. */
        void set_max_snow_depth(float pixels);

        /** Shift the particles as the camera moves, so the fall stays fixed in the world; distant ones move less. */
        void scroll(float dx, float dy);
        /** Advance the simulation. */
        void update(float dt_seconds);
        /** Draw onto the screen. */
        void draw();
        /** Draw only the piled snow (call before drawing sprites that should stand in front of it, if wanted). */
        void draw_snow_cover();

        /** Remove every particle, splash and snow pile. */
        void clear();
        int particle_count() const { return static_cast<int>(drops_.size()); }
        int splash_count() const { return static_cast<int>(splashes_.size()); }
        /** Snow depth in pixels at world x (0 when accumulation is off or there is none). */
        float snow_depth_at(float x) const;

    private:
        struct Drop
        {
            float x, y, vx, vy, depth, phase, size;
            int bounces;
            float rest;
        };
        struct Splash
        {
            float x, y, age, life;
            bool hail;
        };

        void spawn(Drop &drop, bool anywhere);
        float ground_at(float x) const;
        float random(float low, float high);
        void add_snow(float x, float amount);

        float area_x_ = 0, area_y_ = 0, area_w_ = 0, area_h_ = 0;
        bool area_set_ = false;
        float ground_y_ = 0;
        bool ground_set_ = false;
        std::function<float(float)> ground_fn_;
        int max_particles_ = 1500;
        float max_snow_depth_ = 22.0f;
        float time_ = 0.0f;
        PrecipitationType active_type_ = PrecipitationType::none;
        std::vector<Drop> drops_;
        std::vector<Splash> splashes_;
        std::vector<float> snow_;
        std::mt19937 rng_{12345u};
    };

    /**
     * Rain, hail or snow in a 3D scene. A box of particles follows the camera, so a small number covers the whole
     * view. Particles are drawn as soft streaks and flakes through draw_mesh_instanced(), so they respect depth, are
     * hidden by terrain and buildings, and take on the environment's fog. Works on every renderer that supports
     * instancing. Rain makes ripples where it lands, hail bounces, and snow adds to snow_cover().
     */
    class Precipitation3D
    {
    public:
        Precipitation3D() = default;
        /** Frees GPU resources; safe to run after sl::shutdown() (nothing is touched then). */
        ~Precipitation3D() { release(); }
        Precipitation3D(const Precipitation3D &) = delete;
        Precipitation3D &operator=(const Precipitation3D &) = delete;

        /** The weather; call weather.transition_to() or weather.set_immediately(). */
        PrecipitationController weather;

        /** Size in metres (x, y, z) of the box of particles around the camera. Default 36 x 24 x 36. */
        void set_volume(const glm::vec3 &size);
        /** Particles at 1.0 intensity (default 5000, the capacity of the instance buffer). */
        void set_max_particles(int count);
        /** Height of the ground at (x, z) in metres, for landings. Defaults to y = 0. */
        void set_ground(std::function<float(float, float)> height);
        /** Snow cover gained per second at full intensity (default 0.01) and lost per second when it is not snowing. */
        void set_accumulation(float gain_per_second, float melt_per_second);

        /**
         * Wind that matches a cloud layer, so rain slants the way the clouds drift. `scale` converts the layer's
         * wind (m/s) to ground-level wind; precipitation seldom keeps the full cloud speed.
         */
        static glm::vec2 wind_from_clouds(const CloudLayer &clouds, float scale = 0.5f);

        /** Advance the simulation around `camera`. */
        void update(float dt_seconds, const Camera &camera);
        /**
         * Draw into the active 3D surface; call between begin_surface_3d() and end_surface_3d() after the scene so
         * depth tests hide particles behind geometry. Returns false if the backend cannot instance meshes.
         */
        bool draw(const Camera &camera);
        /** Free GPU resources now. Optional: the destructor does this, and it is a no-op once sl::shutdown() has run. */
        void release();

        /** How much snow has settled, 0-1. Use it to whiten terrain (see snow_tinted()). */
        float snow_cover() const { return snow_cover_; }
        void set_snow_cover(float cover);

        int particle_count() const { return active_; }
        int splash_count() const { return static_cast<int>(splashes_.size()); }

    private:
        struct Drop
        {
            glm::vec3 pos, vel;
            float size, phase, rest;
            int bounces;
        };
        struct Splash
        {
            glm::vec3 pos;
            float age, life;
            bool hail;
        };

        float random(float low, float high);
        void respawn(Drop &drop, const glm::vec3 &centre, bool anywhere);
        float ground_at(float x, float z) const;
        bool ensure_resources();

        glm::vec3 volume_{36.0f, 24.0f, 36.0f};
        int max_particles_ = 5000;
        std::function<float(float, float)> ground_fn_;
        float gain_ = 0.01f, melt_ = 0.02f;
        float snow_cover_ = 0.0f;
        float time_ = 0.0f;
        PrecipitationType active_type_ = PrecipitationType::none;
        int active_ = 0;
        std::vector<Drop> drops_;
        std::vector<Splash> splashes_;
        std::vector<InstanceData3D> instances_;
        Mesh *quad_ = nullptr;
        unsigned resource_generation_ = 0;
        Bitmap *dot_ = nullptr;
        Bitmap *ring_ = nullptr;
        std::mt19937 rng_{54321u};
    };

    /**
     * Adjust a 3D environment for the current weather: fog thickens and greys (or whitens for snow), and the first
     * cloud layer's coverage and density rise with intensity. Call each frame on a copy of the clear-weather
     * environment before set_environment_3d().
     */
    void precipitation_apply_environment(Environment3D &environment, const PrecipitationConfig &weather);

    /** Blend `base` toward snow white by `cover` (0-1); for ground tints and terrain materials. */
    Colour snow_tinted(Colour base, float cover);

    /**
     * Screen-space effects drawn over the finished frame, in 2D or on top of a composited 3D surface: raindrops on the
     * lens that bead up and slide down, melting flakes in snow, and a haze that veils the view.
     */
    class PrecipitationLens
    {
    public:
        /** Spawn raindrops and melting flakes on the lens. Off by default: only the haze is drawn. */
        bool droplets = false;
        /** Advance the droplets for the current weather. */
        void update(float dt_seconds, const PrecipitationConfig &weather);
        /** Draw the haze and droplets over the rectangle (x, y, width, height). */
        void draw(float x, float y, float width, float height, const PrecipitationConfig &weather);
        /** Remove all droplets. */
        void clear();
        int droplet_count() const { return static_cast<int>(drops_.size()); }

    private:
        struct Droplet
        {
            float x, y, radius, age, slide, trail;
            bool sliding;
        };
        std::vector<Droplet> drops_;
        float accumulator_ = 0.0f;
        std::mt19937 rng_{777u};
    };

    /**
     * Looping weather sound made with the synthesiser: hiss of rain, clatter of hail, a faint wind for snow. The
     * sound steps between a few volume levels as intensity changes. Needs audio_fx_init().
     */
    class PrecipitationAudio
    {
    public:
        PrecipitationAudio() = default;
        PrecipitationAudio(const PrecipitationAudio &) = delete;
        PrecipitationAudio &operator=(const PrecipitationAudio &) = delete;
        ~PrecipitationAudio() { release(); }

        /** Pick the loop and volume for the weather; call each frame (cheap). `master` is 0-255; the default 26 is about 0.1 of full volume. */
        void update(const PrecipitationConfig &weather, int master = 10);
        /** Stop and free the samples. Call before audio shutdown. */
        void release();
        /** Volume (0-255) chosen for a weather and master volume; exposed for testing. */
        static int volume_for(const PrecipitationConfig &weather, int master = 10);

    private:
        Sample *samples_[3] = {nullptr, nullptr, nullptr};
        std::uint64_t voice_ = 0;
        int playing_type_ = -1;
        int playing_volume_ = -1;
    };

} // namespace sl
