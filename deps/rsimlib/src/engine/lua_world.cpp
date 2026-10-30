/** @file
 * @brief Lua bindings for tile maps, fluid simulations and voxel worlds.
 */

#include "lua_world.h"

#include "atmosphere.h"
#include "clutter.h"
#include "props3d.h"
#include "draw.h"
#include "fluid.h"
#include "fluid3d.h"
#include "surface3d.h"
#include "system.h"
#include "terrain.h"
#include "tilemap.h"
#include "voxel.h"

#include <sol/sol.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>

namespace sl
{
    namespace
    {
        constexpr int max_fluid_2d_size = 1024;
        constexpr int max_fluid_3d_size = 128;
        constexpr long long max_fill_blocks = 4'000'000;
        constexpr int max_view_size = 4096;

        struct ElementName
        {
            const char *name;
            FluidElement element;
            Colour colour;
        };

        constexpr ElementName element_names[] = {
            {"empty", FluidElement::empty, {0, 0, 0, 0}},
            {"solid", FluidElement::solid, {110, 110, 120, 255}},
            {"wood", FluidElement::wood, {120, 80, 40, 255}},
            {"sand", FluidElement::sand, {220, 190, 110, 255}},
            {"water", FluidElement::water, {40, 100, 230, 255}},
            {"oil", FluidElement::oil, {90, 60, 30, 255}},
            {"acid", FluidElement::acid, {120, 230, 60, 255}},
            {"lava", FluidElement::lava, {240, 90, 20, 255}},
            {"gunpowder", FluidElement::gunpowder, {70, 70, 70, 255}},
            {"plant", FluidElement::plant, {50, 170, 60, 255}},
            {"pump", FluidElement::pump, {200, 60, 200, 255}},
            {"fire", FluidElement::fire, {255, 160, 30, 255}},
            {"smoke", FluidElement::smoke, {110, 110, 110, 200}},
            {"steam", FluidElement::steam, {210, 220, 235, 180}},
        };

        std::optional<FluidElement> element_from_name(const std::string &name)
        {
            for (const ElementName &entry : element_names)
            {
                if (name == entry.name)
                    return entry.element;
            }
            return std::nullopt;
        }

        const ElementName &element_entry(FluidElement element)
        {
            return element_names[std::min<std::size_t>(static_cast<std::size_t>(element), std::size(element_names) - 1)];
        }

        sol::table cell_table(sol::state_view lua, const FluidCell &cell)
        {
            sol::table result = lua.create_table();
            result["element"] = element_entry(cell.type).name;
            result["mass"] = cell.mass;
            result["temp"] = cell.temp;
            return result;
        }

        Colour colour_from_table(sol::optional<sol::table> table, Colour fallback)
        {
            if (!table)
                return fallback;
            auto channel = [&](int index, Uint8 value)
            { return static_cast<Uint8>(std::clamp(table->get_or(index, static_cast<int>(value)), 0, 255)); };
            return {channel(1, fallback.red), channel(2, fallback.green), channel(3, fallback.blue), channel(4, fallback.alpha)};
        }

        template <typename T>
        T *find_in(const std::unordered_map<std::string, T *> &map, const std::string &id)
        {
            const auto it = map.find(id);
            return it == map.end() ? nullptr : it->second;
        }
    }

    struct LuaWorld::Implementation
    {
        struct TileView
        {
            float x = 0.0f;
            float y = 0.0f;
            int width = 0;
            int height = 0;
        };
        struct FluidView
        {
            float x = 0.0f;
            float y = 0.0f;
            float scale = 2.0f;
        };
        struct VoxelView
        {
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;
            Camera camera;
            Colour sky{18, 22, 36, 255};
            Surface3D *surface = nullptr;
        };

        struct TerrainEntry
        {
            std::unique_ptr<Terrain> terrain;
            std::shared_ptr<GridSource> grid;
            // Layer textures are uploaded by the first draw, when a renderer is certain to exist.
            bool textures_pending = false;
            TerrainTextureOptions texture_options;
            int x = 0;
            int y = 0;
            int width = 0;
            int height = 0;
            dvec3 eye{0.0, 100.0, 0.0};
            dvec3 forward{0.0, 0.0, -1.0};
            dvec3 up{0.0, 1.0, 0.0};
            double fov = 60.0;
            double near_plane = 0.5;
            double far_plane = 30000.0;
            glm::vec3 light_direction{-0.5f, -0.8f, -0.3f};
            float ambient = 0.3f;
            Colour sky{150, 185, 230, 255};
            Surface3D *surface = nullptr;
            // Declared after `terrain` so it is destroyed first.
            std::unique_ptr<ClutterLayer> clutter;
            std::vector<ClutterSpecies> pending_species;
            bool has_environment = false;
            Environment3D environment;
        };

        std::unordered_map<std::string, TerrainEntry> terrains;
        std::unordered_map<std::string, TileMap *> tilemaps;
        std::unordered_map<std::string, TileView> tile_views;
        std::unordered_map<std::string, FluidSimulation *> fluids;
        std::unordered_map<std::string, FluidView> fluid_views;
        std::unordered_map<std::string, FluidSimulation3D *> fluids_3d;
        struct WorldAtlas
        {
            std::string sprite;
            std::string normal_sprite;
            int columns = 16;
            int rows = 16;
        };
        std::unordered_map<std::string, std::unique_ptr<VoxelWorld>> worlds;
        std::unordered_map<std::string, WorldAtlas> atlases;
        BitmapResolver bitmaps;
        std::unordered_map<std::string, VoxelView> voxel_views;
        FluidVoxelBlocks fluid_blocks;

        VoxelWorld *world(const std::string &id) const
        {
            const auto it = worlds.find(id);
            return it == worlds.end() ? nullptr : it->second.get();
        }

        void destroy_view(const std::string &id)
        {
            const auto it = voxel_views.find(id);
            if (it != voxel_views.end())
            {
                if (it->second.surface)
                    destroy_surface_3d(it->second.surface);
                voxel_views.erase(it);
            }
        }

        TerrainEntry *terrain(const std::string &id)
        {
            const auto it = terrains.find(id);
            return it == terrains.end() ? nullptr : &it->second;
        }

        void clear()
        {
            for (auto &entry : terrains)
            {
                if (entry.second.surface)
                    destroy_surface_3d(entry.second.surface);
            }
            terrains.clear();
            for (auto &entry : tilemaps)
                destroy_tilemap(entry.second);
            for (auto &entry : fluids)
                destroy_fluid_simulation(entry.second);
            for (auto &entry : fluids_3d)
                destroy_fluid_simulation_3d(entry.second);
            for (auto &entry : voxel_views)
            {
                if (entry.second.surface)
                    destroy_surface_3d(entry.second.surface);
            }
            tilemaps.clear();
            tile_views.clear();
            fluids.clear();
            fluid_views.clear();
            fluids_3d.clear();
            voxel_views.clear();
            worlds.clear();
            atlases.clear();
        }

        void bind_tilemap(sol::table table, const PathResolver &resolver, const DrawRequest &request);
        void bind_fluid(sol::table table, const DrawRequest &request);
        void bind_fluid_3d(sol::table table);
        void bind_voxel(sol::table table, const DrawRequest &request);
        void bind_terrain(sol::table table, const DrawRequest &request);
    };

    void LuaWorld::Implementation::bind_tilemap(sol::table table, const PathResolver &resolver, const DrawRequest &request)
    {
        table.set_function("load", [this, resolver](const std::string &id, const std::string &path)
                           {
            const auto resolved = resolver(path);
            if (id.empty() || !resolved || tilemaps.count(id))
                return false;
            TileMap *map = load_tiled_map(resolved->string());
            if (!map)
                return false;
            tilemaps.emplace(id, map);
            return true; });
        table.set_function("unload", [this](const std::string &id)
                           {
            const auto it = tilemaps.find(id);
            if (it == tilemaps.end())
                return false;
            destroy_tilemap(it->second);
            tilemaps.erase(it);
            tile_views.erase(id);
            return true; });
        table.set_function("info", [this](sol::this_state state, const std::string &id) -> sol::object
                           {
            sol::state_view lua(state);
            const TileMap *map = find_in(tilemaps, id);
            if (!map)
                return sol::nil;
            sol::table info = lua.create_table();
            info["width"] = map->width;
            info["height"] = map->height;
            info["tile_width"] = map->tile_width;
            info["tile_height"] = map->tile_height;
            info["layers"] = static_cast<int>(map->layers.size());
            info["isometric"] = map->orientation == TileMapOrientation::isometric;
            return info; });
        table.set_function("get", [this](const std::string &id, int layer, int column, int row)
                           { return static_cast<lua_Integer>(tilemap_get(find_in(tilemaps, id), layer, column, row)); });
        table.set_function("set", [this](const std::string &id, int layer, int column, int row, lua_Integer cell)
                           {
            TileMap *map = find_in(tilemaps, id);
            return map && cell >= 0 && tilemap_set(map, layer, column, row, static_cast<std::uint32_t>(cell)); });
        table.set_function("is_solid", [this](const std::string &id, int column, int row, sol::optional<int> layer)
                           { return tilemap_is_solid(find_in(tilemaps, id), column, row, layer.value_or(-1)); });
        table.set_function("rect_blocked", [this](const std::string &id, float x, float y, float width, float height,
                                                  sol::optional<int> layer)
                           { return tilemap_rect_blocked(find_in(tilemaps, id), x, y, width, height, layer.value_or(-1)); });
        table.set_function("set_solid", [this](const std::string &id, lua_Integer gid, bool solid)
                           {
            TileMap *map = find_in(tilemaps, id);
            if (!map || gid <= 0)
                return false;
            tilemap_set_solid(map, static_cast<std::uint32_t>(gid), solid);
            return true; });
        table.set_function("property", [this](const std::string &id, lua_Integer gid, const std::string &key)
                           {
            const TileMap *map = find_in(tilemaps, id);
            return map && gid > 0 ? tilemap_tile_property(map, static_cast<std::uint32_t>(gid), key) : std::string{}; });
        table.set_function("world_to_tile", [this](const std::string &id, float x, float y)
                           {
            int column = 0, row = 0;
            if (const TileMap *map = find_in(tilemaps, id))
                tilemap_world_to_tile(map, x, y, column, row);
            return std::make_tuple(column, row); });
        table.set_function("tile_to_world", [this](const std::string &id, int column, int row)
                           {
            float x = 0.0f, y = 0.0f;
            if (const TileMap *map = find_in(tilemaps, id))
                tilemap_tile_to_world(map, column, row, x, y);
            return std::make_tuple(x, y); });
        table.set_function("update", [this](const std::string &id, float dt)
                           {
            TileMap *map = find_in(tilemaps, id);
            if (map)
                update_tilemap(map, dt);
            return map != nullptr; });
        table.set_function("find_object", [this](sol::this_state state, const std::string &id, const std::string &name) -> sol::object
                           {
            sol::state_view lua(state);
            const TileObject *object = tilemap_find_object(find_in(tilemaps, id), name);
            if (!object)
                return sol::nil;
            sol::table result = lua.create_table();
            result["name"] = object->name;
            result["type"] = object->type;
            result["x"] = object->x;
            result["y"] = object->y;
            result["width"] = object->width;
            result["height"] = object->height;
            sol::table properties = lua.create_table();
            for (const auto &[key, value] : object->properties)
                properties[key] = value;
            result["properties"] = properties;
            return result; });
        table.set_function("draw", [this, request](const std::string &id, float camera_x, float camera_y,
                                                   sol::optional<int> width, sol::optional<int> height)
                           {
            if (!find_in(tilemaps, id))
                return false;
            tile_views[id] = {camera_x, camera_y, std::max(0, width.value_or(0)), std::max(0, height.value_or(0))};
            request(LuaWorldDraw::tilemap, id);
            return true; });
    }

    void LuaWorld::Implementation::bind_fluid(sol::table table, const DrawRequest &request)
    {
        table.set_function("create", [this](const std::string &id, int width, int height)
                           {
            if (id.empty() || fluids.count(id) || width < 3 || height < 3 || width > max_fluid_2d_size ||
                height > max_fluid_2d_size)
                return false;
            FluidSimulation *simulation = create_fluid_simulation(width, height);
            if (!simulation)
                return false;
            fluids.emplace(id, simulation);
            return true; });
        table.set_function("destroy", [this](const std::string &id)
                           {
            const auto it = fluids.find(id);
            if (it == fluids.end())
                return false;
            destroy_fluid_simulation(it->second);
            fluids.erase(it);
            fluid_views.erase(id);
            return true; });
        table.set_function("clear", [this](const std::string &id)
                           {
            FluidSimulation *simulation = find_in(fluids, id);
            if (simulation)
                fluid_clear(simulation);
            return simulation != nullptr; });
        table.set_function("step", [this](const std::string &id, sol::optional<int> steps)
                           {
            FluidSimulation *simulation = find_in(fluids, id);
            if (!simulation)
                return false;
            for (int i = 0; i < std::clamp(steps.value_or(1), 1, 100); ++i)
                fluid_step(simulation);
            return true; });
        table.set_function("set_cell", [this](const std::string &id, int x, int y, const std::string &element,
                                              sol::optional<float> mass, sol::optional<float> temp)
                           {
            FluidSimulation *simulation = find_in(fluids, id);
            const auto type = element_from_name(element);
            return simulation && type && fluid_set_cell(simulation, x, y, *type, mass.value_or(type == FluidElement::empty ? 0.0f : 1.0f),
                                                        temp.value_or(20.0f)); });
        table.set_function("get_cell", [this](sol::this_state state, const std::string &id, int x, int y) -> sol::object
                           {
            const FluidSimulation *simulation = find_in(fluids, id);
            if (!simulation || !fluid_in_bounds(simulation, x, y))
                return sol::nil;
            return cell_table(sol::state_view(state), fluid_get_cell(simulation, x, y)); });
        table.set_function("size", [this](const std::string &id)
                           {
            const FluidSimulation *simulation = find_in(fluids, id);
            return std::make_tuple(fluid_width(simulation), fluid_height(simulation)); });
        table.set_function("draw", [this, request](const std::string &id, float x, float y, sol::optional<float> scale)
                           {
            if (!find_in(fluids, id))
                return false;
            fluid_views[id] = {x, y, std::clamp(scale.value_or(2.0f), 0.25f, 64.0f)};
            request(LuaWorldDraw::fluid, id);
            return true; });
    }

    void LuaWorld::Implementation::bind_fluid_3d(sol::table table)
    {
        table.set_function("create", [this](const std::string &id, int width, int height, int depth,
                                            sol::optional<int> origin_x, sol::optional<int> origin_y, sol::optional<int> origin_z)
                           {
            const int largest = std::max({width, height, depth});
            if (id.empty() || fluids_3d.count(id) || std::min({width, height, depth}) < 3 || largest > max_fluid_3d_size)
                return false;
            FluidSimulation3D *simulation = create_fluid_simulation_3d(width, height, depth);
            if (!simulation)
                return false;
            fluid3d_set_origin(simulation, origin_x.value_or(0), origin_y.value_or(0), origin_z.value_or(0));
            fluid3d_register_blocks(fluid_blocks);
            fluids_3d.emplace(id, simulation);
            return true; });
        table.set_function("destroy", [this](const std::string &id)
                           {
            const auto it = fluids_3d.find(id);
            if (it == fluids_3d.end())
                return false;
            destroy_fluid_simulation_3d(it->second);
            fluids_3d.erase(it);
            return true; });
        table.set_function("clear", [this](const std::string &id)
                           {
            FluidSimulation3D *simulation = find_in(fluids_3d, id);
            if (simulation)
                fluid3d_clear(simulation);
            return simulation != nullptr; });
        table.set_function("step", [this](const std::string &id, sol::optional<int> steps)
                           {
            FluidSimulation3D *simulation = find_in(fluids_3d, id);
            if (!simulation)
                return false;
            for (int i = 0; i < std::clamp(steps.value_or(1), 1, 100); ++i)
                fluid3d_step(simulation);
            return true; });
        table.set_function("set_budget", [this](const std::string &id, int blocks)
                           {
            FluidSimulation3D *simulation = find_in(fluids_3d, id);
            if (simulation)
                simulation->max_blocks_per_step = std::max(0, blocks);
            return simulation != nullptr; });
        table.set_function("set_cell", [this](const std::string &id, int x, int y, int z, const std::string &element,
                                              sol::optional<float> mass)
                           {
            FluidSimulation3D *simulation = find_in(fluids_3d, id);
            const auto type = element_from_name(element);
            return simulation && type && fluid3d_set_cell(simulation, x, y, z, *type, mass.value_or(type == FluidElement::empty ? 0.0f : 1.0f)); });
        table.set_function("get_cell", [this](sol::this_state state, const std::string &id, int x, int y, int z) -> sol::object
                           {
            const FluidSimulation3D *simulation = find_in(fluids_3d, id);
            if (!simulation || !fluid3d_in_bounds(simulation, x, y, z))
                return sol::nil;
            return cell_table(sol::state_view(state), fluid3d_get_cell(simulation, x, y, z)); });
        table.set_function("count", [this](const std::string &id)
                           {
            const FluidSimulation3D *simulation = find_in(fluids_3d, id);
            return simulation ? static_cast<lua_Integer>(fluid3d_count_non_empty(simulation)) : 0; });
        table.set_function("import", [this](const std::string &id, const std::string &world_id)
                           {
            FluidSimulation3D *simulation = find_in(fluids_3d, id);
            const VoxelWorld *voxels = world(world_id);
            if (!simulation || !voxels)
                return false;
            fluid3d_import_from_voxels(simulation, *voxels, fluid_blocks);
            return true; });
        table.set_function("export", [this](const std::string &id, const std::string &world_id)
                           {
            FluidSimulation3D *simulation = find_in(fluids_3d, id);
            VoxelWorld *voxels = world(world_id);
            return simulation && voxels ? static_cast<lua_Integer>(fluid3d_export_to_voxels(simulation, *voxels, fluid_blocks)) : 0; });
        table.set_function("move_region", [this](const std::string &id, const std::string &world_id, int x, int y, int z)
                           {
            FluidSimulation3D *simulation = find_in(fluids_3d, id);
            VoxelWorld *voxels = world(world_id);
            if (!simulation || !voxels)
                return false;
            fluid3d_move_region(simulation, *voxels, x, y, z, fluid_blocks);
            return true; });
    }

    void LuaWorld::Implementation::bind_voxel(sol::table table, const DrawRequest &request)
    {
        table.set_function("create_world", [this](const std::string &id)
                           {
            if (id.empty() || worlds.count(id))
                return false;
            worlds.emplace(id, std::make_unique<VoxelWorld>());
            return true; });
        table.set_function("destroy_world", [this](const std::string &id)
                           {
            destroy_view(id);
            atlases.erase(id);
            return worlds.erase(id) > 0; });
        table.set_function("define_block", [](int block, sol::table tint, sol::optional<sol::table> options)
                           {
            if (block < 1 || block > 255)
                return false;
            BlockDefinition definition;
            definition.tint = colour_from_table(tint, {255, 255, 255, 255});
            if (options)
            {
                definition.is_solid = options->get_or("solid", true);
                definition.is_transparent = options->get_or("transparent", false);
                definition.light_emission = static_cast<std::uint8_t>(std::clamp(options->get_or("emission", 0), 0, 15));
                // Atlas tiles: `tile` sets every face, then `top`, `bottom` and `side` override. Each is {column, row}.
                auto read_tile = [&](const char *key, std::initializer_list<int> faces)
                {
                    sol::optional<sol::table> tile = options->get<sol::optional<sol::table>>(key);
                    if (!tile)
                        return;
                    const VoxelTile value{std::clamp(tile->get_or(1, 0), 0, 255), std::clamp(tile->get_or(2, 0), 0, 255)};
                    for (const int face : faces)
                        definition.tiles[face] = value;
                };
                read_tile("tile", {0, 1, 2, 3, 4, 5});
                read_tile("side", {2, 3, 4, 5});
                read_tile("top", {0});
                read_tile("bottom", {1});
            }
            register_block_definition(static_cast<BlockId>(block), definition);
            return true; });
        table.set_function("set_atlas", [this](const std::string &id, const std::string &sprite, int columns, int rows,
                                               sol::optional<std::string> normal_sprite)
                           {
            VoxelWorld *voxels = world(id);
            if (!voxels || !bitmaps || !bitmaps(sprite) || columns < 1 || rows < 1 || columns > 256 || rows > 256 ||
                (normal_sprite && !bitmaps(*normal_sprite)))
                return false;
            atlases[id] = {sprite, normal_sprite.value_or(""), columns, rows};
            voxels->mark_all_dirty();
            return true; });
        table.set_function("set_block", [this](const std::string &id, int x, int y, int z, int block)
                           {
            VoxelWorld *voxels = world(id);
            return voxels && block >= 0 && block <= 255 && voxels->set_block(x, y, z, static_cast<BlockId>(block)); });
        table.set_function("get_block", [this](const std::string &id, int x, int y, int z)
                           {
            const VoxelWorld *voxels = world(id);
            return voxels ? static_cast<int>(voxels->get_block(x, y, z)) : 0; });
        table.set_function("fill", [this](const std::string &id, int x1, int y1, int z1, int x2, int y2, int z2, int block)
                           {
            VoxelWorld *voxels = world(id);
            if (!voxels || block < 0 || block > 255)
                return false;
            const long long volume = (static_cast<long long>(std::abs(x2 - x1)) + 1) * (std::abs(y2 - y1) + 1) *
                                     (std::abs(z2 - z1) + 1);
            if (volume > max_fill_blocks)
                return false;
            for (int z = std::min(z1, z2); z <= std::max(z1, z2); ++z)
                for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y)
                    for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x)
                        voxels->set_block(x, y, z, static_cast<BlockId>(block));
            return true; });
        table.set_function("update_lighting", [this](const std::string &id)
                           {
            VoxelWorld *voxels = world(id);
            if (voxels)
                voxels->update_lighting();
            return voxels != nullptr; });
        table.set_function("stats", [this](sol::this_state state, const std::string &id) -> sol::object
                           {
            const VoxelWorld *voxels = world(id);
            if (!voxels)
                return sol::nil;
            sol::table stats = sol::state_view(state).create_table();
            stats["chunks"] = static_cast<lua_Integer>(voxels->chunk_count());
            stats["triangles"] = voxels->total_triangles();
            return stats; });
        table.set_function("raycast", [this](sol::this_state state, const std::string &id, float ox, float oy, float oz,
                                             float dx, float dy, float dz, sol::optional<float> max_distance) -> sol::object
                           {
            const VoxelWorld *voxels = world(id);
            const glm::vec3 direction(dx, dy, dz);
            if (!voxels || glm::dot(direction, direction) < 1e-12f)
                return sol::nil;
            const VoxelRaycastResult hit = voxels->raycast({ox, oy, oz}, glm::normalize(direction),
                                                           std::clamp(max_distance.value_or(8.0f), 0.0f, 512.0f));
            if (!hit.hit)
                return sol::nil;
            sol::table result = sol::state_view(state).create_table();
            result["x"] = hit.hit_block.x;
            result["y"] = hit.hit_block.y;
            result["z"] = hit.hit_block.z;
            result["place_x"] = hit.place_block.x;
            result["place_y"] = hit.place_block.y;
            result["place_z"] = hit.place_block.z;
            result["face"] = static_cast<int>(hit.hit_face);
            result["distance"] = hit.distance;
            return result; });
        table.set_function("view", [this, request](const std::string &id, int x, int y, int width, int height,
                                                   sol::table eye, sol::table target, sol::optional<float> field_of_view,
                                                   sol::optional<sol::table> sky)
                           {
            if (!world(id) || width < 1 || height < 1 || width > max_view_size || height > max_view_size)
                return false;
            VoxelView &view = voxel_views[id];
            if (view.surface && (view.width != width || view.height != height))
            {
                destroy_surface_3d(view.surface);
                view.surface = nullptr;
            }
            view.x = x;
            view.y = y;
            view.width = width;
            view.height = height;
            view.camera.position = {eye.get_or(1, 0.0f), eye.get_or(2, 0.0f), eye.get_or(3, 0.0f)};
            view.camera.target = {target.get_or(1, 0.0f), target.get_or(2, 0.0f), target.get_or(3, 0.0f)};
            view.camera.field_of_view = std::clamp(field_of_view.value_or(60.0f), 10.0f, 120.0f);
            view.camera.near_plane = 0.1f;
            view.camera.far_plane = 500.0f;
            view.sky = colour_from_table(sky, view.sky);
            request(LuaWorldDraw::voxel_view, id);
            return true; });
    }

    namespace
    {
        constexpr int max_terrain_grid = 4096;

        CloudLayer cloud_layer_from_table(const sol::table &clouds, CloudLayer layer)
        {
            // A type starts from that type's usual layer, so any other key overrides it.
            if (const auto type = clouds.get<sol::optional<std::string>>("type"))
            {
                static const std::pair<const char *, CloudType> types[] = {
                    {"cumulus", CloudType::cumulus},           {"stratocumulus", CloudType::stratocumulus},
                    {"stratus", CloudType::stratus},           {"altocumulus", CloudType::altocumulus},
                    {"cirrus", CloudType::cirrus},             {"cumulonimbus", CloudType::cumulonimbus}};
                bool known = false;
                for (const auto &[name, value] : types)
                    if (*type == name)
                    {
                        layer = cloud_layer(value);
                        known = true;
                    }
                if (!known)
                    throw std::runtime_error("unknown cloud type '" + *type + "'");
            }
            layer.enabled = clouds.get_or("enabled", true);
            layer.volumetric = clouds.get_or("volumetric", layer.volumetric);
            layer.altitude = clouds.get_or("altitude", layer.altitude);
            layer.thickness = clouds.get_or("thickness", layer.thickness);
            layer.coverage = std::clamp(clouds.get_or("coverage", layer.coverage), 0.0f, 1.0f);
            layer.density = std::clamp(clouds.get_or("density", layer.density), 0.0f, 1.0f);
            layer.scale = clouds.get_or("scale", layer.scale);
            layer.softness = std::clamp(clouds.get_or("softness", layer.softness), 0.0f, 1.0f);
            if (const auto wind = clouds.get<sol::optional<sol::table>>("wind"))
                layer.wind = {wind->get_or(1, layer.wind.x), wind->get_or(2, layer.wind.y)};
            return layer;
        }

        Environment3D environment_from_table(const sol::table &t, Environment3D e)
        {
            e.fog_enabled = t.get_or("fog", e.fog_enabled);
            e.fog_density = t.get_or("fog_density", e.fog_density);
            e.fog_height_falloff = t.get_or("fog_height_falloff", e.fog_height_falloff);
            e.fog_colour = colour_from_table(t.get<sol::optional<sol::table>>("fog_colour"), e.fog_colour);
            e.sky_zenith = colour_from_table(t.get<sol::optional<sol::table>>("sky_zenith"), e.sky_zenith);
            e.sky_horizon = colour_from_table(t.get<sol::optional<sol::table>>("sky_horizon"), e.sky_horizon);
            e.sun_intensity = t.get_or("sun_intensity", e.sun_intensity);
            e.sun_diameter = t.get_or("sun_diameter", e.sun_diameter);
            e.planet = t.get_or("planet", e.planet);
            e.planet_radius = t.get_or("planet_radius", e.planet_radius);
            e.atmosphere_height = t.get_or("atmosphere_height", e.atmosphere_height);
            e.haze_scale = t.get_or("haze_scale", e.haze_scale);
            if (const auto clouds = t.get<sol::optional<sol::table>>("clouds"))
            {
                // Either one layer ({type = "cumulus", ...}) or a list of up to four ({{...}, {...}}).
                if (clouds->get<sol::optional<sol::table>>(1))
                {
                    CloudLayer *slots[4] = {&e.clouds, &e.more_clouds[0], &e.more_clouds[1], &e.more_clouds[2]};
                    for (int i = 0; i < 4; ++i)
                    {
                        *slots[i] = CloudLayer{};
                        if (const auto layer = clouds->get<sol::optional<sol::table>>(i + 1))
                            *slots[i] = cloud_layer_from_table(*layer, *slots[i]);
                    }
                }
                else
                    e.clouds = cloud_layer_from_table(*clouds, e.clouds);
            }
            return e;
        }

        dvec3 dvec3_from_table(sol::optional<sol::table> table, const dvec3 &fallback)
        {
            if (!table)
                return fallback;
            return {table->get_or(1, fallback.x), table->get_or(2, fallback.y), table->get_or(3, fallback.z)};
        }

        sol::table vector_table(sol::state_view lua, const dvec3 &value)
        {
            sol::table result = lua.create_table();
            result["x"] = value.x;
            result["y"] = value.y;
            result["z"] = value.z;
            return result;
        }

        NoiseParameters noise_from_table(const sol::table &table)
        {
            NoiseParameters noise;
            noise.seed = table.get_or("seed", noise.seed);
            noise.frequency = std::max(1.0e-9, table.get_or("frequency", noise.frequency));
            noise.octaves = std::clamp(table.get_or("octaves", noise.octaves), 1, 16);
            noise.lacunarity = std::clamp(table.get_or("lacunarity", noise.lacunarity), 1.1, 4.0);
            noise.gain = std::clamp(table.get_or("gain", noise.gain), 0.0, 1.0);
            noise.amplitude = table.get_or("amplitude", noise.amplitude);
            noise.base_height = table.get_or("base_height", noise.base_height);
            noise.ridged = table.get_or("ridged", noise.ridged);
            noise.floor_height = table.get_or("floor_height", noise.floor_height);
            return noise;
        }
    }

    void LuaWorld::Implementation::bind_terrain(sol::table table, const DrawRequest &request)
    {
        // terrain.create(id, options): options.source selects the height source, the rest is the TerrainConfig.
        table.set_function("create", [this](const std::string &id, sol::table options)
                           {
            if (id.empty() || terrains.count(id))
                return false;
            TerrainConfig config;
            const std::string shape = options.get_or("shape", std::string("flat"));
            if (shape != "flat" && shape != "sphere")
                return false;
            config.shape = shape == "sphere" ? TerrainShape::sphere : TerrainShape::flat;
            config.size = std::max(1.0, options.get_or("size", config.size));
            config.origin = dvec3_from_table(options.get<sol::optional<sol::table>>("origin"), config.origin);
            config.radius = std::max(1.0, options.get_or("radius", config.radius));
            config.chunk_cells = std::clamp(options.get_or("chunk_cells", config.chunk_cells), 4, 128);
            config.finest_cell_size = std::max(0.01, options.get_or("finest_cell_size", config.finest_cell_size));
            config.max_level = std::clamp(options.get_or("max_level", config.max_level), -1, 24);
            config.max_screen_error = std::max(0.5, options.get_or("max_screen_error", config.max_screen_error));
            config.geomorph = options.get_or("geomorph", config.geomorph);

            TerrainEntry entry;
            auto sprite_of = [this](const sol::object &value) -> Bitmap *
            {
                if (value.get_type() != sol::type::string)
                    return nullptr;
                return bitmaps ? bitmaps(value.as<std::string>()) : nullptr;
            };
            bool any_albedo = false;
            if (sol::optional<sol::table> layers = options.get<sol::optional<sol::table>>("layers"))
            {
                for (std::size_t i = 1; i <= layers->size() && config.layers.size() < static_cast<std::size_t>(terrain_max_layers); ++i)
                {
                    sol::optional<sol::table> description = layers->get<sol::optional<sol::table>>(i);
                    if (!description)
                        return false;
                    TerrainLayer layer;
                    if (sol::optional<sol::table> colour = description->get<sol::optional<sol::table>>("colour"))
                        layer.colour = {static_cast<Uint8>(std::clamp(colour->get_or(1, 128), 0, 255)),
                                        static_cast<Uint8>(std::clamp(colour->get_or(2, 128), 0, 255)),
                                        static_cast<Uint8>(std::clamp(colour->get_or(3, 128), 0, 255)), 255};
                    layer.tile_metres = std::max(0.01, description->get_or("tile_metres", layer.tile_metres));
                    layer.min_height = description->get_or("min_height", layer.min_height);
                    layer.max_height = description->get_or("max_height", layer.max_height);
                    layer.height_blend = description->get_or("height_blend", layer.height_blend);
                    layer.min_slope = description->get_or("min_slope", layer.min_slope);
                    layer.max_slope = description->get_or("max_slope", layer.max_slope);
                    layer.slope_blend = description->get_or("slope_blend", layer.slope_blend);
                    layer.strength = description->get_or("strength", layer.strength);
                    const sol::object sprite = description->get<sol::object>("sprite");
                    if (sprite.valid() && sprite.get_type() != sol::type::lua_nil)
                    {
                        if (!(layer.albedo = sprite_of(sprite)))
                            return false;
                        any_albedo = true;
                        const sol::object normal = description->get<sol::object>("normal");
                        if (normal.valid() && normal.get_type() != sol::type::lua_nil && !(layer.normal_map = sprite_of(normal)))
                            return false;
                        if (sol::optional<sol::table> variants = description->get<sol::optional<sol::table>>("variants"))
                            for (std::size_t k = 1; k <= variants->size(); ++k)
                            {
                                // A variant is a sprite name or a table {sprite = ..., normal = ...}.
                                const sol::object item = variants->get<sol::object>(k);
                                TerrainLayerVariant variant;
                                if (item.get_type() == sol::type::table)
                                {
                                    const sol::table fields = item.as<sol::table>();
                                    variant.albedo = sprite_of(fields.get<sol::object>("sprite"));
                                    const sol::object normal_field = fields.get<sol::object>("normal");
                                    if (normal_field.valid() && normal_field.get_type() != sol::type::lua_nil &&
                                        !(variant.normal_map = sprite_of(normal_field)))
                                        return false;
                                }
                                else
                                    variant.albedo = sprite_of(item);
                                if (!variant.albedo)
                                    return false;
                                layer.variants.push_back(variant);
                            }
                    }
                    config.layers.push_back(layer);
                }
            }
            if (sol::optional<sol::table> textures = options.get<sol::optional<sol::table>>("textures"))
            {
                TerrainTextureOptions &texture = entry.texture_options;
                texture.fade_start = textures->get_or("fade_start", texture.fade_start);
                texture.fade_end = textures->get_or("fade_end", texture.fade_end);
                texture.macro_tile_metres = textures->get_or("macro_tile_metres", texture.macro_tile_metres);
                texture.macro_strength = textures->get_or("macro_strength", texture.macro_strength);
                texture.max_size = std::clamp(textures->get_or("max_size", texture.max_size), 4, 8192);
                const sol::object macro = textures->get<sol::object>("macro_sprite");
                if (macro.valid() && macro.get_type() != sol::type::lua_nil && !(texture.macro_texture = sprite_of(macro)))
                    return false;
            }
            entry.textures_pending = any_albedo;
            std::shared_ptr<TerrainSource> source;
            sol::optional<sol::table> description = options.get<sol::optional<sol::table>>("source");
            const std::string kind = description ? description->get_or("type", std::string("noise")) : "noise";
            if (kind == "noise")
            {
                source = std::make_shared<NoiseSource>(description ? noise_from_table(*description) : NoiseParameters{});
            }
            else if (kind == "flat")
            {
                const double height = description->get_or("height", 0.0);
                source = std::make_shared<FunctionSource>([height](double, double) { return height; }, height, height,
                                                          [height](const dvec3 &, double) { return height; });
            }
            else if (kind == "grid")
            {
                const int width = description->get_or("width", 0);
                const int depth = description->get_or("depth", width);
                if (width < 2 || depth < 2 || width > max_terrain_grid || depth > max_terrain_grid)
                    return false;
                const double world_width = std::max(1.0, description->get_or("world_width", config.size));
                const double world_depth = std::max(1.0, description->get_or("world_depth", world_width));
                entry.grid = std::make_shared<GridSource>(width, depth, world_width, world_depth,
                                                          description->get_or("origin_x", config.origin.x),
                                                          description->get_or("origin_z", config.origin.z),
                                                          description->get_or("height", 0.0f));
                source = entry.grid;
            }
            else if (kind == "heightmap")
            {
                const std::string sprite = description->get_or("sprite", std::string());
                Bitmap *bitmap = bitmaps ? bitmaps(sprite) : nullptr;
                const double world_width = std::max(1.0, description->get_or("world_width", config.size));
                if (!bitmap)
                    return false;
                source = HeightmapSource::from_bitmap(bitmap, description->get_or("min_height", 0.0),
                                                      description->get_or("max_height", 500.0), world_width,
                                                      std::max(1.0, description->get_or("world_depth", world_width)),
                                                      description->get_or("origin_x", config.origin.x),
                                                      description->get_or("origin_z", config.origin.z),
                                                      description->get_or("pack_16_bit", false));
            }
            if (!source)
                return false;
            entry.terrain = std::make_unique<Terrain>(config, source);
            terrains.emplace(id, std::move(entry));
            return true; });
        table.set_function("destroy", [this](const std::string &id)
                           {
            const auto it = terrains.find(id);
            if (it == terrains.end())
                return false;
            if (it->second.surface)
                destroy_surface_3d(it->second.surface);
            terrains.erase(it);
            return true; });
        table.set_function("height_at", [this](const std::string &id, double x, double z) -> sol::optional<double>
                           {
            const TerrainEntry *entry = terrain(id);
            if (!entry)
                return sol::nullopt;
            return entry->terrain->height_at(x, z); });
        table.set_function("normal_at", [this](sol::this_state state, const std::string &id, double x, double z) -> sol::object
                           {
            const TerrainEntry *entry = terrain(id);
            if (!entry)
                return sol::nil;
            return vector_table(state, entry->terrain->normal_at(x, z)); });
        table.set_function("height_at_direction", [this](const std::string &id, double x, double y, double z) -> sol::optional<double>
                           {
            const TerrainEntry *entry = terrain(id);
            const dvec3 direction(x, y, z);
            if (!entry || glm::dot(direction, direction) < 1e-24)
                return sol::nullopt;
            return entry->terrain->height_at_direction(glm::normalize(direction)); });
        table.set_function("altitude_of", [this](const std::string &id, double x, double y, double z) -> sol::optional<double>
                           {
            const TerrainEntry *entry = terrain(id);
            if (!entry)
                return sol::nullopt;
            return entry->terrain->altitude_of({x, y, z}); });
        table.set_function("raycast", [this](sol::this_state state, const std::string &id, double ox, double oy, double oz,
                                             double dx, double dy, double dz, sol::optional<double> max_distance) -> sol::object
                           {
            const TerrainEntry *entry = terrain(id);
            const dvec3 direction(dx, dy, dz);
            if (!entry || glm::dot(direction, direction) < 1e-24)
                return sol::nil;
            const TerrainHit hit = entry->terrain->raycast({ox, oy, oz}, glm::normalize(direction),
                                                           std::clamp(max_distance.value_or(10000.0), 0.0, 1.0e9));
            if (!hit.hit)
                return sol::nil;
            sol::table result = vector_table(state, hit.position);
            result["normal"] = vector_table(state, hit.normal);
            result["distance"] = hit.distance;
            return result; });
        // Grid edits: raise, flatten and smooth only apply to sources created with type = "grid".
        table.set_function("raise", [this](const std::string &id, double x, double z, double radius, double amount,
                                           sol::optional<double> falloff)
                           {
            TerrainEntry *entry = terrain(id);
            if (!entry || !entry->grid)
                return false;
            entry->grid->raise(x, z, std::max(0.0, radius), amount, falloff.value_or(1.0));
            return true; });
        table.set_function("flatten", [this](const std::string &id, double x, double z, double radius, double height,
                                             sol::optional<double> strength)
                           {
            TerrainEntry *entry = terrain(id);
            if (!entry || !entry->grid)
                return false;
            entry->grid->flatten(x, z, std::max(0.0, radius), height, strength.value_or(1.0));
            return true; });
        table.set_function("smooth", [this](const std::string &id, double x, double z, double radius,
                                            sol::optional<double> strength)
                           {
            TerrainEntry *entry = terrain(id);
            if (!entry || !entry->grid)
                return false;
            entry->grid->smooth(x, z, std::max(0.0, radius), strength.value_or(0.5));
            return true; });
        table.set_function("set_sample", [this](const std::string &id, int ix, int iz, float height)
                           {
            TerrainEntry *entry = terrain(id);
            if (!entry || !entry->grid)
                return false;
            entry->grid->set_sample(ix, iz, height);
            return true; });
        table.set_function("stats", [this](sol::this_state state, const std::string &id) -> sol::object
                           {
            const TerrainEntry *entry = terrain(id);
            if (!entry)
                return sol::nil;
            const TerrainStats stats = entry->terrain->stats();
            sol::table result = sol::state_view(state).create_table();
            result["visible_chunks"] = stats.visible_chunks;
            result["visible_triangles"] = stats.visible_triangles;
            result["resident_chunks"] = stats.resident_chunks;
            result["pending_chunks"] = stats.pending_chunks;
            result["deepest_level"] = stats.deepest_level;
            return result; });
        // terrain.view(id, x, y, width, height, eye, target, options): options are fov, near, far, up, sky, light, ambient.
        table.set_function("view", [this, request](const std::string &id, int x, int y, int width, int height,
                                                   sol::table eye, sol::table target, sol::optional<sol::table> options)
                           {
            TerrainEntry *entry = terrain(id);
            if (!entry || width < 1 || height < 1 || width > max_view_size || height > max_view_size)
                return false;
            if (entry->surface && (entry->width != width || entry->height != height))
            {
                destroy_surface_3d(entry->surface);
                entry->surface = nullptr;
            }
            entry->x = x;
            entry->y = y;
            entry->width = width;
            entry->height = height;
            entry->eye = dvec3_from_table(eye, entry->eye);
            const dvec3 aim = dvec3_from_table(target, entry->eye + entry->forward) - entry->eye;
            if (glm::dot(aim, aim) > 1e-18)
                entry->forward = glm::normalize(aim);
            if (options)
            {
                entry->up = dvec3_from_table(options->get<sol::optional<sol::table>>("up"), entry->up);
                entry->fov = std::clamp(options->get_or("fov", entry->fov), 10.0, 120.0);
                entry->near_plane = std::max(0.01, options->get_or("near", entry->near_plane));
                entry->far_plane = std::max(entry->near_plane * 2.0, options->get_or("far", entry->far_plane));
                entry->ambient = std::clamp(options->get_or("ambient", entry->ambient), 0.0f, 1.0f);
                entry->sky = colour_from_table(options->get<sol::optional<sol::table>>("sky"), entry->sky);
                const dvec3 light = dvec3_from_table(options->get<sol::optional<sol::table>>("light"),
                                                     dvec3(entry->light_direction));
                if (glm::dot(light, light) > 1e-18)
                    entry->light_direction = glm::vec3(glm::normalize(light));
            }
            if (options)
            {
                const auto env = options->get<sol::optional<sol::table>>("environment");
                if (env)
                    entry->environment = environment_from_table(*env, entry->environment), entry->has_environment = true;
                else if (options->get_or("clear_environment", false))
                    entry->has_environment = false;
            }
            request(LuaWorldDraw::terrain_view, id);
            return true; });
        // terrain.add_clutter(id, options): scatter trees or rocks over the terrain. Returns the species index or nil.
        table.set_function("add_clutter", [this](const std::string &id, sol::table options) -> sol::optional<int>
                           {
            TerrainEntry *entry = terrain(id);
            if (!entry)
                return sol::nullopt;
            ClutterSpecies species;
            const std::string mesh = options.get_or<std::string>("mesh", "conifer");
            const float size = options.get_or("size", mesh == "rock" ? 1.0f : 8.0f);
            if (mesh == "rock")
                species.mesh = make_rock_data(size, colour_from_table(options.get<sol::optional<sol::table>>("colour"), {120, 118, 112, 255}),
                                              static_cast<std::uint32_t>(options.get_or("shape_seed", 1)));
            else if (mesh == "conifer")
                species.mesh = make_conifer_data(size, options.get_or("radius", size * 0.25f));
            else
                return sol::nullopt;
            species.impostor = options.get_or("impostor", true);
            species.density = std::clamp(options.get_or("density", species.density), 0.0f, 1.0f);
            species.min_height = options.get_or("min_height", species.min_height);
            species.max_height = options.get_or("max_height", species.max_height);
            species.min_slope = options.get_or("min_slope", species.min_slope);
            species.max_slope = options.get_or("max_slope", species.max_slope);
            species.min_scale = std::max(0.01f, options.get_or("min_scale", species.min_scale));
            species.max_scale = std::max(species.min_scale, options.get_or("max_scale", species.max_scale));
            species.lean = options.get_or("lean", species.lean);
            species.tint_variation = options.get_or("tint_variation", species.tint_variation);
            species.near_distance = options.get_or("near_distance", species.near_distance);
            species.far_distance = options.get_or("far_distance", species.far_distance);
            species.seed = static_cast<std::uint32_t>(options.get_or("seed", static_cast<int>(entry->pending_species.size()) + 1));
            // Built on the next draw, when a graphics context and no active surface are guaranteed.
            entry->pending_species.push_back(std::move(species));
            if (entry->clutter)
                entry->clutter.reset();
            return static_cast<int>(entry->pending_species.size()); });
        table.set_function("clear_clutter", [this](const std::string &id)
                           {
            TerrainEntry *entry = terrain(id);
            if (!entry)
                return false;
            entry->clutter.reset();
            entry->pending_species.clear();
            return true; });
    }

    LuaWorld::LuaWorld()
        : implementation_(new Implementation)
    {
    }

    LuaWorld::~LuaWorld()
    {
        clear();
        delete implementation_;
    }

    void LuaWorld::bind(sol::state &lua, PathResolver resolver, DrawRequest request, BitmapResolver bitmaps)
    {
        implementation_->bitmaps = std::move(bitmaps);
        implementation_->bind_tilemap(lua.create_named_table("tilemap"), resolver, request);
        implementation_->bind_fluid(lua.create_named_table("fluid"), request);
        implementation_->bind_fluid_3d(lua.create_named_table("fluid3d"));
        implementation_->bind_voxel(lua.create_named_table("voxel"), request);
        implementation_->bind_terrain(lua.create_named_table("terrain"), request);
    }

    void LuaWorld::clear()
    {
        implementation_->clear();
    }

    void LuaWorld::draw(LuaWorldDraw kind, const std::string &id, Bitmap *target) const
    {
        Implementation &state = *implementation_;
        switch (kind)
        {
        case LuaWorldDraw::tilemap:
        {
            const TileMap *map = find_in(state.tilemaps, id);
            const auto view = state.tile_views.find(id);
            if (map && view != state.tile_views.end())
                draw_tilemap(map, view->second.x, view->second.y, view->second.width, view->second.height);
            break;
        }
        case LuaWorldDraw::fluid:
        {
            const FluidSimulation *simulation = find_in(state.fluids, id);
            const auto view = state.fluid_views.find(id);
            if (!simulation || view == state.fluid_views.end())
                break;
            const float scale = view->second.scale;
            for (int y = 0; y < simulation->height; ++y)
            {
                for (int x = 0; x < simulation->width; ++x)
                {
                    const FluidCell &cell = simulation->cells[static_cast<std::size_t>(y) * simulation->width + x];
                    if (cell.type == FluidElement::empty)
                        continue;
                    const float left = view->second.x + static_cast<float>(x) * scale;
                    const float top = view->second.y + static_cast<float>(y) * scale;
                    rectfill(target, left, top, left + scale, top + scale, element_entry(cell.type).colour);
                }
            }
            break;
        }
        case LuaWorldDraw::voxel_view:
        {
            VoxelWorld *voxels = state.world(id);
            const auto view = state.voxel_views.find(id);
            if (!voxels || view == state.voxel_views.end())
                break;
            Implementation::VoxelView &voxel_view = view->second;
            if (!voxel_view.surface)
                voxel_view.surface = create_surface_3d(voxel_view.width, voxel_view.height);
            if (!voxel_view.surface)
                break;
            Material material;
            material.cull = CullMode::none;
            VoxelAtlas atlas;
            const auto atlas_entry = state.atlases.find(id);
            if (atlas_entry != state.atlases.end() && state.bitmaps)
            {
                atlas.texture = state.bitmaps(atlas_entry->second.sprite);
                atlas.tiles_per_row = atlas_entry->second.columns;
                atlas.tiles_per_col = atlas_entry->second.rows;
                if (atlas.texture)
                {
                    material.texture = atlas.texture;
                    if (!atlas_entry->second.normal_sprite.empty())
                        material.normal_map = state.bitmaps(atlas_entry->second.normal_sprite);
                }
            }
            voxels->update_dirty_chunks(atlas.texture ? &atlas : nullptr);
            if (begin_surface_3d(voxel_view.surface, voxel_view.camera))
            {
                clear_surface_3d(voxel_view.sky);
                voxels->draw(material);
                end_surface_3d();
                draw_sprite(surface_3d_bitmap(voxel_view.surface), static_cast<float>(voxel_view.x),
                            static_cast<float>(voxel_view.y));
            }
            break;
        }
        case LuaWorldDraw::terrain_view:
        {
            Implementation::TerrainEntry *entry = state.terrain(id);
            if (!entry || entry->width < 1)
                break;
            if (!entry->surface)
                entry->surface = create_surface_3d(entry->width, entry->height);
            if (!entry->surface)
                break;
            if (entry->textures_pending)
            {
                entry->terrain->set_layer_textures(entry->texture_options);
                entry->textures_pending = false;
                entry->texture_options.macro_texture = nullptr;
            }
            TerrainView view;
            view.position = entry->eye;
            view.forward = entry->forward;
            view.up = entry->up;
            view.fov_y_degrees = entry->fov;
            view.aspect = static_cast<double>(entry->width) / entry->height;
            view.viewport_height = entry->height;
            view.far_distance = entry->far_plane;
            entry->terrain->update(view);
            if (!entry->pending_species.empty() && !entry->clutter)
            {
                // Species are baked here, outside any active surface.
                entry->clutter = std::make_unique<ClutterLayer>(*entry->terrain);
                for (const ClutterSpecies &species : entry->pending_species)
                    entry->clutter->add_species(species);
            }
            if (entry->clutter)
                entry->clutter->update(view);

            // The camera sits at the origin and the terrain is drawn relative to the eye to keep floats precise.
            Camera camera;
            camera.position = glm::vec3(0.0f);
            camera.target = glm::vec3(entry->forward);
            camera.up = glm::vec3(entry->up);
            camera.field_of_view = static_cast<float>(entry->fov);
            camera.near_plane = static_cast<float>(entry->near_plane);
            camera.far_plane = static_cast<float>(entry->far_plane);
            if (begin_surface_3d(entry->surface, camera))
            {
                clear_surface_3d(entry->sky);
                Light3D light;
                light.direction = entry->light_direction;
                light.ambient = entry->ambient;
                if (entry->has_environment)
                {
                    Environment3D environment = entry->environment;
                    environment.sun_direction = entry->light_direction;
                    environment.up = glm::vec3(entry->up);
                    environment.camera_position = entry->eye;
                    environment.camera_altitude = static_cast<float>(entry->terrain->altitude_of(entry->eye));
                    environment.time = time_ms() * 0.001;
                    set_environment_3d(environment);
                    draw_sky_3d();
                    light.colour = atmosphere_sun_colour(environment);
                }
                else
                {
                    clear_environment_3d();
                }
                set_light_3d(light);
                entry->terrain->draw(entry->eye, Material{});
                if (entry->clutter)
                    entry->clutter->draw(entry->eye);
                clear_environment_3d();
                end_surface_3d();
                draw_sprite(surface_3d_bitmap(entry->surface), static_cast<float>(entry->x),
                            static_cast<float>(entry->y));
            }
            break;
        }
        }
    }
}
