#include "physics.h"

#include <box2d/box2d.h>

#include <algorithm>
#include <utility>

namespace sl
{
    namespace
    {
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

    class ContactListener final : public b2ContactListener
    {
    public:
        explicit ContactListener(PhysicsWorld *owner) : owner(owner) {}

        void BeginContact(b2Contact *contact) override;
        void EndContact(b2Contact *contact) override;

        PhysicsWorld *owner;
    };

    struct PhysicsWorld
    {
        explicit PhysicsWorld(Vec2 gravity) : world(to_box2d(gravity)), listener(this)
        {
            world.SetContactListener(&listener);
        }

        b2World world;
        ContactListener listener;
        std::vector<PhysicsContact> contacts;
    };

    struct PhysicsBody
    {
        PhysicsWorld *owner = nullptr;
        b2Body *body = nullptr;
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
            b2WorldManifold manifold;
            contact->GetWorldManifold(&manifold);
            PhysicsContact event;
            event.type = type;
            event.body_a = reinterpret_cast<PhysicsBody *>(bodyA->GetUserData().pointer);
            event.body_b = reinterpret_cast<PhysicsBody *>(bodyB->GetUserData().pointer);
            event.point = from_box2d(manifold.points[0]);
            event.normal = {manifold.normal.x, manifold.normal.y};
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
        return new PhysicsWorld(gravity);
    }

    void destroy_physics_world(PhysicsWorld *world)
    {
        delete world;
    }

    void step_physics_world(PhysicsWorld *world, float time_step, int velocity_iterations, int position_iterations)
    {
        if (!world || time_step <= 0.0f)
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
        if (!world)
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
        return body;
    }

    void destroy_physics_body(PhysicsBody *body)
    {
        if (!body)
        {
            return;
        }
        if (body->owner && body->body)
        {
            body->owner->world.DestroyBody(body->body);
        }
        delete body;
    }

    void set_physics_body_transform(PhysicsBody *body, Vec2 position, float angle)
    {
        if (body && body->body)
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
        if (body && body->body)
        {
            body->body->SetLinearVelocity(to_box2d(velocity));
        }
    }

    void apply_physics_force(PhysicsBody *body, Vec2 force)
    {
        if (body && body->body)
        {
            body->body->ApplyForceToCenter(to_box2d(force), true);
        }
    }

    bool add_box_fixture(PhysicsBody *body, float width, float height, float density, float friction, float restitution)
    {
        if (!body || !body->body || width <= 0.0f || height <= 0.0f)
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
        return body->body->CreateFixture(&fixture) != nullptr;
    }

    bool add_circle_fixture(PhysicsBody *body, float radius, float density, float friction, float restitution)
    {
        if (!body || !body->body || radius <= 0.0f)
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
        return body->body->CreateFixture(&fixture) != nullptr;
    }

    bool add_polygon_fixture(PhysicsBody *body, const std::vector<Vec2> &vertices,
                             float density, float friction, float restitution)
    {
        if (!body || !body->body || vertices.size() < 3 || vertices.size() > b2_maxPolygonVertices)
        {
            return false;
        }
        b2Vec2 points[b2_maxPolygonVertices];
        for (std::size_t index = 0; index < vertices.size(); ++index)
        {
            points[index] = to_box2d(vertices[index]);
        }
        b2PolygonShape shape;
        shape.Set(points, static_cast<int32>(vertices.size()));
        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = std::max(0.0f, density);
        fixture.friction = std::max(0.0f, friction);
        fixture.restitution = std::clamp(restitution, 0.0f, 1.0f);
        return body->body->CreateFixture(&fixture) != nullptr;
    }
}
