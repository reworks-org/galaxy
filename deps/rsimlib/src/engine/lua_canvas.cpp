/** @file
 * @brief Implements persistent Lua-driven drawing, sprites, and audio bindings.
 */

#include "lua_canvas.h"

#include "audio.h"
#include "display.h"
#include "graphics_fx.h"
#include "physics.h"
#include "system.h"

#include <sol/sol.hpp>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sl
{
    namespace
    {
        enum class DrawingType
        {
            pixel,
            line,
            circle,
            circlefill,
            rect,
            rectfill,
            ellipse,
            ellipsefill,
            triangle,
            trianglefill,
            sprite,
            sprite_stretched,
            sprite_rotated,
            sprite_rotated_stretched
        };

        struct DrawingCommand
        {
            DrawingType type;
            float x1 = 0.0f;
            float y1 = 0.0f;
            float x2 = 0.0f;
            float y2 = 0.0f;
            float x3 = 0.0f;
            float y3 = 0.0f;
            Colour colour{};
            std::string sprite_id;
        };

        Colour make_colour(int red, int green, int blue, sol::optional<int> alpha)
        {
            return {
                static_cast<Uint8>(std::clamp(red, 0, 255)),
                static_cast<Uint8>(std::clamp(green, 0, 255)),
                static_cast<Uint8>(std::clamp(blue, 0, 255)),
                static_cast<Uint8>(std::clamp(alpha.value_or(255), 0, 255))};
        }
    }

    struct LuaCanvas::Implementation
    {
        LuaRuntime runtime;
        Colour background{0, 0, 0};
        std::vector<DrawingCommand> commands;
        std::filesystem::path asset_root{"assets"};
        std::unordered_map<std::string, Bitmap *> sprites;
        std::unordered_map<std::string, Bitmap *> render_targets;
        std::unordered_map<std::string, Sample *> sounds;
        std::unordered_map<std::string, Stream *> music;
        std::unordered_map<std::string, Shader *> shaders;
        std::unordered_map<std::string, StorageBuffer *> storage_buffers;

        ScreenShake screen_shake;
        Bloom bloom;
        Vignette vignette;
        ColourAdjust colour_adjust;

        std::uint64_t next_physics_handle = 1;
        std::unordered_map<std::uint64_t, PhysicsWorld *> physics_worlds;
        std::unordered_map<std::uint64_t, PhysicsBody *> physics_bodies;
        std::unordered_map<std::uint64_t, std::uint64_t> physics_body_worlds;

        std::optional<std::filesystem::path> resolve_asset_path(const std::string &path) const
        {
            const std::filesystem::path asset_path{path};
            if (path.empty() || asset_path.is_absolute() ||
                std::find(asset_path.begin(), asset_path.end(), std::filesystem::path{".."}) != asset_path.end())
            {
                return std::nullopt;
            }
            return asset_root / asset_path;
        }

        void add_drawing(
            DrawingType type, float x1, float y1, float x2, float y2,
            float x3, float y3, Colour colour)
        {
            commands.push_back({type, x1, y1, x2, y2, x3, y3, colour});
        }

        bool load_sprite(const std::string &id, const std::string &path)
        {
            const auto asset_path = resolve_asset_path(path);
            if (id.empty() || !asset_path || sprites.find(id) != sprites.end())
            {
                return false;
            }
            Bitmap *sprite = sl::load_bitmap(asset_path->string());
            if (!sprite)
            {
                return false;
            }
            sprites.emplace(id, sprite);
            return true;
        }

        bool load_sound(const std::string &id, const std::string &path)
        {
            const auto asset_path = resolve_asset_path(path);
            if (id.empty() || !asset_path || sounds.find(id) != sounds.end())
            {
                return false;
            }
            Sample *sound = sl::load_sample(asset_path->string());
            if (!sound)
            {
                return false;
            }
            sounds.emplace(id, sound);
            return true;
        }

        std::uint64_t play_sound(const std::string &id, int volume, int pan, int frequency, int loops) const
        {
            const auto sound = sounds.find(id);
            return sound == sounds.end() ? 0 : sl::play_sample(sound->second, volume, pan, frequency, loops);
        }

        bool unload_sound(const std::string &id)
        {
            const auto sound = sounds.find(id);
            if (sound == sounds.end())
            {
                return false;
            }
            sl::destroy_sample(sound->second);
            sounds.erase(sound);
            return true;
        }

        bool load_music(const std::string &id, const std::string &path)
        {
            const auto asset_path = resolve_asset_path(path);
            if (id.empty() || !asset_path || music.find(id) != music.end())
            {
                return false;
            }
            Stream *stream = sl::load_stream(asset_path->string());
            if (!stream)
            {
                return false;
            }
            music.emplace(id, stream);
            return true;
        }

        bool play_music(const std::string &id, int loops) const
        {
            const auto stream = music.find(id);
            if (stream == music.end())
            {
                return false;
            }
            sl::play_stream(stream->second, loops);
            return true;
        }

        bool unload_music(const std::string &id)
        {
            const auto stream = music.find(id);
            if (stream == music.end())
            {
                return false;
            }
            sl::destroy_stream(stream->second);
            music.erase(stream);
            return true;
        }

        bool add_sprite(const std::string &id, float x, float y, float width = 0.0f, float height = 0.0f)
        {
            if (sprites.find(id) == sprites.end() && render_targets.find(id) == render_targets.end())
            {
                return false;
            }
            commands.push_back({width == 0.0f && height == 0.0f ? DrawingType::sprite : DrawingType::sprite_stretched,
                                x,
                                y,
                                width,
                                height,
                                0.0f,
                                0.0f,
                                {},
                                id});
            return true;
        }

        bool add_rotated_sprite(const std::string &id, float center_x, float center_y, float angle_degrees, float width = 0.0f, float height = 0.0f)
        {
            if (sprites.find(id) == sprites.end() && render_targets.find(id) == render_targets.end())
            {
                return false;
            }
            commands.push_back({width == 0.0f && height == 0.0f ? DrawingType::sprite_rotated : DrawingType::sprite_rotated_stretched,
                                center_x,
                                center_y,
                                width,
                                height,
                                angle_degrees,
                                0.0f,
                                {},
                                id});
            return true;
        }

        bool create_render_target_handle(const std::string &id, int width, int height)
        {
            if (id.empty() || width <= 0 || height <= 0 || render_targets.find(id) != render_targets.end())
            {
                return false;
            }
            Bitmap *target = sl::create_render_target(width, height);
            if (!target)
                target = sl::create_bitmap(width, height);
            if (!target)
                return false;
            render_targets.emplace(id, target);
            return true;
        }

        bool destroy_render_target_handle(const std::string &id)
        {
            const auto iterator = render_targets.find(id);
            if (iterator == render_targets.end())
                return false;
            sl::destroy_bitmap(iterator->second);
            render_targets.erase(iterator);
            return true;
        }

        Bitmap *find_bitmap(const std::string &id) const
        {
            auto sp = sprites.find(id);
            if (sp != sprites.end())
                return sp->second;
            auto rt = render_targets.find(id);
            if (rt != render_targets.end())
                return rt->second;
            return nullptr;
        }

        void clear_render_targets()
        {
            for (const auto &[id, target] : render_targets)
            {
                sl::destroy_bitmap(target);
            }
            render_targets.clear();
        }

        void clear_compute()
        {
            for (const auto &[id, shader] : shaders)
                delete shader;
            shaders.clear();
            for (const auto &[id, buffer] : storage_buffers)
                delete buffer;
            storage_buffers.clear();
        }

        bool unload_sprite(const std::string &id)
        {
            const auto sprite = sprites.find(id);
            if (sprite == sprites.end())
            {
                return false;
            }
            commands.erase(
                std::remove_if(commands.begin(), commands.end(), [&id](const DrawingCommand &command)
                               { return (command.type == DrawingType::sprite || command.type == DrawingType::sprite_stretched ||
                                         command.type == DrawingType::sprite_rotated || command.type == DrawingType::sprite_rotated_stretched) &&
                                        command.sprite_id == id; }),
                commands.end());
            sl::destroy_bitmap(sprite->second);
            sprites.erase(sprite);
            return true;
        }

        void clear_sprites()
        {
            commands.erase(
                std::remove_if(commands.begin(), commands.end(), [](const DrawingCommand &command)
                               { return command.type == DrawingType::sprite || command.type == DrawingType::sprite_stretched ||
                                        command.type == DrawingType::sprite_rotated || command.type == DrawingType::sprite_rotated_stretched; }),
                commands.end());
            for (const auto &sprite : sprites)
            {
                sl::destroy_bitmap(sprite.second);
            }
            sprites.clear();
        }

        void clear_audio()
        {
            for (const auto &sound : sounds)
            {
                sl::destroy_sample(sound.second);
            }
            sounds.clear();
            for (const auto &stream : music)
            {
                sl::destroy_stream(stream.second);
            }
            music.clear();
        }

        std::uint64_t new_physics_handle()
        {
            return next_physics_handle++;
        }

        void clear_physics()
        {
            for (const auto &[handle, body] : physics_bodies)
            {
                destroy_physics_body(body);
            }
            physics_bodies.clear();
            physics_body_worlds.clear();
            for (const auto &[handle, world] : physics_worlds)
            {
                destroy_physics_world(world);
            }
            physics_worlds.clear();
            next_physics_handle = 1;
        }

        PhysicsWorld *physics_world(std::uint64_t handle) const
        {
            const auto iterator = physics_worlds.find(handle);
            return iterator == physics_worlds.end() ? nullptr : iterator->second;
        }

        PhysicsBody *physics_body(std::uint64_t handle) const
        {
            const auto iterator = physics_bodies.find(handle);
            return iterator == physics_bodies.end() ? nullptr : iterator->second;
        }

        void destroy_physics_world_handle(std::uint64_t handle)
        {
            const auto world = physics_worlds.find(handle);
            if (world == physics_worlds.end())
            {
                return;
            }
            for (auto iterator = physics_body_worlds.begin(); iterator != physics_body_worlds.end();)
            {
                if (iterator->second == handle)
                {
                    const auto body = physics_bodies.find(iterator->first);
                    if (body != physics_bodies.end())
                    {
                        destroy_physics_body(body->second);
                        physics_bodies.erase(body);
                    }
                    iterator = physics_body_worlds.erase(iterator);
                }
                else
                {
                    ++iterator;
                }
            }
            destroy_physics_world(world->second);
            physics_worlds.erase(world);
        }
    };

    void LuaCanvas::reset()
    {
        if (implementation_)
        {
            implementation_->clear_sprites();
            implementation_->clear_render_targets();
            implementation_->clear_audio();
            implementation_->clear_physics();
            implementation_->clear_compute();
            implementation_->commands.clear();
        }
    }

    LuaCanvas::LuaCanvas()
        : implementation_(new Implementation)
    {
    }

    LuaCanvas::~LuaCanvas()
    {
        reset();
        delete implementation_;
    }

    void LuaCanvas::initialise(LuaRuntime::OutputHandler output_handler)
    {
        if (implementation_->runtime.is_initialised())
        {
            return;
        }

        implementation_->runtime.initialise(std::move(output_handler));
        sol::table app = implementation_->runtime.state()["app"];
        app.set_function(
            "clear_screen",
            [this](int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->background = make_colour(red, green, blue, alpha);
            });
        app.set_function("clear_drawings", [this]()
                         { implementation_->commands.clear(); });
        app.set_function("load_sprite", [this](const std::string &id, const std::string &path)
                         { return implementation_->load_sprite(id, path); });
        app.set_function("sprite", [this](const std::string &id, float x, float y)
                         { return implementation_->add_sprite(id, x, y); });
        app.set_function("sprite_stretched", [this](const std::string &id, float x, float y, float width, float height)
                         { return implementation_->add_sprite(id, x, y, width, height); });
        app.set_function("sprite_rotated", [this](const std::string &id, float center_x, float center_y, float angle_degrees)
                         { return implementation_->add_rotated_sprite(id, center_x, center_y, angle_degrees); });
        app.set_function("sprite_rotated_stretched", [this](const std::string &id, float center_x, float center_y, float angle_degrees, float width, float height)
                         { return implementation_->add_rotated_sprite(id, center_x, center_y, angle_degrees, width, height); });
        app.set_function("unload_sprite", [this](const std::string &id)
                         { return implementation_->unload_sprite(id); });
        app.set_function("clear_sprites", [this]()
                         { implementation_->clear_sprites(); });
        app.set_function("load_sound", [this](const std::string &id, const std::string &path)
                         { return implementation_->load_sound(id, path); });
        app.set_function("play_sound", [this](const std::string &id, sol::optional<int> volume,
                                              sol::optional<int> pan, sol::optional<int> frequency,
                                              sol::optional<int> loops)
                         { return implementation_->play_sound(id, volume.value_or(255), pan.value_or(128),
                                                              frequency.value_or(1000), loops.value_or(0)); });
        app.set_function("stop_sound", [](std::uint64_t voice)
                         { sl::stop_voice(voice); });
        app.set_function("unload_sound", [this](const std::string &id)
                         { return implementation_->unload_sound(id); });
        app.set_function("load_music", [this](const std::string &id, const std::string &path)
                         { return implementation_->load_music(id, path); });
        app.set_function("play_music", [this](const std::string &id, sol::optional<int> loops)
                         { return implementation_->play_music(id, loops.value_or(-1)); });
        app.set_function("stop_music", []()
                         { sl::stop_stream(); });
        app.set_function("pause_music", []()
                         { sl::pause_stream(); });
        app.set_function("resume_music", []()
                         { sl::resume_stream(); });
        app.set_function("set_music_volume", [](int volume)
                         { sl::music_set_volume(volume); });
        app.set_function("unload_music", [this](const std::string &id)
                         { return implementation_->unload_music(id); });
        app.set_function("create_render_target", [this](const std::string &id, int width, int height)
                         { return implementation_->create_render_target_handle(id, width, height); });
        app.set_function("destroy_render_target", [this](const std::string &id)
                         { return implementation_->destroy_render_target_handle(id); });

        sol::table display = implementation_->runtime.state().create_named_table("display");
        display.set_function("width", []()
                             { return sl::screen_width(); });
        display.set_function("height", []()
                             { return sl::screen_height(); });
        display.set_function("virtual_width", []()
                             { return sl::virtual_screen_width(); });
        display.set_function("virtual_height", []()
                             { return sl::virtual_screen_height(); });
        display.set_function("set_title", [](const std::string &title)
                             { sl::set_window_title(title.c_str()); });

        sol::table system = implementation_->runtime.state().create_named_table("system");
        system.set_function("time_ms", []()
                            { return sl::time_ms(); });
        system.set_function("get_fps", []()
                            { return sl::get_fps(); });
        system.set_function("set_fps", [](int fps)
                            { sl::set_fps(fps); });
        system.set_function("get_frame_time", []()
                            { return sl::get_frame_time(); });

        sol::table fx = implementation_->runtime.state().create_named_table("fx");
        fx.set_function("shake", [this](float amplitude, float duration)
                        { implementation_->screen_shake.trigger(amplitude, duration); });
        fx.set_function("update_shake", [this](float delta)
                        { implementation_->screen_shake.update(delta); });
        fx.set_function("shake_active", [this]()
                        { return implementation_->screen_shake.active(); });
        fx.set_function("clear_shake", [this]()
                        { implementation_->screen_shake.clear(); });

        fx.set_function("bloom_init", [this]()
                        { return implementation_->bloom.initialise(); });
        fx.set_function("bloom_config", [this](sol::optional<float> threshold, sol::optional<float> intensity, sol::optional<float> radius)
                        {
                            if (threshold) implementation_->bloom.set_threshold(*threshold);
                            if (intensity) implementation_->bloom.set_intensity(*intensity);
                            if (radius) implementation_->bloom.set_radius(*radius); });
        fx.set_function("apply_bloom", [this](const std::string &target_id)
                        {
                            Bitmap *bmp = implementation_->find_bitmap(target_id);
                            if (bmp && implementation_->bloom.is_valid())
                            {
                                implementation_->bloom.apply(bmp);
                                return true;
                            }
                            return false; });

        fx.set_function("vignette_init", [this]()
                        { return implementation_->vignette.initialise(); });
        fx.set_function("vignette_config", [this](sol::optional<float> radius, sol::optional<float> softness, sol::optional<float> intensity)
                        {
                            if (radius) implementation_->vignette.set_radius(*radius);
                            if (softness) implementation_->vignette.set_softness(*softness);
                            if (intensity) implementation_->vignette.set_intensity(*intensity); });
        fx.set_function("apply_vignette", [this](const std::string &target_id)
                        {
                            Bitmap *bmp = implementation_->find_bitmap(target_id);
                            if (bmp && implementation_->vignette.is_valid())
                            {
                                implementation_->vignette.apply(bmp);
                                return true;
                            }
                            return false; });

        sol::table compute = implementation_->runtime.state().create_named_table("compute");
        compute.set_function("load_shader", [this](const std::string &id, const std::string &source)
                             {
                                 if (id.empty() || implementation_->shaders.find(id) != implementation_->shaders.end())
                                     return false;
                                 auto *shader = new Shader();
                                 if (!shader->load_compute(source))
                                 {
                                     delete shader;
                                     return false;
                                 }
                                 implementation_->shaders.emplace(id, shader);
                                 return true; });
        compute.set_function("destroy_shader", [this](const std::string &id)
                             {
                                 auto it = implementation_->shaders.find(id);
                                 if (it == implementation_->shaders.end()) return false;
                                 delete it->second;
                                 implementation_->shaders.erase(it);
                                 return true; });
        compute.set_function("create_buffer", [this](const std::string &id, std::size_t size_bytes)
                             {
                                 if (id.empty() || implementation_->storage_buffers.find(id) != implementation_->storage_buffers.end())
                                     return false;
                                 auto *buffer = new StorageBuffer(size_bytes);
                                 if (!buffer->is_valid())
                                 {
                                     delete buffer;
                                     return false;
                                 }
                                 implementation_->storage_buffers.emplace(id, buffer);
                                 return true; });
        compute.set_function("destroy_buffer", [this](const std::string &id)
                             {
                                 auto it = implementation_->storage_buffers.find(id);
                                 if (it == implementation_->storage_buffers.end()) return false;
                                 delete it->second;
                                 implementation_->storage_buffers.erase(it);
                                 return true; });
        compute.set_function("upload_floats", [this](const std::string &buffer_id, sol::table float_table)
                             {
                                 auto it = implementation_->storage_buffers.find(buffer_id);
                                 if (it == implementation_->storage_buffers.end()) return false;
                                 std::vector<float> values;
                                 for (const auto &kv : float_table)
                                 {
                                     values.push_back(kv.second.as<float>());
                                 }
                                 return it->second->upload(values); });
        compute.set_function("readback_floats", [this](sol::this_state state, const std::string &buffer_id, std::size_t count)
                             {
                                 sol::state_view lua(state);
                                 sol::table result = lua.create_table();
                                 auto it = implementation_->storage_buffers.find(buffer_id);
                                 if (it == implementation_->storage_buffers.end() || count == 0) return result;
                                 std::vector<float> values(count, 0.0f);
                                 if (it->second->readback(values))
                                 {
                                     for (std::size_t i = 0; i < count; ++i)
                                     {
                                         result[i + 1] = values[i];
                                     }
                                 }
                                 return result; });
        compute.set_function("bind_buffer", [this](const std::string &buffer_id, unsigned int binding_slot)
                             {
                                 auto it = implementation_->storage_buffers.find(buffer_id);
                                 return it != implementation_->storage_buffers.end() && it->second->bind(binding_slot); });
        compute.set_function("set_uniform_float", [this](const std::string &shader_id, const std::string &name, float value)
                             {
                                 auto it = implementation_->shaders.find(shader_id);
                                 return it != implementation_->shaders.end() && it->second->set_uniform(name.c_str(), value); });
        compute.set_function("dispatch_for", [this](const std::string &shader_id, unsigned int totalX,
                                                    sol::optional<unsigned int> totalY, sol::optional<unsigned int> totalZ,
                                                    sol::optional<unsigned int> localX, sol::optional<unsigned int> localY, sol::optional<unsigned int> localZ)
                             {
                                 auto it = implementation_->shaders.find(shader_id);
                                 return it != implementation_->shaders.end() &&
                                     sl::dispatch_compute_for(*it->second, totalX, totalY.value_or(1), totalZ.value_or(1),
                                                              localX.value_or(16), localY.value_or(16), localZ.value_or(1)); });
        compute.set_function("barrier", []()
                             { sl::compute_barrier(); });

        sol::table physics = implementation_->runtime.state().create_named_table("physics");
        physics.set_function("create_world", [this](float gravity_x, float gravity_y)
                             {
                                 PhysicsWorld *world = create_physics_world({gravity_x, gravity_y});
                                 if (!world)
                                 {
                                     return std::uint64_t{0};
                                 }
                                 const std::uint64_t handle = implementation_->new_physics_handle();
                                 implementation_->physics_worlds.emplace(handle, world);
                                 return handle; });
        physics.set_function("destroy_world", [this](std::uint64_t handle)
                             { implementation_->destroy_physics_world_handle(handle); });
        physics.set_function("create_body", [this](std::uint64_t world_handle, const std::string &type, float x, float y)
                             {
                                 PhysicsWorld *world = implementation_->physics_world(world_handle);
                                 if (!world)
                                 {
                                     return std::uint64_t{0};
                                 }
                                 BodyType body_type;
                                 if (type == "static") body_type = BodyType::static_body;
                                 else if (type == "kinematic") body_type = BodyType::kinematic_body;
                                 else if (type == "dynamic") body_type = BodyType::dynamic_body;
                                 else return std::uint64_t{0};
                                 PhysicsBody *body = create_physics_body(world, body_type, {x, y});
                                 if (!body)
                                 {
                                     return std::uint64_t{0};
                                 }
                                 const std::uint64_t handle = implementation_->new_physics_handle();
                                 implementation_->physics_bodies.emplace(handle, body);
                                 implementation_->physics_body_worlds.emplace(handle, world_handle);
                                 return handle; });
        physics.set_function("destroy_body", [this](std::uint64_t handle)
                             {
                                 const auto body = implementation_->physics_bodies.find(handle);
                                 if (body == implementation_->physics_bodies.end()) return;
                                 destroy_physics_body(body->second);
                                 implementation_->physics_bodies.erase(body);
                                 implementation_->physics_body_worlds.erase(handle); });
        physics.set_function("add_box", [this](std::uint64_t handle, float width, float height,
                                               sol::optional<float> density, sol::optional<float> friction,
                                               sol::optional<float> restitution)
                             {
                                 PhysicsBody *body = implementation_->physics_body(handle);
                                 return body && add_box_fixture(body, width, height, density.value_or(1.0f),
                                                                friction.value_or(0.3f), restitution.value_or(0.0f)); });
        physics.set_function("add_circle", [this](std::uint64_t handle, float radius,
                                                  sol::optional<float> density, sol::optional<float> friction,
                                                  sol::optional<float> restitution)
                             {
                                 PhysicsBody *body = implementation_->physics_body(handle);
                                 return body && add_circle_fixture(body, radius, density.value_or(1.0f),
                                                                   friction.value_or(0.3f), restitution.value_or(0.0f)); });
        physics.set_function("add_polygon", [this](std::uint64_t handle, sol::table vertices,
                                                   sol::optional<float> density, sol::optional<float> friction,
                                                   sol::optional<float> restitution)
                             {
                                 PhysicsBody *body = implementation_->physics_body(handle);
                                 if (!body) return false;
                                 std::vector<Vec2> points;
                                 for (const auto &entry : vertices)
                                 {
                                     sol::table point = entry.second.as<sol::table>();
                                     points.push_back({point["x"].get_or(0.0f), point["y"].get_or(0.0f)});
                                 }
                                 return add_polygon_fixture(body, points, density.value_or(1.0f),
                                                            friction.value_or(0.3f), restitution.value_or(0.0f)); });
        physics.set_function("step", [this](std::uint64_t handle, float time_step,
                                            sol::optional<int> velocity_iterations,
                                            sol::optional<int> position_iterations)
                             {
                                 PhysicsWorld *world = implementation_->physics_world(handle);
                                 if (!world) return false;
                                 step_physics_world(world, time_step, velocity_iterations.value_or(8),
                                                    position_iterations.value_or(3));
                                 return true; });
        physics.set_function("position", [this](sol::this_state state, std::uint64_t handle)
                             {
                                 sol::state_view lua(state);
                                 sol::table result = lua.create_table();
                                 const Vec2 position = physics_body_position(implementation_->physics_body(handle));
                                 result["x"] = position.x;
                                 result["y"] = position.y;
                                 return result; });
        physics.set_function("velocity", [this](sol::this_state state, std::uint64_t handle)
                             {
                                 sol::state_view lua(state);
                                 sol::table result = lua.create_table();
                                 const Vec2 velocity = physics_body_velocity(implementation_->physics_body(handle));
                                 result["x"] = velocity.x;
                                 result["y"] = velocity.y;
                                 return result; });
        physics.set_function("set_velocity", [this](std::uint64_t handle, float x, float y)
                             {
                                 PhysicsBody *body = implementation_->physics_body(handle);
                                 if (!body) return false;
                                 set_physics_body_velocity(body, {x, y});
                                 return true; });
        physics.set_function("contacts", [this](sol::this_state state, std::uint64_t handle)
                             {
                                 sol::state_view lua(state);
                                 sol::table result = lua.create_table();
                                 PhysicsWorld *world = implementation_->physics_world(handle);
                                 if (!world) return result;
                                 int index = 1;
                                 for (const PhysicsContact &contact : poll_physics_contacts(world))
                                 {
                                     sol::table event = lua.create_table();
                                     event["type"] = contact.type == ContactType::begin ? "begin" : "end";
                                     event["point"] = lua.create_table_with("x", contact.point.x, "y", contact.point.y);
                                     event["normal"] = lua.create_table_with("x", contact.normal.x, "y", contact.normal.y);
                                     event["body_a"] = std::uint64_t{0};
                                     event["body_b"] = std::uint64_t{0};
                                     for (const auto &[body_handle, body] : implementation_->physics_bodies)
                                     {
                                         if (body == contact.body_a) event["body_a"] = body_handle;
                                         if (body == contact.body_b) event["body_b"] = body_handle;
                                     }
                                     result[index++] = event;
                                 }
                                 return result; });
        app.set_function(
            "pixel",
            [this](float x, float y, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::pixel, x, y, 0.0f, 0.0f, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "line",
            [this](float x1, float y1, float x2, float y2, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::line, x1, y1, x2, y2, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "circle",
            [this](float x, float y, float radius, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::circle, x, y, radius, 0.0f, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "circlefill",
            [this](float x, float y, float radius, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::circlefill, x, y, radius, 0.0f, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "rect",
            [this](float left, float top, float right, float bottom, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::rect, left, top, right, bottom, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "rectfill",
            [this](float left, float top, float right, float bottom, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::rectfill, left, top, right, bottom, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "ellipse",
            [this](float x, float y, float radius_x, float radius_y, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::ellipse, x, y, radius_x, radius_y, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "ellipsefill",
            [this](float x, float y, float radius_x, float radius_y, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::ellipsefill, x, y, radius_x, radius_y, 0.0f, 0.0f, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "triangle",
            [this](float x1, float y1, float x2, float y2, float x3, float y3, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::triangle, x1, y1, x2, y2, x3, y3, make_colour(red, green, blue, alpha));
            });
        app.set_function(
            "trianglefill",
            [this](float x1, float y1, float x2, float y2, float x3, float y3, int red, int green, int blue, sol::optional<int> alpha)
            {
                implementation_->add_drawing(DrawingType::trianglefill, x1, y1, x2, y2, x3, y3, make_colour(red, green, blue, alpha));
            });
    }

    bool LuaCanvas::is_initialised() const
    {
        return implementation_->runtime.is_initialised();
    }

    LuaScriptResult LuaCanvas::run_text(const std::string &script)
    {
        if (!is_initialised())
        {
            initialise();
        }
        return implementation_->runtime.execute(script);
    }

    LuaScriptResult LuaCanvas::run_file(const std::string &path)
    {
        std::ifstream file(path);
        if (!file)
        {
            return {false, "Unable to open Lua script: " + path};
        }
        std::ostringstream script;
        script << file.rdbuf();
        return run_text(script.str());
    }

    LuaScriptResult LuaCanvas::dispatch_keypress(const std::string &key)
    {
        if (!is_initialised())
        {
            initialise();
        }
        return implementation_->runtime.call("on_keypress", key);
    }

    LuaRuntime &LuaCanvas::runtime()
    {
        if (!is_initialised())
        {
            initialise();
        }
        return implementation_->runtime;
    }

    void LuaCanvas::set_asset_root(const std::string &path)
    {
        implementation_->asset_root = path;
    }

    void LuaCanvas::render(Bitmap *target) const
    {
        if (!target)
        {
            return;
        }
        clear_to_colour(target, implementation_->background);
        for (const DrawingCommand &command : implementation_->commands)
        {
            switch (command.type)
            {
            case DrawingType::pixel:
                putpixel(target, static_cast<int>(command.x1), static_cast<int>(command.y1), command.colour);
                break;
            case DrawingType::line:
                line(target, command.x1, command.y1, command.x2, command.y2, command.colour);
                break;
            case DrawingType::circle:
                circle(target, command.x1, command.y1, command.x2, command.colour);
                break;
            case DrawingType::circlefill:
                circlefill(target, command.x1, command.y1, command.x2, command.colour);
                break;
            case DrawingType::rect:
                rect(target, command.x1, command.y1, command.x2, command.y2, command.colour);
                break;
            case DrawingType::rectfill:
                rectfill(target, command.x1, command.y1, command.x2, command.y2, command.colour);
                break;
            case DrawingType::ellipse:
                ellipse(target, command.x1, command.y1, command.x2, command.y2, command.colour);
                break;
            case DrawingType::ellipsefill:
                ellipsefill(target, command.x1, command.y1, command.x2, command.y2, command.colour);
                break;
            case DrawingType::triangle:
                triangle(target, command.x1, command.y1, command.x2, command.y2, command.x3, command.y3, command.colour);
                break;
            case DrawingType::trianglefill:
                trianglefill(target, command.x1, command.y1, command.x2, command.y2, command.x3, command.y3, command.colour);
                break;
            case DrawingType::sprite:
            {
                Bitmap *sprite = implementation_->find_bitmap(command.sprite_id);
                if (sprite)
                    draw_sprite(sprite, command.x1, command.y1);
                break;
            }
            case DrawingType::sprite_stretched:
            {
                Bitmap *sprite = implementation_->find_bitmap(command.sprite_id);
                if (sprite)
                    draw_sprite_stretched(sprite, command.x1, command.y1, static_cast<int>(command.x2), static_cast<int>(command.y2));
                break;
            }
            case DrawingType::sprite_rotated:
            {
                Bitmap *sprite = implementation_->find_bitmap(command.sprite_id);
                if (sprite)
                    draw_sprite_rotated(sprite, command.x1, command.y1, command.x3);
                break;
            }
            case DrawingType::sprite_rotated_stretched:
            {
                Bitmap *sprite = implementation_->find_bitmap(command.sprite_id);
                if (sprite)
                    draw_sprite_rotated_stretched(sprite, command.x1, command.y1, command.x3, static_cast<int>(command.x2), static_cast<int>(command.y2));
                break;
            }
            }
        }
    }

    void LuaCanvas::clear()
    {
        implementation_->background = {0, 0, 0};
        implementation_->commands.clear();
    }
}