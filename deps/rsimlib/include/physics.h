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
    struct PhysicsJoint;

    /** Kinds of joint that can connect bodies. */
    enum class JointType
    {
        distance,
        revolute,
        prismatic,
        weld,
        wheel,
        mouse
    };

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
        /** True when either body is a sensor (see set_physics_body_sensor()); sensor contacts have no point or normal. */
        bool sensor = false;
    };

    /** The result of a ray cast against the physics world. */
    struct PhysicsRayHit
    {
        /** True when the ray hit something. */
        bool hit = false;
        /** The body that was hit. */
        PhysicsBody *body = nullptr;
        /** Hit position in screen pixels. */
        Vec2 point;
        /** Surface normal at the hit, pointing back toward the ray origin side. */
        Vec2 normal;
        /** Fraction (0-1) of the way along the ray where the hit occurred. */
        float fraction = 0.0f;
        /** Distance from the ray start in pixels. */
        float distance = 0.0f;
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

    /**
     * Make every fixture of the body (including ones added later) a sensor: it reports begin/end contacts with
     * PhysicsContact::sensor set but does not collide, and ray casts ignore it unless include_sensors is true.
     */
    void set_physics_body_sensor(PhysicsBody *body, bool sensor);
    /**
     * Set collision filtering for every fixture of the body (including ones added later). Two bodies collide when
     * each one's category bits overlap the other's mask bits; a shared positive group index forces collision and a
     * shared negative one prevents it. The defaults are category 1, mask all bits, group 0.
     */
    void set_physics_body_filter(PhysicsBody *body, std::uint16_t category_bits, std::uint16_t mask_bits, std::int16_t group_index = 0);

    /** Cast a ray from `from` to `to` (pixels) and return the closest hit. Bodies in `ignore` are skipped, as are sensors unless include_sensors. */
    PhysicsRayHit physics_raycast(PhysicsWorld *world, Vec2 from, Vec2 to, bool include_sensors = false,
                                  const PhysicsBody *ignore = nullptr);
    /** Cast a ray and return every hit, nearest first. */
    std::vector<PhysicsRayHit> physics_raycast_all(PhysicsWorld *world, Vec2 from, Vec2 to, bool include_sensors = false,
                                                   const PhysicsBody *ignore = nullptr);
    /** Return the bodies whose fixtures overlap the rectangle between two pixel-space corners (by bounding box). */
    std::vector<PhysicsBody *> physics_query_box(PhysicsWorld *world, Vec2 corner_a, Vec2 corner_b, bool include_sensors = false);
    /** Return the bodies with a fixture containing the pixel-space point. */
    std::vector<PhysicsBody *> physics_query_point(PhysicsWorld *world, Vec2 point, bool include_sensors = false);

    /** Add a box fixture using full width and height in pixels. */
    bool add_box_fixture(PhysicsBody *body, float width, float height, float density = 1.0f,
                         float friction = 0.3f, float restitution = 0.0f);
    /** Add a circular fixture using radius in pixels. */
    bool add_circle_fixture(PhysicsBody *body, float radius, float density = 1.0f,
                            float friction = 0.3f, float restitution = 0.0f);
    /** Add a convex polygon fixture using pixel-space vertices. */
    bool add_polygon_fixture(PhysicsBody *body, const std::vector<Vec2> &vertices,
                             float density = 1.0f, float friction = 0.3f, float restitution = 0.0f);

    /**
     * Joints connect two bodies. Anchors and axes are given in pixel-space world coordinates while the bodies are in
     * their starting pose. A joint handle stays valid until destroy_physics_joint() or the world is destroyed; if one of
     * its bodies is destroyed first the joint becomes inactive (physics_joint_active() returns false) and is ignored.
     */
    /** Keep two anchor points at a fixed length. A non-zero frequency_hz makes it a damped spring. */
    PhysicsJoint *create_distance_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor_a, Vec2 anchor_b,
                                        float frequency_hz = 0.0f, float damping_ratio = 0.0f);
    /** Add a revolute (hinge) joint: pin two bodies together at a point so they can rotate about it. */
    PhysicsJoint *create_revolute_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor);
    /** Let body b slide along an axis relative to body a. */
    PhysicsJoint *create_prismatic_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor, Vec2 axis);
    /** Glue two bodies together. A non-zero frequency_hz makes the weld flexible. */
    PhysicsJoint *create_weld_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor,
                                    float frequency_hz = 0.0f, float damping_ratio = 0.0f);
    /** Suspension-style joint: body b rotates freely at the anchor and moves along the axis on a spring. */
    PhysicsJoint *create_wheel_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor, Vec2 axis,
                                     float frequency_hz = 4.0f, float damping_ratio = 0.7f);
    /** Drag a dynamic body toward a target point (for example the mouse) with a limited force. */
    PhysicsJoint *create_mouse_joint(PhysicsWorld *world, PhysicsBody *body, Vec2 target, float max_force = 10000.0f);
    /** Remove a joint. Safe to call for joints whose bodies were already destroyed. */
    void destroy_physics_joint(PhysicsJoint *joint);
    /** True while the joint is still connecting live bodies. */
    bool physics_joint_active(const PhysicsJoint *joint);
    JointType physics_joint_type(const PhysicsJoint *joint);
    /** Move the target of a mouse joint. */
    void set_mouse_joint_target(PhysicsJoint *joint, Vec2 target);
    /** Drive a revolute or wheel joint at a speed in radians per second, limited to max_torque. */
    void set_joint_motor(PhysicsJoint *joint, float speed, float max_torque);
    /** Drive a prismatic joint at a speed in pixels per second, limited to max_force. */
    void set_prismatic_motor(PhysicsJoint *joint, float speed, float max_force);
    /** Limit a revolute joint's angle (radians, relative to the starting pose). */
    void set_revolute_limits(PhysicsJoint *joint, float lower, float upper);
    /** Limit a prismatic joint's translation in pixels. */
    void set_prismatic_limits(PhysicsJoint *joint, float lower, float upper);
    /** Pixel-space anchor positions, for drawing. */
    Vec2 physics_joint_anchor_a(const PhysicsJoint *joint);
    Vec2 physics_joint_anchor_b(const PhysicsJoint *joint);

    /** What debug_draw_physics() shows. Colours are fixed per category; alpha is applied to fills. */
    struct PhysicsDebugDraw
    {
        bool shapes = true;
        bool joints = true;
        bool bounding_boxes = false;
        bool centers_of_mass = false;
        /** Added to every pixel position, e.g. a camera offset. */
        Vec2 offset;
        /** Pixel scale around the origin, applied before the offset. */
        float scale = 1.0f;
        /** Fill opacity of shapes (0-1). */
        float fill_alpha = 0.25f;
    };

    /** Draw every body, joint and optionally bounds to the screen using the 2D primitives. Call it during drawing. */
    void debug_draw_physics(PhysicsWorld *world, const PhysicsDebugDraw &options = {});
}
