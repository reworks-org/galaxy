#pragma once

#include <cstdint>
#include <vector>

namespace sl
{
    /** 2D position or vector expressed in screen pixels. */
    struct Vec2
    {
        /** Horizontal component in screen pixels or pixels per second. */
        float x = 0.0f;
        /** Vertical component in screen pixels or pixels per second. */
        float y = 0.0f;
    };

    /** Motion model used when creating a physics body. */
    enum class BodyType
    {
        static_body,
        kinematic_body,
        dynamic_body
    };

    struct PhysicsWorld;
    struct PhysicsBody;

    /** Whether a contact has just begun or ended. */
    enum class ContactType
    {
        begin,
        end
    };

    /** A contact event reported by the most recent physics step. */
    struct PhysicsContact
    {
        /** Whether the contact began or ended. */
        ContactType type = ContactType::begin;
        /** First body participating in the contact. */
        PhysicsBody *body_a = nullptr;
        /** Second body participating in the contact. */
        PhysicsBody *body_b = nullptr;
        /** Contact point in screen pixels. */
        Vec2 point;
        /** Contact normal pointing from the first body toward the second. */
        Vec2 normal;
    };

    /** Pixels represented by one Box2D metre in the wrapper. */
    inline constexpr float physics_pixels_per_meter = 64.0f;

    /** Create a physics world. Gravity uses pixels per second squared. */
    PhysicsWorld *create_physics_world(Vec2 gravity = {0.0f, 980.0f});
    /** Destroy a world and all bodies owned by it. */
    void destroy_physics_world(PhysicsWorld *world);
    /** Advance the simulation by seconds. */
    void step_physics_world(PhysicsWorld *world, float time_step,
                            int velocity_iterations = 8, int position_iterations = 3);
    /** Return and clear contact events generated since the previous call. */
    std::vector<PhysicsContact> poll_physics_contacts(PhysicsWorld *world);

    /** Create a body at a pixel-space position. */
    PhysicsBody *create_physics_body(PhysicsWorld *world, BodyType type, Vec2 position = {});
    /** Destroy a body owned by its world. */
    void destroy_physics_body(PhysicsBody *body);
    /** Set a body's pixel-space position and angle in radians. */
    void set_physics_body_transform(PhysicsBody *body, Vec2 position, float angle = 0.0f);
    /** Return a body's pixel-space position, or zero for an invalid body. */
    Vec2 physics_body_position(const PhysicsBody *body);
    /** Return a body's angle in radians, or zero for an invalid body. */
    float physics_body_angle(const PhysicsBody *body);
    /** Return a body's linear velocity in pixels per second. */
    Vec2 physics_body_velocity(const PhysicsBody *body);
    /** Set a body's linear velocity in pixels per second. */
    void set_physics_body_velocity(PhysicsBody *body, Vec2 velocity);
    /** Add a force at the body's centre, expressed in pixel-space units. */
    void apply_physics_force(PhysicsBody *body, Vec2 force);

    /** Add a box fixture using full width and height in pixels. */
    bool add_box_fixture(PhysicsBody *body, float width, float height, float density = 1.0f,
                         float friction = 0.3f, float restitution = 0.0f);
    /** Add a circular fixture using radius in pixels. */
    bool add_circle_fixture(PhysicsBody *body, float radius, float density = 1.0f,
                            float friction = 0.3f, float restitution = 0.0f);
    /** Add a convex polygon fixture using pixel-space vertices. */
    bool add_polygon_fixture(PhysicsBody *body, const std::vector<Vec2> &vertices,
                             float density = 1.0f, float friction = 0.3f, float restitution = 0.0f);
}
