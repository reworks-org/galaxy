#include "physics.h"

#include "draw.h"

#include <box2d/box2d.h>

#include <algorithm>
#include <cmath>
#include <utility>

namespace sl
{
    namespace
    {
        bool finite(Vec2 value)
        {
            return std::isfinite(value.x) && std::isfinite(value.y);
        }

        bool finite_fixture_properties(float density, float friction, float restitution)
        {
            return std::isfinite(density) && std::isfinite(friction) && std::isfinite(restitution);
        }

        b2Vec2 to_box2d(Vec2 value)
        {
            return {value.x / physics_pixels_per_meter, value.y / physics_pixels_per_meter};
        }

        Vec2 from_box2d(b2Vec2 value)
        {
            return {value.x * physics_pixels_per_meter, value.y * physics_pixels_per_meter};
        }

        b2BodyType to_box2d(BodyType type)
        {
            switch (type)
            {
            case BodyType::static_body:
                return b2_staticBody;
            case BodyType::kinematic_body:
                return b2_kinematicBody;
            case BodyType::dynamic_body:
                return b2_dynamicBody;
            }
            return b2_staticBody;
        }
    }

    struct PhysicsWorld;
    struct PhysicsBody;
    struct PhysicsJoint;

    class ContactListener final : public b2ContactListener
    {
    public:
        explicit ContactListener(PhysicsWorld *owner) : owner(owner) {}

        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;

        PhysicsWorld *owner;
    };

    /** Detaches joint handles from Box2D joints that die with one of their bodies. */
    class JointDestructionListener final : public b2DestructionListener
    {
    public:
        void SayGoodbye(b2Joint *joint) override;
        void SayGoodbye(b2Fixture *) override {}
    };

    struct PhysicsJoint
    {
        PhysicsWorld *owner = nullptr;
        b2Joint *joint = nullptr;
        JointType type = JointType::distance;
    };

    struct PhysicsWorld
    {
        explicit PhysicsWorld(Vec2 gravity) : world(to_box2d(gravity)), listener(this)
        {
            world.SetContactListener(&listener);
            world.SetDestructionListener(&destruction_listener);
        }

        b2World world;
        ContactListener listener;
        JointDestructionListener destruction_listener;
        std::vector<PhysicsContact> contacts;
        std::vector<PhysicsBody *> bodies;
        std::vector<PhysicsJoint *> joints;
        b2Body *ground = nullptr;
    };

    void JointDestructionListener::SayGoodbye(b2Joint *joint)
    {
        if (PhysicsJoint *handle = reinterpret_cast<PhysicsJoint *>(joint->GetUserData().pointer))
            handle->joint = nullptr;
    }

    struct PhysicsBody
    {
        PhysicsWorld *owner = nullptr;
        b2Body *body = nullptr;
        bool sensor = false;
        b2Filter filter;
    };

    namespace
    {
        void record_contact(PhysicsWorld *world, ContactType type, b2Contact *contact)
        {
            if (!world || !contact)
            {
                return;
            }
            b2Body *bodyA = contact->GetFixtureA()->GetBody();
            b2Body *bodyB = contact->GetFixtureB()->GetBody();
            PhysicsContact event;
            event.type = type;
            event.body_a = reinterpret_cast<PhysicsBody *>(bodyA->GetUserData().pointer);
            event.body_b = reinterpret_cast<PhysicsBody *>(bodyB->GetUserData().pointer);
            if (contact->GetManifold()->pointCount > 0)
            {
                b2WorldManifold manifold;
                contact->GetWorldManifold(&manifold);
                event.point = from_box2d(manifold.points[0]);
                event.normal = {manifold.normal.x, manifold.normal.y};
            }
            event.sensor = contact->GetFixtureA()->IsSensor() || contact->GetFixtureB()->IsSensor();
            world->contacts.push_back(event);
        }
    }

    void ContactListener::BeginContact(b2Contact *contact)
    {
        record_contact(owner, ContactType::begin, contact);
    }

    void ContactListener::EndContact(b2Contact *contact)
    {
        record_contact(owner, ContactType::end, contact);
    }

    PhysicsWorld *create_physics_world(Vec2 gravity)
    {
        return finite(gravity) ? new PhysicsWorld(gravity) : nullptr;
    }

    void destroy_physics_world(PhysicsWorld *world)
    {
        if (!world)
            return;
        world->world.SetContactListener(nullptr);
        world->world.SetDestructionListener(nullptr);
        for (PhysicsBody *body : world->bodies)
            delete body;
        for (PhysicsJoint *joint : world->joints)
            delete joint;
        delete world;
    }

    void step_physics_world(PhysicsWorld *world, float time_step, int velocity_iterations, int position_iterations)
    {
        if (!world || !std::isfinite(time_step) || time_step <= 0.0f)
        {
            return;
        }
        world->world.Step(time_step, std::max(1, velocity_iterations), std::max(1, position_iterations));
    }

    std::vector<PhysicsContact> poll_physics_contacts(PhysicsWorld *world)
    {
        if (!world)
        {
            return {};
        }
        std::vector<PhysicsContact> contacts = std::move(world->contacts);
        world->contacts.clear();
        return contacts;
    }

    PhysicsBody *create_physics_body(PhysicsWorld *world, BodyType type, Vec2 position)
    {
        if (!world || !finite(position))
        {
            return nullptr;
        }
        b2BodyDef definition;
        definition.type = to_box2d(type);
        definition.position = to_box2d(position);
        PhysicsBody *body = new PhysicsBody;
        body->owner = world;
        definition.userData.pointer = reinterpret_cast<uintptr_t>(body);
        body->body = world->world.CreateBody(&definition);
        if (!body->body)
        {
            delete body;
            return nullptr;
        }
        world->bodies.push_back(body);
        return body;
    }

    void destroy_physics_body(PhysicsBody *body)
    {
        if (!body)
        {
            return;
        }
        if (body->owner)
        {
            PhysicsWorld *owner = body->owner;
            if (body->body)
                owner->world.DestroyBody(body->body);
            owner->contacts.erase(std::remove_if(owner->contacts.begin(), owner->contacts.end(),
                                                 [body](const PhysicsContact &contact)
                                                 { return contact.body_a == body || contact.body_b == body; }),
                                  owner->contacts.end());
            owner->bodies.erase(std::remove(owner->bodies.begin(), owner->bodies.end(), body), owner->bodies.end());
        }
        delete body;
    }

    void set_physics_body_transform(PhysicsBody *body, Vec2 position, float angle)
    {
        if (body && body->body && finite(position) && std::isfinite(angle))
        {
            body->body->SetTransform(to_box2d(position), angle);
        }
    }

    Vec2 physics_body_position(const PhysicsBody *body)
    {
        return body && body->body ? from_box2d(body->body->GetPosition()) : Vec2{};
    }

    float physics_body_angle(const PhysicsBody *body)
    {
        return body && body->body ? body->body->GetAngle() : 0.0f;
    }

    Vec2 physics_body_velocity(const PhysicsBody *body)
    {
        return body && body->body ? from_box2d(body->body->GetLinearVelocity()) : Vec2{};
    }

    void set_physics_body_velocity(PhysicsBody *body, Vec2 velocity)
    {
        if (body && body->body && finite(velocity))
        {
            body->body->SetLinearVelocity(to_box2d(velocity));
        }
    }

    void apply_physics_force(PhysicsBody *body, Vec2 force)
    {
        if (body && body->body && finite(force))
        {
            body->body->ApplyForceToCenter(to_box2d(force), true);
        }
    }

    bool add_box_fixture(PhysicsBody *body, float width, float height, float density, float friction, float restitution)
    {
        if (!body || !body->body || !std::isfinite(width) || !std::isfinite(height) ||
            !finite_fixture_properties(density, friction, restitution) || width <= 0.0f || height <= 0.0f)
        {
            return false;
        }
        b2PolygonShape shape;
        shape.SetAsBox(width * 0.5f / physics_pixels_per_meter, height * 0.5f / physics_pixels_per_meter);
        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = std::max(0.0f, density);
        fixture.friction = std::max(0.0f, friction);
        fixture.restitution = std::clamp(restitution, 0.0f, 1.0f);
        fixture.isSensor = body->sensor;
        fixture.filter = body->filter;
        return body->body->CreateFixture(&fixture) != nullptr;
    }

    bool add_circle_fixture(PhysicsBody *body, float radius, float density, float friction, float restitution)
    {
        if (!body || !body->body || !std::isfinite(radius) ||
            !finite_fixture_properties(density, friction, restitution) || radius <= 0.0f)
        {
            return false;
        }
        b2CircleShape shape;
        shape.m_radius = radius / physics_pixels_per_meter;
        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = std::max(0.0f, density);
        fixture.friction = std::max(0.0f, friction);
        fixture.restitution = std::clamp(restitution, 0.0f, 1.0f);
        fixture.isSensor = body->sensor;
        fixture.filter = body->filter;
        return body->body->CreateFixture(&fixture) != nullptr;
    }

    bool add_polygon_fixture(PhysicsBody *body, const std::vector<Vec2> &vertices,
                             float density, float friction, float restitution)
    {
        if (!body || !body->body || vertices.size() < 3 || vertices.size() > b2_maxPolygonVertices ||
            !finite_fixture_properties(density, friction, restitution))
        {
            return false;
        }
        b2Vec2 points[b2_maxPolygonVertices];
        for (std::size_t index = 0; index < vertices.size(); ++index)
        {
            if (!finite(vertices[index]))
                return false;
            points[index] = to_box2d(vertices[index]);
        }
        bool has_area = false;
        for (std::size_t a = 0; a < vertices.size() && !has_area; ++a)
            for (std::size_t b = a + 1; b < vertices.size() && !has_area; ++b)
                for (std::size_t c = b + 1; c < vertices.size(); ++c)
                {
                    const float cross = (points[b].x - points[a].x) * (points[c].y - points[a].y) -
                                        (points[b].y - points[a].y) * (points[c].x - points[a].x);
                    if (std::abs(cross) > b2_linearSlop * b2_linearSlop)
                    {
                        has_area = true;
                        break;
                    }
                }
        if (!has_area)
            return false;
        b2PolygonShape shape;
        shape.Set(points, static_cast<int32>(vertices.size()));
        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = std::max(0.0f, density);
        fixture.friction = std::max(0.0f, friction);
        fixture.restitution = std::clamp(restitution, 0.0f, 1.0f);
        fixture.isSensor = body->sensor;
        fixture.filter = body->filter;
        return body->body->CreateFixture(&fixture) != nullptr;
    }

    void set_physics_body_sensor(PhysicsBody *body, bool sensor)
    {
        if (!body || !body->body)
            return;
        body->sensor = sensor;
        for (b2Fixture *fixture = body->body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            fixture->SetSensor(sensor);
    }

    void set_physics_body_filter(PhysicsBody *body, std::uint16_t category_bits, std::uint16_t mask_bits, std::int16_t group_index)
    {
        if (!body || !body->body)
            return;
        body->filter.categoryBits = category_bits;
        body->filter.maskBits = mask_bits;
        body->filter.groupIndex = group_index;
        for (b2Fixture *fixture = body->body->GetFixtureList(); fixture; fixture = fixture->GetNext())
            fixture->SetFilterData(body->filter);
    }

    namespace
    {
        class RayCollector final : public b2RayCastCallback
        {
        public:
            RayCollector(bool include_sensors, const PhysicsBody *ignore, bool closest_only, float length_pixels)
                : include_sensors(include_sensors), ignore(ignore), closest_only(closest_only), length_pixels(length_pixels) {}

            float ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction) override
            {
                const PhysicsBody *owner = reinterpret_cast<PhysicsBody *>(fixture->GetBody()->GetUserData().pointer);
                if ((fixture->IsSensor() && !include_sensors) || owner == ignore)
                    return -1.0f;
                PhysicsRayHit hit;
                hit.hit = true;
                hit.body = const_cast<PhysicsBody *>(owner);
                hit.point = from_box2d(point);
                hit.normal = {normal.x, normal.y};
                hit.fraction = fraction;
                hit.distance = fraction * length_pixels;
                if (closest_only)
                {
                    hits.assign(1, hit);
                    return fraction;
                }
                hits.push_back(hit);
                return 1.0f;
            }

            std::vector<PhysicsRayHit> hits;
            bool include_sensors;
            const PhysicsBody *ignore;
            bool closest_only;
            float length_pixels;
        };

        class BoxCollector final : public b2QueryCallback
        {
        public:
            BoxCollector(bool include_sensors, const b2Vec2 *point) : include_sensors(include_sensors), point(point) {}

            bool ReportFixture(b2Fixture *fixture) override
            {
                if ((fixture->IsSensor() && !include_sensors) || (point && !fixture->TestPoint(*point)))
                    return true;
                PhysicsBody *owner = reinterpret_cast<PhysicsBody *>(fixture->GetBody()->GetUserData().pointer);
                if (owner && std::find(bodies.begin(), bodies.end(), owner) == bodies.end())
                    bodies.push_back(owner);
                return true;
            }

            std::vector<PhysicsBody *> bodies;
            bool include_sensors;
            const b2Vec2 *point;
        };
    }

    std::vector<PhysicsRayHit> physics_raycast_all(PhysicsWorld *world, Vec2 from, Vec2 to, bool include_sensors,
                                                   const PhysicsBody *ignore)
    {
        if (!world || !finite(from) || !finite(to))
            return {};
        const float length = std::hypot(to.x - from.x, to.y - from.y);
        if (length <= 0.0f)
            return {};
        RayCollector collector(include_sensors, ignore, false, length);
        world->world.RayCast(&collector, to_box2d(from), to_box2d(to));
        std::sort(collector.hits.begin(), collector.hits.end(),
                  [](const PhysicsRayHit &a, const PhysicsRayHit &b) { return a.fraction < b.fraction; });
        return collector.hits;
    }

    PhysicsRayHit physics_raycast(PhysicsWorld *world, Vec2 from, Vec2 to, bool include_sensors, const PhysicsBody *ignore)
    {
        if (!world || !finite(from) || !finite(to))
            return {};
        const float length = std::hypot(to.x - from.x, to.y - from.y);
        if (length <= 0.0f)
            return {};
        RayCollector collector(include_sensors, ignore, true, length);
        world->world.RayCast(&collector, to_box2d(from), to_box2d(to));
        return collector.hits.empty() ? PhysicsRayHit{} : collector.hits.front();
    }

    std::vector<PhysicsBody *> physics_query_box(PhysicsWorld *world, Vec2 corner_a, Vec2 corner_b, bool include_sensors)
    {
        if (!world || !finite(corner_a) || !finite(corner_b))
            return {};
        const b2Vec2 a = to_box2d(corner_a);
        const b2Vec2 b = to_box2d(corner_b);
        b2AABB box;
        box.lowerBound = {std::min(a.x, b.x), std::min(a.y, b.y)};
        box.upperBound = {std::max(a.x, b.x), std::max(a.y, b.y)};
        BoxCollector collector(include_sensors, nullptr);
        world->world.QueryAABB(&collector, box);
        return collector.bodies;
    }

    std::vector<PhysicsBody *> physics_query_point(PhysicsWorld *world, Vec2 point, bool include_sensors)
    {
        if (!world || !finite(point))
            return {};
        const b2Vec2 p = to_box2d(point);
        b2AABB box;
        box.lowerBound = {p.x - b2_linearSlop, p.y - b2_linearSlop};
        box.upperBound = {p.x + b2_linearSlop, p.y + b2_linearSlop};
        BoxCollector collector(include_sensors, &p);
        world->world.QueryAABB(&collector, box);
        return collector.bodies;
    }

    namespace
    {
        PhysicsJoint *register_joint(PhysicsWorld *world, PhysicsJoint *handle, b2JointDef &definition, JointType type)
        {
            definition.userData.pointer = reinterpret_cast<uintptr_t>(handle);
            handle->owner = world;
            handle->type = type;
            handle->joint = world->world.CreateJoint(&definition);
            if (!handle->joint)
            {
                delete handle;
                return nullptr;
            }
            world->joints.push_back(handle);
            return handle;
        }

        bool joint_bodies_valid(PhysicsWorld *world, const PhysicsBody *a, const PhysicsBody *b)
        {
            return world && a && b && a != b && a->body && b->body && a->owner == world && b->owner == world;
        }

        template <typename T>
        T *joint_as(PhysicsJoint *handle, b2JointType type)
        {
            return handle && handle->joint && handle->joint->GetType() == type ? static_cast<T *>(handle->joint) : nullptr;
        }
    }

    PhysicsJoint *create_distance_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor_a, Vec2 anchor_b,
                                        float frequency_hz, float damping_ratio)
    {
        if (!joint_bodies_valid(world, a, b) || !finite(anchor_a) || !finite(anchor_b) ||
            !std::isfinite(frequency_hz) || !std::isfinite(damping_ratio))
            return nullptr;
        b2DistanceJointDef definition;
        definition.Initialize(a->body, b->body, to_box2d(anchor_a), to_box2d(anchor_b));
        if (frequency_hz > 0.0f)
        {
            definition.minLength = 0.0f;
            definition.maxLength = b2_maxFloat;
            b2LinearStiffness(definition.stiffness, definition.damping, frequency_hz, std::max(0.0f, damping_ratio),
                              a->body, b->body);
        }
        return register_joint(world, new PhysicsJoint, definition, JointType::distance);
    }

    PhysicsJoint *create_revolute_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor)
    {
        if (!joint_bodies_valid(world, a, b) || !finite(anchor))
            return nullptr;
        b2RevoluteJointDef definition;
        definition.Initialize(a->body, b->body, to_box2d(anchor));
        return register_joint(world, new PhysicsJoint, definition, JointType::revolute);
    }

    PhysicsJoint *create_prismatic_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor, Vec2 axis)
    {
        if (!joint_bodies_valid(world, a, b) || !finite(anchor) || !finite(axis) || (axis.x == 0.0f && axis.y == 0.0f))
            return nullptr;
        b2PrismaticJointDef definition;
        b2Vec2 direction(axis.x, axis.y);
        direction.Normalize();
        definition.Initialize(a->body, b->body, to_box2d(anchor), direction);
        return register_joint(world, new PhysicsJoint, definition, JointType::prismatic);
    }

    PhysicsJoint *create_weld_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor,
                                    float frequency_hz, float damping_ratio)
    {
        if (!joint_bodies_valid(world, a, b) || !finite(anchor) || !std::isfinite(frequency_hz) || !std::isfinite(damping_ratio))
            return nullptr;
        b2WeldJointDef definition;
        definition.Initialize(a->body, b->body, to_box2d(anchor));
        if (frequency_hz > 0.0f)
            b2AngularStiffness(definition.stiffness, definition.damping, frequency_hz, std::max(0.0f, damping_ratio),
                               a->body, b->body);
        return register_joint(world, new PhysicsJoint, definition, JointType::weld);
    }

    PhysicsJoint *create_wheel_joint(PhysicsWorld *world, PhysicsBody *a, PhysicsBody *b, Vec2 anchor, Vec2 axis,
                                     float frequency_hz, float damping_ratio)
    {
        if (!joint_bodies_valid(world, a, b) || !finite(anchor) || !finite(axis) || (axis.x == 0.0f && axis.y == 0.0f) ||
            !std::isfinite(frequency_hz) || !std::isfinite(damping_ratio))
            return nullptr;
        b2WheelJointDef definition;
        b2Vec2 direction(axis.x, axis.y);
        direction.Normalize();
        definition.Initialize(a->body, b->body, to_box2d(anchor), direction);
        b2LinearStiffness(definition.stiffness, definition.damping, std::max(0.0f, frequency_hz),
                          std::max(0.0f, damping_ratio), a->body, b->body);
        return register_joint(world, new PhysicsJoint, definition, JointType::wheel);
    }

    PhysicsJoint *create_mouse_joint(PhysicsWorld *world, PhysicsBody *body, Vec2 target, float max_force)
    {
        if (!world || !body || !body->body || body->owner != world || !finite(target) || !std::isfinite(max_force) ||
            max_force <= 0.0f || body->body->GetType() != b2_dynamicBody)
            return nullptr;
        if (!world->ground)
        {
            b2BodyDef ground;
            world->ground = world->world.CreateBody(&ground);
        }
        b2MouseJointDef definition;
        definition.bodyA = world->ground;
        definition.bodyB = body->body;
        definition.target = to_box2d(target);
        definition.maxForce = max_force / physics_pixels_per_meter * std::max(body->body->GetMass(), 1.0f);
        b2LinearStiffness(definition.stiffness, definition.damping, 5.0f, 0.7f, world->ground, body->body);
        PhysicsJoint *handle = register_joint(world, new PhysicsJoint, definition, JointType::mouse);
        if (handle)
            body->body->SetAwake(true);
        return handle;
    }

    void destroy_physics_joint(PhysicsJoint *joint)
    {
        if (!joint)
            return;
        if (PhysicsWorld *owner = joint->owner)
        {
            if (joint->joint)
            {
                joint->joint->GetUserData().pointer = 0;
                owner->world.DestroyJoint(joint->joint);
            }
            owner->joints.erase(std::remove(owner->joints.begin(), owner->joints.end(), joint), owner->joints.end());
        }
        delete joint;
    }

    bool physics_joint_active(const PhysicsJoint *joint)
    {
        return joint && joint->joint;
    }

    JointType physics_joint_type(const PhysicsJoint *joint)
    {
        return joint ? joint->type : JointType::distance;
    }

    void set_mouse_joint_target(PhysicsJoint *joint, Vec2 target)
    {
        if (b2MouseJoint *mouse = joint_as<b2MouseJoint>(joint, e_mouseJoint); mouse && finite(target))
            mouse->SetTarget(to_box2d(target));
    }

    void set_joint_motor(PhysicsJoint *joint, float speed, float max_torque)
    {
        if (!std::isfinite(speed) || !std::isfinite(max_torque))
            return;
        const float torque = std::max(0.0f, max_torque) / physics_pixels_per_meter;
        if (b2RevoluteJoint *revolute = joint_as<b2RevoluteJoint>(joint, e_revoluteJoint))
        {
            revolute->EnableMotor(torque > 0.0f);
            revolute->SetMotorSpeed(speed);
            revolute->SetMaxMotorTorque(torque);
        }
        else if (b2WheelJoint *wheel = joint_as<b2WheelJoint>(joint, e_wheelJoint))
        {
            wheel->EnableMotor(torque > 0.0f);
            wheel->SetMotorSpeed(speed);
            wheel->SetMaxMotorTorque(torque);
        }
    }

    void set_prismatic_motor(PhysicsJoint *joint, float speed, float max_force)
    {
        if (b2PrismaticJoint *prismatic = joint_as<b2PrismaticJoint>(joint, e_prismaticJoint);
            prismatic && std::isfinite(speed) && std::isfinite(max_force))
        {
            const float force = std::max(0.0f, max_force) / physics_pixels_per_meter;
            prismatic->EnableMotor(force > 0.0f);
            prismatic->SetMotorSpeed(speed / physics_pixels_per_meter);
            prismatic->SetMaxMotorForce(force);
        }
    }

    void set_revolute_limits(PhysicsJoint *joint, float lower, float upper)
    {
        if (b2RevoluteJoint *revolute = joint_as<b2RevoluteJoint>(joint, e_revoluteJoint);
            revolute && std::isfinite(lower) && std::isfinite(upper) && lower <= upper)
        {
            revolute->SetLimits(lower, upper);
            revolute->EnableLimit(true);
        }
    }

    void set_prismatic_limits(PhysicsJoint *joint, float lower, float upper)
    {
        if (b2PrismaticJoint *prismatic = joint_as<b2PrismaticJoint>(joint, e_prismaticJoint);
            prismatic && std::isfinite(lower) && std::isfinite(upper) && lower <= upper)
        {
            prismatic->SetLimits(lower / physics_pixels_per_meter, upper / physics_pixels_per_meter);
            prismatic->EnableLimit(true);
        }
    }

    Vec2 physics_joint_anchor_a(const PhysicsJoint *joint)
    {
        return joint && joint->joint ? from_box2d(joint->joint->GetAnchorA()) : Vec2{};
    }

    Vec2 physics_joint_anchor_b(const PhysicsJoint *joint)
    {
        return joint && joint->joint ? from_box2d(joint->joint->GetAnchorB()) : Vec2{};
    }

    namespace
    {
        /** Routes Box2D's debug drawing through the 2D primitives. */
        class DebugDrawer final : public b2Draw
        {
        public:
            explicit DebugDrawer(const PhysicsDebugDraw &options) : options_(options) {}

            void DrawPolygon(const b2Vec2 *vertices, int32 count, const b2Color &color) override
            {
                const Colour outline = to_colour(color, 1.0f);
                for (int i = 0; i < count; ++i)
                {
                    const Vec2 a = screen(vertices[i]), b = screen(vertices[(i + 1) % count]);
                    line(screen_bitmap(), a.x, a.y, b.x, b.y, outline);
                }
            }

            void DrawSolidPolygon(const b2Vec2 *vertices, int32 count, const b2Color &color) override
            {
                const Colour fill = to_colour(color, options_.fill_alpha);
                const Vec2 first = screen(vertices[0]);
                for (int i = 1; i + 1 < count; ++i)
                {
                    const Vec2 b = screen(vertices[i]), c = screen(vertices[i + 1]);
                    trianglefill(screen_bitmap(), first.x, first.y, b.x, b.y, c.x, c.y, fill);
                }
                DrawPolygon(vertices, count, color);
            }

            void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override
            {
                const Vec2 c = screen(center);
                circle(screen_bitmap(), c.x, c.y, radius * pixels(), to_colour(color, 1.0f));
            }

            void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override
            {
                const Vec2 c = screen(center);
                circlefill(screen_bitmap(), c.x, c.y, radius * pixels(), to_colour(color, options_.fill_alpha));
                DrawCircle(center, radius, color);
                const Vec2 tip = screen(center + radius * axis);
                line(screen_bitmap(), c.x, c.y, tip.x, tip.y, to_colour(color, 1.0f));
            }

            void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override
            {
                const Vec2 a = screen(p1), b = screen(p2);
                line(screen_bitmap(), a.x, a.y, b.x, b.y, to_colour(color, 1.0f));
            }

            void DrawTransform(const b2Transform &xf) override
            {
                const Vec2 origin = screen(xf.p);
                const Vec2 x = screen(xf.p + 0.25f * xf.q.GetXAxis());
                const Vec2 y = screen(xf.p + 0.25f * xf.q.GetYAxis());
                line(screen_bitmap(), origin.x, origin.y, x.x, x.y, Colour{230, 80, 80});
                line(screen_bitmap(), origin.x, origin.y, y.x, y.y, Colour{80, 230, 80});
            }

            void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override
            {
                const Vec2 c = screen(p);
                circlefill(screen_bitmap(), c.x, c.y, size * 0.5f, to_colour(color, 1.0f));
            }

        private:
            static Bitmap *screen_bitmap() { return sl::screen; }

            float pixels() const { return physics_pixels_per_meter * options_.scale; }

            Vec2 screen(const b2Vec2 &value) const
            {
                return {value.x * pixels() + options_.offset.x, value.y * pixels() + options_.offset.y};
            }

            static Colour to_colour(const b2Color &color, float alpha)
            {
                const auto channel = [](float value)
                { return static_cast<Uint8>(std::clamp(value, 0.0f, 1.0f) * 255.0f + 0.5f); };
                return Colour{channel(color.r), channel(color.g), channel(color.b), channel(alpha)};
            }

            PhysicsDebugDraw options_;
        };
    }

    void debug_draw_physics(PhysicsWorld *world, const PhysicsDebugDraw &options)
    {
        if (!world || !sl::screen || !std::isfinite(options.scale) || options.scale <= 0.0f || !finite(options.offset))
            return;
        DebugDrawer drawer(options);
        std::uint32_t flags = 0;
        if (options.shapes)
            flags |= b2Draw::e_shapeBit;
        if (options.joints)
            flags |= b2Draw::e_jointBit;
        if (options.bounding_boxes)
            flags |= b2Draw::e_aabbBit;
        if (options.centers_of_mass)
            flags |= b2Draw::e_centerOfMassBit;
        drawer.SetFlags(flags);
        world->world.SetDebugDraw(&drawer);
        world->world.DebugDraw();
        world->world.SetDebugDraw(nullptr);
    }
}
