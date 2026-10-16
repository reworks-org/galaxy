#pragma once

#include "draw.h"

#include <cstddef>
#include <vector>

namespace sl
{

    /** One live particle owned by a ParticleEmitter. */
    struct Particle
    {
        /** Current horizontal position in pixels. */
        float x = 0.0f;
        /** Current vertical position in pixels. */
        float y = 0.0f;
        /** Horizontal velocity in pixels per second. */
        float vx = 0.0f;
        /** Vertical velocity in pixels per second. */
        float vy = 0.0f;
        /** Remaining lifetime in seconds. */
        float life = 0.0f;
        /** Lifetime assigned when the particle was spawned, in seconds. */
        float max_life = 1.0f;
    };

    /** Configures how a ParticleEmitter spawns and evolves particles. */
    struct EmitterConfig
    {
        /** Particles spawned per second while the emitter is active. */
        float emit_rate = 30.0f;
        /** Seconds each particle lives before disappearing. */
        float particle_lifetime = 1.0f;

        /** Initial speed range, in pixels/second. */
        float min_speed = 20.0f;
        /** Maximum initial speed, in pixels/second. */
        float max_speed = 80.0f;
        /** Spawn direction in degrees (0 = +x/right, -90 = up), and +/- spread around it. */
        float direction_degrees = -90.0f;
        /** Maximum angular deviation from the configured direction, in degrees. */
        float spread_degrees = 30.0f;

        /** Quad size in pixels at birth and at death. */
        float start_size = 8.0f;
        /** Quad size in pixels when the particle expires. */
        float end_size = 0.0f;
        /** Colour at birth and at death; channels (including alpha) fade linearly between them. */
        Colour start_colour{255, 255, 255, 255};
        /** Colour at particle expiry. */
        Colour end_colour{255, 255, 255, 0};

        /** Added to vertical velocity every second (positive falls down). */
        float gravity = 0.0f;
        /** Hard cap on live particles this emitter can own at once. */
        int max_particles = 500;
        /** Optional texture drawn per particle; nullptr draws a flat-coloured quad. */
        Bitmap *texture = nullptr;
    };

    /** A movable point that continuously spawns and owns its own particles. */
    class ParticleEmitter
    {
    public:
        ParticleEmitter() = default;
        /** Create an emitter from a configuration and initial pixel position. */
        explicit ParticleEmitter(const EmitterConfig &config, float x = 0.0f, float y = 0.0f);

        /** Move the emitter; newly spawned particles originate from this position. */
        void set_position(float x, float y);
        /** Return the current horizontal position in pixels. */
        float x() const;
        /** Return the current vertical position in pixels. */
        float y() const;

        /** Start/stop spawning new particles; existing ones keep updating/rendering. */
        void set_active(bool active);
        /** Return whether the emitter is spawning new particles. */
        bool is_active() const;

        /** Instantly remove all live particles. */
        void clear();

        /** Advance existing particles and spawn new ones for this frame. */
        void update(float dt_seconds);
        /** Draw every live particle owned by this emitter in a single batched draw call. */
        void render() const;

        /** Return the number of currently live particles. */
        std::size_t particle_count() const;

    private:
        EmitterConfig config_;
        float x_ = 0.0f;
        float y_ = 0.0f;
        bool active_ = true;
        float spawn_accumulator_ = 0.0f;
        std::vector<Particle> particles_;
    };

} // namespace sl
