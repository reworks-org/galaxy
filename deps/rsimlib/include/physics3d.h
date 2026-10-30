#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

namespace sl
{

    /**
     * A small 3D rigid body for debris and other lightweight physical effects: an oriented box with mass, inertia,
     * linear and angular velocity. Bodies are simulated by RigidBoxWorld3D; they do not collide with each other.
     */
    struct RigidBox3D
    {
        /** Centre of mass in world units. */
        glm::vec3 position{0.0f};
        /** Metres (world units) per second. */
        glm::vec3 velocity{0.0f};
        glm::quat orientation{1.0f, 0.0f, 0.0f, 0.0f};
        /** Radians per second about each world axis. */
        glm::vec3 angular_velocity{0.0f};
        /** Half the box's size along each local axis. */
        glm::vec3 half_extents{0.5f};
        float mass = 1.0f;
        /** Bounciness on contact, 0 (dead) to 1 (perfectly elastic). */
        float restitution = 0.35f;
        /** Coulomb friction coefficient against surfaces. */
        float friction = 0.5f;
        /** Fraction of linear and angular velocity lost per second. */
        float linear_damping = 0.0f;
        float angular_damping = 0.0f;
        /** Seconds the body has existed. */
        float age = 0.0f;
        /** The body is removed once age reaches this; 0 lives forever. */
        float lifetime = 0.0f;
        /** Free for the caller, for example a colour index. */
        std::uint32_t user = 0;
        /** True after the last step if the body touched the floor or an obstacle. */
        bool touching = false;
    };

    /** An axis-aligned static box that bodies collide with. */
    struct StaticBox3D
    {
        glm::vec3 min{0.0f};
        glm::vec3 max{0.0f};
    };

    /**
     * Steps a set of RigidBox3D bodies under gravity with impulse-based contact against a horizontal floor and
     * static boxes. Contacts include rotation: a cube landing on a corner tumbles, and friction spins it up.
     */
    class RigidBoxWorld3D
    {
    public:
        /** Acceleration applied to every body each second. */
        glm::vec3 gravity{0.0f, -9.81f, 0.0f};
        /** Bodies rest on the plane y = floor_height when has_floor is true. */
        bool has_floor = true;
        float floor_height = 0.0f;
        /** Static boxes bodies collide with. Edit freely between steps. */
        std::vector<StaticBox3D> obstacles;
        /** Bodies are removed (oldest first) beyond this count so effects cannot grow without bound. */
        std::size_t max_bodies = 4096;

        /** Add a body and return a reference to it, valid until the next add, step or clear. */
        RigidBox3D &add(const RigidBox3D &body);
        /** Apply an impulse (mass times velocity change) at a world point, which also spins the body. */
        static void apply_impulse(RigidBox3D &body, const glm::vec3 &impulse, const glm::vec3 &point);
        /**
         * Push every body away from `centre` with an impulse falling off linearly to zero at `radius`, with a random
         * spin from `spin`. Bodies already outside the radius are not affected.
         */
        void explode(const glm::vec3 &centre, float radius, float strength, float spin = 6.0f);
        /** Advance by dt seconds, split into `substeps` for stable stacking of fast bodies; expired bodies are removed. */
        void step(float dt, int substeps = 2);
        void clear() { bodies_.clear(); }

        std::vector<RigidBox3D> &bodies() { return bodies_; }
        const std::vector<RigidBox3D> &bodies() const { return bodies_; }

        /** World-space model matrix (translation, rotation and scale to the box's full size) for rendering a unit cube. */
        static glm::mat4 model_matrix(const RigidBox3D &body);

    private:
        void integrate(RigidBox3D &body, float dt) const;
        void collide(RigidBox3D &body) const;
        std::vector<RigidBox3D> bodies_;
    };

} // namespace sl
