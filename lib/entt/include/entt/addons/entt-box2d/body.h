#pragma once

namespace Physics
{

struct Body
{
  b2Body* body = nullptr;
};

bool set_position(Body& body, b2Vec2 position)
{
  if(body.body)
  {
    body.body->SetTransform(position, body.body->GetAngle());
    return true;
  }
  return false;
}

bool set_position(entt::registry& r, entt::entity entity, b2Vec2 position)
{
  if(Body* body = r.try_get< Body >(entity))
    return set_position(*body, position);
  return false;
}

bool get_position(Body& body, b2Vec2& position)
{
  if(body.body)
  {
    position = body.body->GetPosition();
    return true;
  }
  return false;
}

bool get_position(entt::registry& r, entt::entity entity, b2Vec2& position)
{
  if(Body* body = r.try_get< Body >(entity))
  {
    return get_position(*body, position);
  }
  return false;
}

bool get_transform(entt::registry& r, entt::entity entity, b2Transform& transform)
{
  if(Body* body = r.try_get< Body >(entity))
  {
    transform = body->body->GetTransform();
    return true;
  }
  return false;
}

bool set_linear_velocity(entt::registry& r, entt::entity entity, b2Vec2 velocity)
{
  if(auto body = r.try_get< Body >(entity))
  {
    body->body->SetLinearVelocity(velocity);
    return true;
  }
  return false;
}

bool get_linear_velocity(entt::registry& r, entt::entity entity, b2Vec2& velocity)
{
  if(auto body = r.try_get< Body >(entity))
  {
    velocity = body->body->GetLinearVelocity();
    return true;
  }
  return false;
}

bool apply_force_to_center(Body& body, b2Vec2 force)
{
  if(auto b2body = body.body)
  {
    b2body->ApplyForceToCenter(force, true);
    return true;
  }
  return false;
}

bool apply_force_to_center(entt::registry& r, entt::entity entity, b2Vec2 force)
{
  if(auto body = r.try_get< Body >(entity))
  {
    return apply_force_to_center(*body, force);
  }
  return false;
}

bool apply_torque(Body& body, float torque)
{
  if(auto b2body = body.body)
  {
    b2body->ApplyTorque(torque, true);
    return true;
  }
  return false;
}

bool apply_torque(entt::registry& r, entt::entity entity, float torque)
{
  if(auto body = r.try_get<Body>(entity))
  {
    return apply_torque(*body, torque);
  }
  return false;
}

void destroy_body(entt::registry& r, entt::entity id)
{
  Body& body = r.get< Body >(id);
  if(body.body)
    body.body->GetWorld()->DestroyBody( body.body );
  body.body = nullptr;
}


} // ::Physics



#ifdef FOWL_ENTT_MRUBY

MRUBY_COMPONENT_INTERFACE_BEGIN(Physics::Body)


  MRUBY_COMPONENT_GET
  {
    if(auto body = registry.try_get< Physics::Body >(entity))
    {
      if(auto b2body = body->body)
      {
        std::string body_type_str;
        switch(b2body->GetType())
        {
        case b2_dynamicBody:
          body_type_str = "dynamic";
          break;

        case b2_staticBody:
          body_type_str = "static";
          break;

        default:
          body_type_str = "kinematic";
        }
        
        MRuby::HashBuilder builder(state);
        builder
          ("type", body_type_str)
          ("position", b2body->GetPosition())
          ("linear_damping", b2body->GetLinearDamping())
          ("angular_damping", b2body->GetAngularDamping())
        ;
        return builder.self;
      }
    }
    return mrb_nil_value();
  }

  MRUBY_COMPONENT_SET
  {
    if(!argc || ! mrb_hash_p(argv[0]))
      return mrb_nil_value();

    b2BodyDef body_def;
    std::string body_type;

    MRuby::HashReader reader(state, argv[0]);
    reader
      ("type", body_type)
      ("position", body_def.position)
      ("linear_damping", body_def.linearDamping)
      ("angular_damping", body_def.angularDamping)
    ;

    Physics::World& world = registry.ctx< Physics::World >();

    b2BodyType b2_type = b2_dynamicBody;
    if(body_type == "static")
      b2_type = b2_staticBody;
    else if(body_type == "kinematic")
      b2_type = b2_kinematicBody;

    body_def.type = b2_type;

    b2Body* b2_body = world.world.CreateBody(&body_def);
    b2_body->SetUserData(reinterpret_cast< void* >(entity));

    auto& body = registry.emplace_or_replace< Physics::Body >(entity, b2_body);

    return argv[0];
  }

MRUBY_COMPONENT_INTERFACE_END

#endif

