#pragma once

#include "body.h"

namespace Physics
{

struct Fixture
{
  b2Fixture* fixture = nullptr;
};


bool set_sensor(entt::registry& r, entt::entity entity, bool is_sensor)
{
  if(auto fixture = r.try_get< Fixture >(entity))
  {
    if(auto b2fixture = fixture->fixture)
    {
      b2fixture->SetSensor(is_sensor);
      return true;
    }
  }
  return false;
}

// bool create_chain(const std::vector< b2Vec2 >& points, bool is_loop = false)
// {
  
// }

void destroy_fixture(entt::registry& r, entt::entity id)
{
  Fixture& fixture = r.get< Fixture >(id);
  if(fixture.fixture)
    fixture.fixture->GetBody()->DestroyFixture(fixture.fixture);
  fixture.fixture = nullptr;
};


} // ::Physics

#ifdef FOWL_ENTT_MRUBY

MRUBY_COMPONENT_INTERFACE_BEGIN(Physics::Fixture)

  MRUBY_COMPONENT_GET
  {
    if(auto fixture = registry.try_get< Physics::Fixture >(entity))
    {
      if(auto f = fixture->fixture)
      {
        MRuby::HashBuilder hash(state);

        b2Shape* shape = f->GetShape();
        b2Shape::Type type = f->GetType();
        switch(type)
        {
        case b2Shape::e_circle:
          hash
            ("shape", "circle")
            ("radius", shape->m_radius);
          break;

        case b2Shape::e_polygon:
        {
          b2PolygonShape* poly = reinterpret_cast< b2PolygonShape* >(shape);

          int size = poly->GetChildCount();
          mrb_value points[size];
          for(int i = 0; i < size; ++i)
            MRuby::to_mrb(state, poly->m_vertices[i], points[i]);

          hash
            ("shape", "poly")
            ("points", mrb_ary_new_from_values(state, size, points));
        } break;

        case b2Shape::e_chain:
        {
          b2ChainShape* chain = reinterpret_cast< b2ChainShape* >(shape);

          int size = chain->GetChildCount();
          mrb_value points[size];
          for(int i = 0; i < size; ++i)
            MRuby::to_mrb(state, chain->m_vertices[i], points[i]);

          hash
            ("shape", "chain")
            ("points", mrb_ary_new_from_values(state, size, points));
        } break;

        default:
          return mrb_nil_value();
        }

        hash
          ("density", f->GetDensity())
          ("friction", f->GetFriction())
          ("is_sensor", f->IsSensor());

        return hash.self;
      }
    }
    return mrb_nil_value();
  }

  MRUBY_COMPONENT_SET
  {
    if(!argc || ! mrb_hash_p(argv[0]))
      return mrb_nil_value();

    b2FixtureDef fixture_def;
    std::string shape_type;
    entt::entity body_id;
    b2Vec2 offset(0,0);
    float scale = 1.0;

    MRuby::HashReader reader(state, argv[0]);
    reader
      ("body", body_id)
      ("shape", shape_type)
      ("scale", scale)
      ("offset", offset)
      ("density", fixture_def.density)
      ("friction", fixture_def.friction)
      ("is_sensor", fixture_def.isSensor)
    ;

    if(!registry.valid(body_id) || !registry.has< Physics::Body >(body_id))
      return mrb_nil_value();

    b2Body* body = registry.get< Physics::Body >(body_id).body;

    b2Fixture* fixture = nullptr;
    if(shape_type == "circle")
    {
      b2CircleShape circle_shape;
      fixture_def.shape = &circle_shape;

      reader("radius", circle_shape.m_radius);
      circle_shape.m_radius *= scale;
      circle_shape.m_p = offset;

      fixture = body->CreateFixture(&fixture_def);
    }
    else if(shape_type == "poly")
    {
      b2PolygonShape polygon_shape;
      fixture_def.shape = &polygon_shape;

      mrb_value points = mrb_hash_get(state, argv[0], mrb_symbol_value(mrb_intern_cstr(state, "points")));
      mrb_int points_size = ARY_LEN(mrb_ary_ptr(points));

      std::vector< b2Vec2 > vertices;
      vertices.resize(points_size);
      for(int i = 0; i < vertices.size(); ++i)
      {
        b2vec2(state, mrb_ary_entry(points, i), vertices[i]);
        vertices[i] *= scale;
        vertices[i] += offset;
      }
      polygon_shape.Set(&vertices[0], vertices.size());

      fixture = body->CreateFixture(&fixture_def);
    }
    else if(shape_type == "box")
    {
      b2PolygonShape box_shape;
      fixture_def.shape = &box_shape;

      float width = 0, height = 0, angle = 0;
      reader
        ("width", width)
        ("height", height)
        ("angle", angle)
      ;

      box_shape.SetAsBox(width, height, offset, angle);
      fixture = body->CreateFixture(&fixture_def);
    }
    else if(shape_type == "chain")
    {
      b2ChainShape chain_shape;
      fixture_def.shape = &chain_shape;

      std::vector< mrb_value > mrb_points;
      bool is_loop = false;

      reader
        ("points", mrb_points)
        ("loop", is_loop)
      ;

      std::vector< b2Vec2 > vertices;
      vertices.resize(mrb_points.size());
      for(int i = 0; i < vertices.size(); ++i)
      {
        b2vec2(state, mrb_points[i], vertices[i]);
        vertices[i] *= scale;
        vertices[i] += offset;
      }

      if(is_loop)
        chain_shape.CreateLoop(&vertices[0], vertices.size());
      else
        chain_shape.CreateChain(&vertices[0], vertices.size());
      fixture = body->CreateFixture(&fixture_def);
    }

    if(fixture)
    {
      fixture->SetUserData(reinterpret_cast< void* >(entity));
      registry.emplace_or_replace< Physics::Fixture >(entity, fixture);
      return argv[0];
    }

    return mrb_nil_value();

  }

MRUBY_COMPONENT_INTERFACE_END

#endif

