#pragma once

#include "surface3d.h"

#include <cstdint>
#include <vector>

namespace sl
{
    /** One live particle of a ParticleSystem3D. Positions are in whatever frame the caller draws the system in. */
    struct Particle3D
    {
        glm::vec3 position{0.0f}, velocity{0.0f};
        /** Added to the velocity every second (a world "down" for falling debris, or "up" for smoke). */
        glm::vec3 acceleration{0.0f};
        /** Fraction of the velocity lost per second. */
        float drag = 0.0f;
        float age = 0.0f, life = 1.0f;
        /** Sphere radius in metres at birth and at death. */
        float size_start = 1.0f, size_end = 1.0f;
        /** Colour at birth and death; alpha fades between them as well. */
        Colour colour_start{255, 255, 255, 255}, colour_end{255, 255, 255, 0};
        /** Adds light (fire, plasma, flashes) instead of blending (smoke, dust). */
        bool additive = true;
    };

    /**
     * A pool of soft glowing or smoky blobs drawn as camera-independent spheres of a unit mesh: cheap, needs no
     * billboarding and works on every renderer. Positions are metres in the frame of the `frame` matrix passed to draw().
     */
    class ParticleSystem3D
    {
    public:
        explicit ParticleSystem3D(std::size_t max_particles = 4000) : capacity_(max_particles) {}

        /** Add a particle; the oldest are dropped once the pool is full. */
        void spawn(const Particle3D &particle);
        /** Age, move and remove particles. */
        void update(float dt_seconds);
        /** Draw every particle with `unit_sphere` (a sphere of radius 1) transformed by `frame`; call between scene begin and end. */
        void draw(const Mesh *unit_sphere, const glm::mat4 &frame = glm::mat4(1.0f)) const;
        void clear() { particles_.clear(); }
        std::size_t size() const { return particles_.size(); }
        const std::vector<Particle3D> &particles() const { return particles_; }

    private:
        std::size_t capacity_;
        std::vector<Particle3D> particles_;
    };

    /** The kinds of effect that spawn_effect3d() and Effect3D make. */
    enum class Effect3DKind
    {
        /** A short bright flash and a few sparks at a gun's muzzle; `direction` is the way it points. */
        muzzle_flash,
        /** A rising puff of smoke. */
        smoke,
        /** A flaring ball of fire with a smoke cloud and debris, for an ordinary explosion. */
        explosion,
        /** A cyan-green plasma burst. */
        plasma,
        /** The nuclear fireball, shock ring, rising stem and mushroom cap over about a minute; `scale` 1 is a 20 kt burst. */
        mushroom
    };

    /**
     * A timed effect that spawns its particles into a ParticleSystem3D over several seconds, so a mushroom cloud can grow.
     * Create one with the kind, position, an "up" direction (away from the ground) and a scale (roughly the fireball radius in metres
     * for explosions, a yield factor for mushroom), then call update() every frame until done().
     */
    class Effect3D
    {
    public:
        Effect3D(Effect3DKind kind, const glm::vec3 &position, const glm::vec3 &up, float scale = 1.0f, std::uint32_t seed = 1,
                 const glm::vec3 &direction = glm::vec3(0.0f));
        /** Advance by dt seconds, spawning into `system`. */
        void update(ParticleSystem3D &system, float dt_seconds);
        bool done() const { return age_ > duration_ && !pending_; }
        float age() const { return age_; }
        Effect3DKind kind() const { return kind_; }
        const glm::vec3 &position() const { return position_; }
        /** Brightness of the effect's light now, 0 to 1 (for a point light or a screen flash). */
        float light() const;
        /** Radius in metres at which the effect is still bright, for a light's range. */
        float light_radius() const;

    private:
        float random01();
        void burst(ParticleSystem3D &system);

        Effect3DKind kind_;
        glm::vec3 position_, up_, direction_;
        float scale_, age_ = 0.0f, duration_;
        std::uint32_t seed_;
        bool pending_ = true;
        float carry_ = 0.0f;
    };

} // namespace sl
