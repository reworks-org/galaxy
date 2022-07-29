#pragma once


#ifdef FOWL_ENTT_MRUBY
# include <mruby/proc.h>
# include <mruby/hash.h>
# include "mruby-bindings.h"
#endif

#ifdef FOWL_ENTT_SFML
# include "sfml-debugdraw.h"
#endif

#include "world.h"
#include "fixture.h"

namespace Physics
{

  template< typename Derived >
  struct RegistryMixin
  {
    inline Derived& derived() { return *static_cast< Derived* >(this); }

    void physics_init()
    {
      Derived& registry = derived();

      registry.template on_destroy< Fixture >().template connect< &destroy_fixture >();
      registry.template on_destroy< Body >().template connect< &destroy_body >();

      // registry.template on_construct< World >().template connect< &construct_world >();
      World& world = registry.template set< World >();

    }

    World& physics_world()
    {
      return derived().template ctx< World >();
    }

#ifdef FOWL_ENTT_SFML

    void physics_sfml_init()
    {
      if constexpr(std::is_base_of_v< UI::RegistryMixin< Derived >, Derived >)
      {
        if(auto window = derived().template try_ctx< sf::RenderWindow* >())
        {
          auto& self = derived();
          auto& world = self.physics_world();
          auto& debugDraw = self.template set< Physics::SFMLDebugDraw >(*window, world.scale);
          debugDraw.SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_centerOfMassBit);
          world.world.SetDebugDraw(& debugDraw);
        }
      }
    }

# ifdef FOWL_ENTT_MRUBY
    static mrb_value physics_mrb_debug_draw(mrb_state* state, mrb_value self)
    {
      Derived* registry = physics_mrb_registry_ptr(state, self);
      if(!registry)
        return mrb_nil_value();

      auto& world = registry->physics_world();
      world.world.DebugDraw();

      return self;
    }
# endif

#endif

#ifdef FOWL_ENTT_MRUBY

    using MRubyRegistryMixin = MRuby::RegistryMixin< Derived >;
    using MRubyRegistryPtr = typename MRubyRegistryMixin::MRubyRegistryPtr;

    void physics_mrb_init(mrb_state* state)
    {
      // Set up some methods, if Derived includes MRubyRegistryMixin
      if constexpr (std::is_base_of< MRubyRegistryMixin, Derived >::value)
      {
        auto registry_class = MRuby::Class{ state, mrb_class_get(state, "Registry") };
        registry_class
          .define_method("world_get_gravity", physics_mrb_registry_world_get_gravity, MRB_ARGS_NONE())
          .define_method("world_gravity", physics_mrb_registry_world_get_gravity, MRB_ARGS_NONE())
          .define_method("world_set_gravity", physics_mrb_registry_world_set_gravity, MRB_ARGS_REQ(1))
          .define_method("world_gravity=", physics_mrb_registry_world_set_gravity, MRB_ARGS_REQ(1))
          .define_method("world_step", physics_mrb_registry_world_step, MRB_ARGS_REQ(1))
# ifdef FOWL_ENTT_SFML
          .define_method("draw_debug_world", physics_mrb_debug_draw, MRB_ARGS_NONE())
# endif
        ;

        if(! derived().template try_ctx< entt::dispatcher >())
          derived().template set< entt::dispatcher >();
        auto& dispatcher = derived().template ctx< entt::dispatcher >();
      }
    }

    static Derived* physics_mrb_registry_ptr(mrb_state* state, mrb_value self)
    {
      return Derived::mrb_value_to_registry(state, self);
    }

    static mrb_value physics_mrb_registry_world_set_gravity(mrb_state* state, mrb_value self)
    {
      Derived* registry = physics_mrb_registry_ptr(state, self);
      if(! registry)
        return mrb_nil_value();

      mrb_value mrb_gravity;
      if(mrb_get_args(state, "A", &mrb_gravity) != 1)
        return mrb_nil_value();

      World& world = registry->physics_world();
      b2Vec2 gravity;
      if(MRuby::from_mrb(state, mrb_gravity, gravity))
      {
        world.world.SetGravity(gravity);
        return self;
      }

      return mrb_nil_value();
    }

    static mrb_value physics_mrb_registry_world_get_gravity(mrb_state* state, mrb_value self)
    {
      Derived* registry = physics_mrb_registry_ptr(state, self);
      if(! registry)
        return mrb_nil_value();

      World& world = registry->physics_world();
      b2Vec2 gravity = world.world.GetGravity();

      mrb_value mrb_gravity[2];
      mrb_gravity[0] = mrb_float_value(state, gravity.x);
      mrb_gravity[1] = mrb_float_value(state, gravity.y);

      return mrb_ary_new_from_values(state, 2, mrb_gravity);
    }

    static mrb_value physics_mrb_registry_world_step(mrb_state* state, mrb_value self)
    {
      Derived* registry = physics_mrb_registry_ptr(state, self);
      if(! registry)
        return mrb_nil_value();

      mrb_float dt;
      if(mrb_get_args(state, "f", &dt) != 1)
        return mrb_nil_value();

      World& world = registry->physics_world();
      world.world.Step(dt, world.velocityIterations, world.positionIterations);

      return self;
    }

#endif

  };

}
