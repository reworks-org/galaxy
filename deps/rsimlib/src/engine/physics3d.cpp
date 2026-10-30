/** @file
 * @brief Implements the lightweight 3D rigid box simulation used for debris.
 */

#include "physics3d.h"

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>

namespace sl
{
    namespace
    {
        glm::vec3 inverse_inertia_apply(const RigidBox3D &body, const glm::mat3 &rotation, const glm::vec3 &v)
        {
            const glm::vec3 h = body.half_extents;
            const glm::vec3 inertia = body.mass / 3.0f * glm::vec3(h.y * h.y + h.z * h.z, h.x * h.x + h.z * h.z, h.x * h.x + h.y * h.y);
            const glm::vec3 local = glm::transpose(rotation) * v;
            return rotation * (local / glm::max(inertia, glm::vec3(1.0e-6f)));
        }

        // Resolve one contact at `point` with unit surface normal `normal` (pointing out of the surface).
        void resolve_contact(RigidBox3D &body, const glm::vec3 &point, const glm::vec3 &normal)
        {
            const glm::mat3 rotation = glm::mat3_cast(body.orientation);
            const glm::vec3 r = point - body.position;
            const glm::vec3 velocity = body.velocity + glm::cross(body.angular_velocity, r);
            const float approach = glm::dot(velocity, normal);
            if (approach >= 0.0f)
                return;
            const float inv_mass = 1.0f / body.mass;
            // Slow contacts do not bounce, so resting bodies settle instead of jittering.
            const float e = approach < -1.0f ? body.restitution : 0.0f;
            const glm::vec3 rn = glm::cross(r, normal);
            const float denominator = inv_mass + glm::dot(normal, glm::cross(inverse_inertia_apply(body, rotation, rn), r));
            const float j = -(1.0f + e) * approach / denominator;
            glm::vec3 impulse = j * normal;

            glm::vec3 tangent = velocity - approach * normal;
            const float slide = glm::length(tangent);
            if (slide > 1.0e-4f)
            {
                tangent /= slide;
                const glm::vec3 rt = glm::cross(r, tangent);
                const float dt_denominator = inv_mass + glm::dot(tangent, glm::cross(inverse_inertia_apply(body, rotation, rt), r));
                const float jt = std::min(slide / dt_denominator, body.friction * j);
                impulse -= jt * tangent;
            }
            RigidBoxWorld3D::apply_impulse(body, impulse, point);
        }

        void corners(const RigidBox3D &body, glm::vec3 (&out)[8])
        {
            const glm::mat3 rotation = glm::mat3_cast(body.orientation);
            int n = 0;
            for (int x = -1; x <= 1; x += 2)
                for (int y = -1; y <= 1; y += 2)
                    for (int z = -1; z <= 1; z += 2)
                        out[n++] = body.position + rotation * (body.half_extents * glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)));
        }

        std::uint32_t next_random(std::uint32_t &state)
        {
            state = state * 1664525u + 1013904223u;
            return state >> 8;
        }

        float random_signed(std::uint32_t &state) { return static_cast<float>(next_random(state)) / 8388608.0f - 1.0f; }
    }

    RigidBox3D &RigidBoxWorld3D::add(const RigidBox3D &body)
    {
        if (bodies_.size() >= max_bodies && !bodies_.empty())
            bodies_.erase(bodies_.begin());
        bodies_.push_back(body);
        return bodies_.back();
    }

    void RigidBoxWorld3D::apply_impulse(RigidBox3D &body, const glm::vec3 &impulse, const glm::vec3 &point)
    {
        body.velocity += impulse / body.mass;
        body.angular_velocity += inverse_inertia_apply(body, glm::mat3_cast(body.orientation), glm::cross(point - body.position, impulse));
    }

    void RigidBoxWorld3D::explode(const glm::vec3 &centre, float radius, float strength, float spin)
    {
        std::uint32_t state = 0x9e3779b9u ^ static_cast<std::uint32_t>(bodies_.size() * 2654435761u);
        for (RigidBox3D &body : bodies_)
        {
            const glm::vec3 offset = body.position - centre;
            const float distance = glm::length(offset);
            if (distance >= radius)
                continue;
            const glm::vec3 direction = distance > 1.0e-4f ? offset / distance : glm::vec3(0.0f, 1.0f, 0.0f);
            const float falloff = 1.0f - distance / radius;
            body.velocity += direction * (strength * falloff);
            body.angular_velocity += glm::vec3(random_signed(state), random_signed(state), random_signed(state)) * spin * falloff;
        }
    }

    glm::mat4 RigidBoxWorld3D::model_matrix(const RigidBox3D &body)
    {
        return glm::translate(glm::mat4(1.0f), body.position) * glm::mat4_cast(body.orientation) *
               glm::scale(glm::mat4(1.0f), body.half_extents * 2.0f);
    }

    void RigidBoxWorld3D::integrate(RigidBox3D &body, float dt) const
    {
        body.velocity += gravity * dt;
        body.velocity *= std::max(0.0f, 1.0f - body.linear_damping * dt);
        body.angular_velocity *= std::max(0.0f, 1.0f - body.angular_damping * dt);
        body.position += body.velocity * dt;
        const float speed = glm::length(body.angular_velocity);
        if (speed > 1.0e-5f)
        {
            const glm::quat turn = glm::angleAxis(speed * dt, body.angular_velocity / speed);
            body.orientation = glm::normalize(turn * body.orientation);
        }
    }

    void RigidBoxWorld3D::collide(RigidBox3D &body) const
    {
        glm::vec3 points[8];
        if (has_floor)
        {
            corners(body, points);
            float deepest = 0.0f;
            for (const glm::vec3 &p : points)
                deepest = std::max(deepest, floor_height - p.y);
            if (deepest > 0.0f)
            {
                body.position.y += deepest;
                for (glm::vec3 &p : points)
                    p.y += deepest;
                for (const glm::vec3 &p : points)
                    if (p.y < floor_height + 1.0e-3f)
                        resolve_contact(body, p, {0.0f, 1.0f, 0.0f});
                body.touching = true;
            }
        }
        for (const StaticBox3D &box : obstacles)
        {
            corners(body, points);
            // Only the body's centre decides which face it entered through, so it is pushed out along one axis.
            const glm::vec3 centre = (box.min + box.max) * 0.5f, extent = (box.max - box.min) * 0.5f;
            bool inside = false;
            for (const glm::vec3 &p : points)
                if (glm::all(glm::greaterThan(p, box.min)) && glm::all(glm::lessThan(p, box.max)))
                    inside = true;
            if (!inside)
                continue;
            const glm::vec3 local = body.position - centre;
            const glm::vec3 depth = extent + body.half_extents - glm::abs(local);
            int axis = 0;
            if (depth.y < depth[axis])
                axis = 1;
            if (depth.z < depth[axis])
                axis = 2;
            glm::vec3 normal(0.0f);
            normal[axis] = local[axis] >= 0.0f ? 1.0f : -1.0f;
            body.position += normal * std::max(depth[axis], 0.0f) * 0.5f;
            corners(body, points);
            for (const glm::vec3 &p : points)
                if (glm::all(glm::greaterThan(p, box.min)) && glm::all(glm::lessThan(p, box.max)))
                    resolve_contact(body, p, normal);
            body.touching = true;
        }
    }

    void RigidBoxWorld3D::step(float dt, int substeps)
    {
        substeps = std::max(1, substeps);
        const float h = dt / static_cast<float>(substeps);
        for (RigidBox3D &body : bodies_)
            body.touching = false;
        for (int s = 0; s < substeps; ++s)
            for (RigidBox3D &body : bodies_)
            {
                integrate(body, h);
                collide(body);
            }
        for (RigidBox3D &body : bodies_)
            body.age += dt;
        bodies_.erase(std::remove_if(bodies_.begin(), bodies_.end(),
                                     [](const RigidBox3D &b) { return b.lifetime > 0.0f && b.age >= b.lifetime; }),
                      bodies_.end());
    }

} // namespace sl
