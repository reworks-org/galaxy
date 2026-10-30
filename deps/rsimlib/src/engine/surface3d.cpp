#include "surface3d.h"

#include "atmosphere_internal.h"
#include "deferred_mesh.h"
#include "error.h"
#include "renderer.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <cmath>
#include <memory>

namespace sl
{
    namespace
    {
        struct ActiveSurface
        {
            Surface3D *surface = nullptr;
            glm::mat4 view_projection{1.0f};
            Camera camera;
            ShadowMap3D *shadow_pass = nullptr;
            bool local_shadow_pass = false;
        };

        ActiveSurface active;
        Light3D current_light;
        std::vector<LocalLight3D> local_lights;
        const ShadowMap3D *current_shadow = nullptr;
        float current_shadow_bias = 0.0015f;
        const LocalShadowMap3D *current_local_shadow = nullptr;
        Vertex3D make_vertex(const glm::vec3 &position, const glm::vec3 &normal, Colour colour,
                             float u = 0.0f, float v = 0.0f)
        {
            Vertex3D vertex;
            vertex.position[0] = position.x;
            vertex.position[1] = position.y;
            vertex.position[2] = position.z;
            vertex.normal[0] = normal.x;
            vertex.normal[1] = normal.y;
            vertex.normal[2] = normal.z;
            vertex.colour[0] = colour.red / 255.0f;
            vertex.colour[1] = colour.green / 255.0f;
            vertex.colour[2] = colour.blue / 255.0f;
            vertex.colour[3] = colour.alpha / 255.0f;
            vertex.uv[0] = u;
            vertex.uv[1] = v;
            return vertex;
        }

        /** Accumulate per-triangle UV tangents into per-vertex tangents (Gram-Schmidt against the normal). */
        void generate_tangents(std::vector<Vertex3D> &vertices, const std::vector<std::uint32_t> &indices)
        {
            std::vector<glm::vec3> tangents(vertices.size(), glm::vec3(0.0f));
            std::vector<glm::vec3> bitangents(vertices.size(), glm::vec3(0.0f));
            const auto position = [&](std::uint32_t i)
            { return glm::vec3(vertices[i].position[0], vertices[i].position[1], vertices[i].position[2]); };
            for (std::size_t t = 0; t + 2 < indices.size(); t += 3)
            {
                const std::uint32_t a = indices[t], b = indices[t + 1], c = indices[t + 2];
                const glm::vec3 e1 = position(b) - position(a);
                const glm::vec3 e2 = position(c) - position(a);
                const float du1 = vertices[b].uv[0] - vertices[a].uv[0], dv1 = vertices[b].uv[1] - vertices[a].uv[1];
                const float du2 = vertices[c].uv[0] - vertices[a].uv[0], dv2 = vertices[c].uv[1] - vertices[a].uv[1];
                const float determinant = du1 * dv2 - du2 * dv1;
                if (std::fabs(determinant) < 1e-12f)
                    continue;
                const float inverse = 1.0f / determinant;
                const glm::vec3 tangent = (e1 * dv2 - e2 * dv1) * inverse;
                const glm::vec3 bitangent = (e2 * du1 - e1 * du2) * inverse;
                for (const std::uint32_t i : {a, b, c})
                {
                    tangents[i] += tangent;
                    bitangents[i] += bitangent;
                }
            }
            for (std::size_t i = 0; i < vertices.size(); ++i)
            {
                const glm::vec3 normal(vertices[i].normal[0], vertices[i].normal[1], vertices[i].normal[2]);
                glm::vec3 tangent = tangents[i] - normal * glm::dot(normal, tangents[i]);
                if (glm::dot(tangent, tangent) < 1e-12f)
                    continue;
                tangent = glm::normalize(tangent);
                // UV v runs down the image while normal-map green points up, so the handedness is flipped.
                const float handedness = glm::dot(glm::cross(normal, tangent), bitangents[i]) < 0.0f ? 1.0f : -1.0f;
                vertices[i].tangent[0] = tangent.x;
                vertices[i].tangent[1] = tangent.y;
                vertices[i].tangent[2] = tangent.z;
                vertices[i].tangent[3] = handedness;
            }
        }

        bool has_tangents(const std::vector<Vertex3D> &vertices)
        {
            for (const Vertex3D &vertex : vertices)
                if (vertex.tangent[0] != 0.0f || vertex.tangent[1] != 0.0f || vertex.tangent[2] != 0.0f ||
                    vertex.tangent[3] != 0.0f)
                    return true;
            return false;
        }

        /** 1x1 (128,128,255) texture used when a material has no normal map; rebuilt if the renderer changes. */
        Bitmap *flat_normal_bitmap()
        {
            static std::unique_ptr<Bitmap> bitmap;
            static detail::Renderer *owner = nullptr;
            detail::Renderer *renderer = detail::active_renderer();
            if (bitmap && owner != renderer)
            {
                // The old renderer (and its textures) is gone; only free the CPU-side object.
                bitmap->gpu_texture = 0;
                bitmap.reset();
            }
            if (!bitmap)
            {
                bitmap.reset(create_bitmap(1, 1));
                if (bitmap)
                {
                    putpixel(bitmap.get(), 0, 0, Colour{128, 128, 255, 255});
                    owner = renderer;
                }
            }
            return bitmap.get();
        }
    } // namespace

    Surface3D *create_surface_3d(int width, int height, int samples)
    {
        if (width <= 0 || height <= 0)
        {
            detail::set_error("Surface dimensions must be positive");
            return nullptr;
        }
        detail::Renderer *renderer = detail::active_renderer();
        std::uint32_t texture = 0;
        std::uint32_t framebuffer = 0;
        samples = samples < 1 ? 1 : samples;
        if (!renderer || !renderer->create_render_target_3d(width, height, samples, texture, framebuffer))
        {
            detail::set_error("3D surfaces are not supported by the active graphics backend");
            return nullptr;
        }
        // Compile the 3D pipelines now rather than on the first draw.
        renderer->warm_up_pipelines(samples);
        Bitmap *bitmap = new Bitmap;
        bitmap->width = width;
        bitmap->height = height;
        bitmap->gpu_texture = texture;
        bitmap->fbo = framebuffer;
        Surface3D *surface = new Surface3D;
        surface->bitmap = bitmap;
        surface->samples = samples;
        return surface;
    }

    int surface_3d_samples(const Surface3D *surface)
    {
        return surface ? surface->samples : 1;
    }

    void destroy_surface_3d(Surface3D *surface)
    {
        if (!surface)
            return;
        if (active.surface == surface)
            end_surface_3d();
        destroy_bitmap(surface->bitmap);
        delete surface;
    }

    Bitmap *surface_3d_bitmap(Surface3D *surface)
    {
        return surface ? surface->bitmap : nullptr;
    }

    bool begin_surface_3d(Surface3D *surface, const Camera &camera)
    {
        if (!surface || !surface->bitmap || active.surface)
        {
            detail::set_error("Invalid 3D surface state");
            return false;
        }
        if (!begin_render_target(surface->bitmap))
            return false;
        const float aspect = static_cast<float>(surface->bitmap->width) / static_cast<float>(surface->bitmap->height);
        active.surface = surface;
        active.camera = camera;
        active.view_projection = camera.projection_matrix(aspect) * camera.view_matrix();
        return true;
    }

    void end_surface_3d()
    {
        if (!active.surface)
            return;
        active.surface = nullptr;
        end_render_target();
    }

    ShadowMap3D *create_shadow_map_3d(int size)
    {
        if (size < 64)
        {
            detail::set_error("Shadow map size must be at least 64");
            return nullptr;
        }
        Surface3D *surface = create_surface_3d(size, size, 1);
        if (!surface)
            return nullptr;
        ShadowMap3D *map = new ShadowMap3D;
        map->surface = surface;
        map->size = size;
        return map;
    }

    void destroy_shadow_map_3d(ShadowMap3D *map)
    {
        if (!map)
            return;
        if (current_shadow == map)
            current_shadow = nullptr;
        if (active.shadow_pass == map)
            end_shadow_pass_3d();
        destroy_surface_3d(map->surface);
        delete map;
    }

    bool begin_shadow_pass_3d(ShadowMap3D *map, const glm::vec3 &centre, float radius)
    {
        if (!map || !map->surface || radius <= 0.0f || active.surface)
        {
            detail::set_error("Invalid shadow pass state");
            return false;
        }
        const glm::vec3 direction = glm::normalize(current_light.direction);
        const glm::vec3 up = std::abs(direction.y) > 0.99f ? glm::vec3(1.0f, 0.0f, 0.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
        const glm::mat4 rotation = glm::lookAt(glm::vec3(0.0f), direction, up);
        // Snap the window to whole texels in light space so shadow edges do not shimmer as the focus moves.
        const float texel = 2.0f * radius / static_cast<float>(map->size);
        const glm::vec3 c = glm::vec3(rotation * glm::vec4(centre, 1.0f));
        const float sx = std::round(c.x / texel) * texel;
        const float sy = std::round(c.y / texel) * texel;
        const glm::mat4 projection = glm::ortho(-radius, radius, -radius, radius, 0.0f, radius * 4.0f);
        const glm::mat4 shift = glm::translate(glm::mat4(1.0f), glm::vec3(-sx, -sy, -(c.z + radius * 2.0f)));
        map->view_projection = projection * shift * rotation;
        map->texel_size = texel;
        if (!begin_render_target(map->surface->bitmap))
            return false;
        active.surface = map->surface;
        active.shadow_pass = map;
        active.camera = Camera{};
        active.view_projection = map->view_projection;
        // Packed depth of 1 (white) marks "nothing here".
        if (detail::Renderer *renderer = detail::active_renderer())
            renderer->clear_3d(1.0f, 1.0f, 1.0f, 1.0f);
        return true;
    }

    void end_shadow_pass_3d()
    {
        if (!active.shadow_pass)
            return;
        active.shadow_pass = nullptr;
        active.surface = nullptr;
        end_render_target();
    }

    void set_shadow_map_3d(const ShadowMap3D *map, float bias)
    {
        current_shadow = map;
        current_shadow_bias = bias;
    }

    const ShadowMap3D *get_shadow_map_3d(float *bias)
    {
        if (bias)
            *bias = current_shadow_bias;
        return current_shadow;
    }

    LocalShadowMap3D *create_local_shadow_map_3d(int face_size, int max_lights)
    {
        if (face_size < 64)
        {
            detail::set_error("Local shadow face size must be at least 64");
            return nullptr;
        }
        max_lights = std::clamp(max_lights, 1, max_local_shadow_lights_3d);
        Surface3D *face = create_surface_3d(face_size, face_size, 1);
        if (!face)
            return nullptr;
        Bitmap *atlas = create_render_target(face_size * 3, face_size * 2 * max_lights);
        if (!atlas)
        {
            destroy_surface_3d(face);
            return nullptr;
        }
        LocalShadowMap3D *map = new LocalShadowMap3D;
        map->face = face;
        map->atlas = atlas;
        map->face_size = face_size;
        map->max_lights = max_lights;
        std::fill(std::begin(map->light_slot), std::end(map->light_slot), -1);
        return map;
    }

    void destroy_local_shadow_map_3d(LocalShadowMap3D *map)
    {
        if (!map)
            return;
        if (current_local_shadow == map)
            current_local_shadow = nullptr;
        destroy_surface_3d(map->face);
        destroy_bitmap(map->atlas);
        delete map;
    }

    bool render_local_shadows_3d(LocalShadowMap3D *map, const std::function<void()> &draw_casters)
    {
        detail::Renderer *renderer = detail::active_renderer();
        if (!map || !map->face || !map->atlas || !draw_casters || !renderer || active.surface)
        {
            detail::set_error("Invalid local shadow pass state");
            return false;
        }
        map->slots_used = 0;
        std::fill(std::begin(map->light_slot), std::end(map->light_slot), -1);
        for (std::size_t i = 0; i < local_lights.size() && map->slots_used < map->max_lights; ++i)
            if (local_lights[i].cast_shadow)
                map->light_slot[i] = map->slots_used++;
        // Packed depth of 1 (white) marks "nothing here", including for faces that are never drawn.
        if (!begin_render_target(map->atlas))
            return false;
        clear_render_target(Colour{255, 255, 255, 255});
        end_render_target();

        const glm::vec3 axes[6] = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};
        for (std::size_t i = 0; i < local_lights.size(); ++i)
        {
            const int slot = map->light_slot[i];
            if (slot < 0)
                continue;
            const LocalLight3D &light = local_lights[i];
            const bool spot = light.outer_angle > 0.0f && glm::length(light.direction) > 0.0f;
            const float near_plane = std::max(0.05f, light.range * 0.004f);
            map->slot_faces[slot] = spot ? 1 : 6;
            map->slot_near[slot] = near_plane;
            map->slot_far[slot] = std::max(light.range, near_plane * 2.0f);
            for (int face = 0; face < map->slot_faces[slot]; ++face)
            {
                const glm::vec3 direction = spot ? glm::normalize(light.direction) : axes[face];
                const glm::vec3 up = std::abs(direction.y) > 0.99f ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f);
                const float fov = spot ? std::clamp(2.0f * light.outer_angle + 6.0f, 20.0f, 170.0f) : 94.0f;
                const glm::mat4 projection =
                    glm::perspective(glm::radians(fov), 1.0f, near_plane, map->slot_far[slot]);
                map->view_projection[slot][face] = projection * glm::lookAt(light.position, light.position + direction, up);

                if (!begin_render_target(map->face->bitmap))
                    return false;
                active.surface = map->face;
                active.local_shadow_pass = true;
                active.camera = Camera{};
                active.view_projection = map->view_projection[slot][face];
                renderer->clear_3d(1.0f, 1.0f, 1.0f, 1.0f);
                draw_casters();
                active.local_shadow_pass = false;
                active.surface = nullptr;
                end_render_target();

                if (!begin_render_target(map->atlas))
                    return false;
                draw_sprite(map->face->bitmap, static_cast<float>((face % 3) * map->face_size),
                            static_cast<float>((slot * 2 + face / 3) * map->face_size));
                end_render_target();
            }
        }
        return true;
    }

    void set_local_shadow_map_3d(const LocalShadowMap3D *map)
    {
        current_local_shadow = map;
    }

    const LocalShadowMap3D *get_local_shadow_map_3d()
    {
        return current_local_shadow;
    }

    void clear_surface_3d(Colour colour)
    {
        detail::Renderer *renderer = detail::active_renderer();
        if (!active.surface || !renderer)
            return;
        renderer->clear_3d(colour.red / 255.0f, colour.green / 255.0f, colour.blue / 255.0f,
                           colour.alpha / 255.0f);
    }

    bool clear_depth_3d()
    {
        detail::Renderer *renderer = detail::active_renderer();
        return active.surface && renderer && renderer->clear_depth_3d();
    }

    void set_camera_3d(const Camera &camera)
    {
        if (!active.surface || !active.surface->bitmap)
            return;
        const float aspect = static_cast<float>(active.surface->bitmap->width) / static_cast<float>(active.surface->bitmap->height);
        active.camera = camera;
        active.view_projection = camera.projection_matrix(aspect) * camera.view_matrix();
    }

    void set_light_3d(const Light3D &light)
    {
        current_light = light;
    }

    void set_local_lights_3d(const std::vector<LocalLight3D> &lights)
    {
        local_lights.assign(lights.begin(), lights.begin() + std::min<std::size_t>(lights.size(), max_local_lights_3d));
    }

    std::vector<LocalLight3D> get_local_lights_3d()
    {
        return local_lights;
    }

    namespace
    {
        RenderStyle3D g_render_style = RenderStyle3D::normal;
    }

    void set_render_style_3d(RenderStyle3D style)
    {
        g_render_style = style;
    }

    RenderStyle3D render_style_3d()
    {
        return g_render_style;
    }

    Light3D get_light_3d()
    {
        return current_light;
    }

    namespace detail
    {
        bool active_camera_info(ActiveCameraInfo &info)
        {
            if (!active.surface || !active.surface->bitmap)
                return false;
            info.camera = active.camera;
            info.width = active.surface->bitmap->width;
            info.height = active.surface->bitmap->height;
            return true;
        }
    } // namespace detail

    namespace
    {
        // Per thread so async loaders can retain data independently of the main thread.
        thread_local bool g_retain_mesh_cpu_data = false;
        thread_local std::vector<detail::DeferredMesh> *g_deferred_meshes = nullptr;
    }

    namespace detail
    {
        DeferredMeshScope::DeferredMeshScope(std::vector<DeferredMesh> &out) : previous_(g_deferred_meshes)
        {
            g_deferred_meshes = &out;
        }

        DeferredMeshScope::~DeferredMeshScope()
        {
            g_deferred_meshes = previous_;
        }

        bool upload_deferred_mesh(const DeferredMesh &deferred)
        {
            Renderer *renderer = active_renderer();
            Mesh *mesh = deferred.mesh;
            std::uint32_t handle = 0;
            if (!mesh || !renderer ||
                !renderer->create_mesh(mesh->vertices.data(), mesh->vertices.size(), mesh->indices.data(),
                                       mesh->indices.size(), handle))
            {
                set_error("Unable to create mesh on the active graphics backend");
                return false;
            }
            mesh->handle = handle;
            if (!deferred.retain_cpu)
            {
                std::vector<Vertex3D>().swap(mesh->vertices);
                std::vector<std::uint32_t>().swap(mesh->indices);
            }
            return true;
        }
    } // namespace detail

    bool mesh_cpu_data_retained()
    {
        return g_retain_mesh_cpu_data;
    }

    void set_mesh_cpu_data_retained(bool retained)
    {
        g_retain_mesh_cpu_data = retained;
    }

    Mesh *create_mesh(const std::vector<Vertex3D> &vertices, const std::vector<std::uint32_t> &indices)
    {
        if (vertices.empty() || indices.empty() || indices.size() % 3 != 0)
        {
            detail::set_error("Mesh needs vertices and a whole number of triangles");
            return nullptr;
        }
        for (const std::uint32_t index : indices)
        {
            if (index >= vertices.size())
            {
                detail::set_error("Mesh index is out of range");
                return nullptr;
            }
        }
        std::vector<Vertex3D> with_tangents;
        const std::vector<Vertex3D> *source = &vertices;
        if (!has_tangents(vertices))
        {
            with_tangents = vertices;
            generate_tangents(with_tangents, indices);
            source = &with_tangents;
        }
        if (g_deferred_meshes)
        {
            Mesh *mesh = new Mesh;
            mesh->vertex_count = static_cast<int>(vertices.size());
            mesh->index_count = static_cast<int>(indices.size());
            mesh->vertices = *source;
            mesh->indices = indices;
            g_deferred_meshes->push_back({mesh, g_retain_mesh_cpu_data});
            return mesh;
        }
        detail::Renderer *renderer = detail::active_renderer();
        std::uint32_t handle = 0;
        if (!renderer || !renderer->create_mesh(source->data(), source->size(), indices.data(),
                                                indices.size(), handle))
        {
            detail::set_error("Unable to create mesh on the active graphics backend");
            return nullptr;
        }
        Mesh *mesh = new Mesh;
        mesh->handle = handle;
        mesh->vertex_count = static_cast<int>(vertices.size());
        mesh->index_count = static_cast<int>(indices.size());
        if (g_retain_mesh_cpu_data)
        {
            mesh->vertices = *source;
            mesh->indices = indices;
        }
        return mesh;
    }

    bool update_mesh_vertices(Mesh *mesh, const std::vector<Vertex3D> &vertices)
    {
        detail::Renderer *renderer = detail::active_renderer();
        if (!mesh || !renderer || vertices.size() != static_cast<std::size_t>(mesh->vertex_count))
        {
            detail::set_error("Mesh update needs the same vertex count as the mesh");
            return false;
        }
        const std::vector<Vertex3D> *source = &vertices;
        std::vector<Vertex3D> with_tangents;
        if (!has_tangents(vertices) && mesh->indices.size() == static_cast<std::size_t>(mesh->index_count))
        {
            with_tangents = vertices;
            generate_tangents(with_tangents, mesh->indices);
            source = &with_tangents;
        }
        if (!renderer->update_mesh_vertices(mesh->handle, source->data(), source->size()))
        {
            detail::set_error("Unable to update mesh on the active graphics backend");
            return false;
        }
        return true;
    }

    void destroy_mesh(Mesh *mesh)
    {
        if (!mesh)
            return;
        if (mesh->handle != 0)
            if (detail::Renderer *renderer = detail::active_renderer())
                renderer->destroy_mesh(mesh->handle);
        delete mesh;
    }

    Mesh *create_cube_mesh(float size, Colour colour)
    {
        const float h = size * 0.5f;
        struct Face
        {
            glm::vec3 normal, u, v;
        };
        // u x v == normal, so each face is counter-clockwise seen from outside.
        const Face faces[6] = {
            {{0, 0, 1}, {1, 0, 0}, {0, 1, 0}},
            {{0, 0, -1}, {-1, 0, 0}, {0, 1, 0}},
            {{1, 0, 0}, {0, 0, -1}, {0, 1, 0}},
            {{-1, 0, 0}, {0, 0, 1}, {0, 1, 0}},
            {{0, 1, 0}, {1, 0, 0}, {0, 0, -1}},
            {{0, -1, 0}, {1, 0, 0}, {0, 0, 1}}};
        std::vector<Vertex3D> vertices;
        std::vector<std::uint32_t> indices;
        for (const Face &face : faces)
        {
            const std::uint32_t base = static_cast<std::uint32_t>(vertices.size());
            const glm::vec3 centre = face.normal * h;
            vertices.push_back(make_vertex(centre - (face.u + face.v) * h, face.normal, colour, 0.0f, 1.0f));
            vertices.push_back(make_vertex(centre + (face.u - face.v) * h, face.normal, colour, 1.0f, 1.0f));
            vertices.push_back(make_vertex(centre + (face.u + face.v) * h, face.normal, colour, 1.0f, 0.0f));
            vertices.push_back(make_vertex(centre + (face.v - face.u) * h, face.normal, colour, 0.0f, 0.0f));
            for (const std::uint32_t offset : {0u, 1u, 2u, 0u, 2u, 3u})
                indices.push_back(base + offset);
        }
        return create_mesh(vertices, indices);
    }

    Mesh *create_plane_mesh(float width, float depth, Colour colour)
    {
        const float hw = width * 0.5f;
        const float hd = depth * 0.5f;
        const glm::vec3 up{0.0f, 1.0f, 0.0f};
        const std::vector<Vertex3D> vertices = {
            make_vertex({-hw, 0.0f, hd}, up, colour, 0.0f, 1.0f), make_vertex({hw, 0.0f, hd}, up, colour, 1.0f, 1.0f),
            make_vertex({hw, 0.0f, -hd}, up, colour, 1.0f, 0.0f), make_vertex({-hw, 0.0f, -hd}, up, colour, 0.0f, 0.0f)};
        return create_mesh(vertices, {0, 1, 2, 0, 2, 3});
    }

    Mesh *create_sphere_mesh(float radius, int segments, int rings, Colour colour)
    {
        if (segments < 3 || rings < 2)
        {
            detail::set_error("Sphere needs at least 3 segments and 2 rings");
            return nullptr;
        }
        std::vector<Vertex3D> vertices;
        std::vector<std::uint32_t> indices;
        for (int ring = 0; ring <= rings; ++ring)
        {
            const float phi = glm::pi<float>() * static_cast<float>(ring) / static_cast<float>(rings);
            for (int segment = 0; segment <= segments; ++segment)
            {
                const float theta = 2.0f * glm::pi<float>() * static_cast<float>(segment) / static_cast<float>(segments);
                const glm::vec3 normal{std::sin(phi) * std::cos(theta), std::cos(phi), std::sin(phi) * std::sin(theta)};
                vertices.push_back(make_vertex(normal * radius, normal, colour,
                                                    static_cast<float>(segment) / static_cast<float>(segments),
                                                    static_cast<float>(ring) / static_cast<float>(rings)));
            }
        }
        const std::uint32_t stride = static_cast<std::uint32_t>(segments + 1);
        for (int ring = 0; ring < rings; ++ring)
        {
            for (int segment = 0; segment < segments; ++segment)
            {
                const std::uint32_t a = ring * stride + segment;
                const std::uint32_t b = a + stride;
                for (const std::uint32_t index : {a, a + 1, b, a + 1, b + 1, b})
                    indices.push_back(index);
            }
        }
        return create_mesh(vertices, indices);
    }

    // Shared by draw_mesh() and draw_mesh_instanced(). For instanced draws `model` is the identity, so the params carry
    // the view-projection and the shader applies each instance's own matrix.
    static bool submit_mesh_draw(const Mesh *mesh, const glm::mat4 &model, const Material &material,
                                 std::uint32_t instance_buffer, std::size_t instance_count)
    {
        detail::Renderer *renderer = detail::active_renderer();
        if (!mesh || !renderer || !active.surface)
        {
            detail::set_error("draw_mesh requires a mesh and an active 3D surface");
            return false;
        }
        if (instance_buffer != 0 && material.terrain.enabled)
        {
            detail::set_error("Terrain shading cannot be drawn with draw_mesh_instanced");
            return false;
        }
        const bool shadow_pass = active.shadow_pass != nullptr || active.local_shadow_pass;
        if (shadow_pass && (material.blend != MaterialBlend::opaque || !material.lit || material.terrain.enabled))
            return true;
        detail::MeshDrawParams params{};
        params.style = shadow_pass ? 0u : static_cast<std::uint32_t>(g_render_style);
        const glm::mat4 mvp = active.view_projection * model;
        for (int column = 0; column < 4; ++column)
            for (int row = 0; row < 4; ++row)
                params.mvp[column * 4 + row] = mvp[column][row];
        const glm::mat3 normal_matrix = glm::transpose(glm::inverse(glm::mat3(model)));
        for (int row = 0; row < 3; ++row)
            for (int column = 0; column < 3; ++column)
                params.normal_rows[row][column] = normal_matrix[column][row];
        if (material.texture)
        {
            if (!upload_bitmap(material.texture))
            {
                detail::set_error("Unable to upload mesh texture");
                return false;
            }
            params.texture = material.texture->gpu_texture;
        }
        Bitmap *normal_map = material.lit ? material.normal_map : nullptr;
        if (!normal_map)
            normal_map = flat_normal_bitmap();
        if (!normal_map || !upload_bitmap(normal_map))
        {
            detail::set_error("Unable to upload mesh normal map");
            return false;
        }
        params.normal_texture = normal_map->gpu_texture;
        params.cull = material.cull;
        if (material.blend != MaterialBlend::opaque && !material.terrain.enabled)
        {
            params.blend = material.blend == MaterialBlend::additive ? 2u : 1u;
            // The mesh shaders treat a positive emissive w as the opacity of a blended draw.
            params.emissive[3] = std::max(material.tint.alpha / 255.0f, 1.0e-4f);
        }
        params.normal_rows[0][3] = material.tint.red / 255.0f;
        params.normal_rows[1][3] = material.tint.green / 255.0f;
        params.normal_rows[2][3] = material.tint.blue / 255.0f;
        if (material.terrain.enabled)
        {
            // Terrain is only translated, so the normal matrix is not needed and its slots carry the terrain
            // parameters. A negative tint red marks the mode for the shaders.
            const TerrainShading &t = material.terrain;
            params.normal_rows[0][0] = t.rotation[0];
            params.normal_rows[0][1] = t.rotation[1];
            params.normal_rows[0][2] = t.rotation[2];
            params.normal_rows[0][3] = -1.0f;
            params.normal_rows[1][0] = t.eye[0];
            params.normal_rows[1][1] = t.eye[1];
            params.normal_rows[1][2] = t.eye[2];
            params.normal_rows[1][3] = t.morph_start;
            params.normal_rows[2][0] = t.morph_end;
            params.normal_rows[2][1] = t.fade_start;
            params.normal_rows[2][2] = t.fade_end;
            params.normal_rows[2][3] = t.rotation[3];
            if (const TerrainTextures *textures = t.textures; textures && textures->albedo_array != 0)
            {
                params.terrain_albedo = textures->albedo_array;
                params.terrain_normals = textures->normal_array;
                for (int i = 0; i < TerrainTextures::max_layers; ++i)
                {
                    params.terrain_layers[i][0] = static_cast<float>(textures->first[i]);
                    params.terrain_layers[i][1] = static_cast<float>(std::max(textures->variants[i], 1));
                    params.terrain_layers[i][2] = std::max(textures->tile_metres[i], 1.0e-3f);
                    params.terrain_layers[i][3] = static_cast<float>(std::max(textures->tiles_per_period[i], 1));
                    for (int c = 0; c < 3; ++c)
                        params.terrain_average[i][c] = textures->average[i][c];
                    params.terrain_average[i][3] = textures->specular[i];
                }
                params.terrain_macro[0] = static_cast<float>(textures->macro_layer);
                params.terrain_macro[1] = std::max(textures->macro_tile_metres, 1.0e-3f);
                params.terrain_macro[2] = textures->macro_strength;
            }
        }
        if (material.lit)
        {
            const glm::vec3 to_light = -glm::normalize(current_light.direction);
            params.light[0] = to_light.x;
            params.light[1] = to_light.y;
            params.light[2] = to_light.z;
            params.light[3] = current_light.ambient;
            params.light_colour[0] = current_light.colour.x;
            params.light_colour[1] = current_light.colour.y;
            params.light_colour[2] = current_light.colour.z;
        }
        else
        {
            params.light[3] = 1.0f;
        }
        params.light_colour[3] = material.alpha_cutoff;
        if (shadow_pass)
        {
            params.shadow[0] = 2.0f;
            params.cull = CullMode::none;
            const bool drawn = instance_buffer != 0
                                   ? renderer->draw_mesh_instanced(mesh->handle, params, instance_buffer, instance_count)
                                   : renderer->draw_mesh(mesh->handle, params);
            if (!drawn)
                detail::set_error("Backend failed to draw the shadow caster");
            return drawn;
        }
        detail::apply_environment(params, material.fog);
        const bool want_pbr = material.pbr && material.lit;
        const bool want_locals = material.lit && !local_lights.empty();
        const bool want_shadow = material.lit && current_shadow && current_shadow->surface;
        const bool want_local_shadow = want_locals && current_local_shadow && current_local_shadow->slots_used > 0;
        // The flat style rebuilds face normals from the camera rays, so it needs them too.
        const bool have_camera = (want_pbr || want_locals || want_shadow || params.style != 0) && detail::apply_pbr_camera(params);
        if (want_shadow && have_camera)
        {
            params.shadow[0] = 1.0f;
            params.shadow[1] = current_shadow_bias;
            params.shadow[2] = current_shadow->texel_size;
            params.shadow_texture = current_shadow->surface->bitmap->gpu_texture;
            for (int column = 0; column < 4; ++column)
                for (int row = 0; row < 4; ++row)
                    params.shadow_matrix[column * 4 + row] = current_shadow->view_projection[column][row];
            params.camera_position[0] = active.camera.position.x;
            params.camera_position[1] = active.camera.position.y;
            params.camera_position[2] = active.camera.position.z;
        }
        if (want_locals && have_camera)
        {
            params.camera_position[0] = active.camera.position.x;
            params.camera_position[1] = active.camera.position.y;
            params.camera_position[2] = active.camera.position.z;
            params.camera_position[0] = active.camera.position.x;
            params.camera_position[1] = active.camera.position.y;
            params.camera_position[2] = active.camera.position.z;
            params.camera_position[3] = static_cast<float>(local_lights.size());
            for (std::size_t i = 0; i < local_lights.size(); ++i)
            {
                const LocalLight3D &l = local_lights[i];
                float(*packed)[4] = &params.local_lights[i * 4];
                const float scale = std::max(l.intensity, 0.0f);
                packed[0][0] = l.position.x;
                packed[0][1] = l.position.y;
                packed[0][2] = l.position.z;
                packed[0][3] = std::max(l.range, 1.0e-3f);
                packed[1][0] = l.colour.x * scale;
                packed[1][1] = l.colour.y * scale;
                packed[1][2] = l.colour.z * scale;
                packed[1][3] = -1.0f;
                packed[2][0] = packed[2][1] = packed[2][2] = packed[2][3] = 0.0f;
                packed[3][0] = -1.0f;
                packed[3][1] = packed[3][2] = packed[3][3] = 0.0f;
                if (want_local_shadow && current_local_shadow->light_slot[i] >= 0)
                {
                    const int slot = current_local_shadow->light_slot[i];
                    packed[3][0] = static_cast<float>(slot);
                    packed[3][1] = current_local_shadow->slot_near[slot];
                    packed[3][2] = current_local_shadow->slot_far[slot];
                }
                if (l.outer_angle > 0.0f && glm::length(l.direction) > 0.0f)
                {
                    const float outer = std::cos(glm::radians(std::min(l.outer_angle, 89.0f)));
                    const float inner = std::cos(glm::radians(std::clamp(l.inner_angle, 0.0f, l.outer_angle)));
                    const glm::vec3 d = glm::normalize(l.direction);
                    packed[1][3] = outer;
                    packed[2][0] = d.x;
                    packed[2][1] = d.y;
                    packed[2][2] = d.z;
                    packed[2][3] = 1.0f / std::max(inner - outer, 1.0e-3f);
                }
            }
        }
        if (want_local_shadow && have_camera)
        {
            const LocalShadowMap3D &map = *current_local_shadow;
            params.local_shadow_texture = map.atlas->gpu_texture;
            params.shadow[3] = static_cast<float>(map.slots_used);
            for (int slot = 0; slot < map.slots_used; ++slot)
                for (int face = 0; face < map.slot_faces[slot]; ++face)
                {
                    float *destination = &params.local_lights[detail::local_shadow_matrix_base + (slot * 6 + face) * 4][0];
                    for (int column = 0; column < 4; ++column)
                        for (int row = 0; row < 4; ++row)
                            destination[column * 4 + row] = map.view_projection[slot][face][column][row];
                }
        }
        if (want_pbr && have_camera)
        {
            if (material.metallic_roughness)
            {
                if (!upload_bitmap(material.metallic_roughness))
                {
                    detail::set_error("Unable to upload mesh metallic-roughness texture");
                    return false;
                }
                params.material_texture = material.metallic_roughness->gpu_texture;
            }
            params.pbr[0] = 1.0f;
            params.pbr[1] = std::clamp(material.metallic, 0.0f, 1.0f);
            params.pbr[2] = std::clamp(material.roughness, 0.0f, 1.0f);
            params.pbr[3] = std::clamp(material.occlusion_strength, 0.0f, 1.0f);
            for (int i = 0; i < 3; ++i)
                params.emissive[i] = std::max(material.emissive[i], 0.0f);
        }
        const bool drawn = instance_buffer != 0
                               ? renderer->draw_mesh_instanced(mesh->handle, params, instance_buffer, instance_count)
                               : renderer->draw_mesh(mesh->handle, params);
        if (!drawn)
        {
            detail::set_error("Unable to draw mesh on the active graphics backend");
            return false;
        }
        return true;
    }

    bool draw_mesh(const Mesh *mesh, const glm::mat4 &model, const Material &material)
    {
        return submit_mesh_draw(mesh, model, material, 0, 0);
    }

    namespace
    {
        constexpr std::size_t min_transient_capacity = 256;

        // Buffers for the one-shot vector overload; each is used by one draw per frame, then reused.
        struct TransientInstanceBuffer
        {
            InstanceBuffer *buffer = nullptr;
            bool in_use = false;
        };
        std::vector<TransientInstanceBuffer> transient_buffers;

        std::vector<detail::InstanceGpu> pack_instances(const std::vector<InstanceData3D> &instances)
        {
            std::vector<detail::InstanceGpu> packed(instances.size());
            for (std::size_t index = 0; index < instances.size(); ++index)
            {
                const InstanceData3D &instance = instances[index];
                for (int column = 0; column < 4; ++column)
                    for (int row = 0; row < 4; ++row)
                        packed[index].model[column * 4 + row] = instance.model[column][row];
                packed[index].tint[0] = instance.tint.red / 255.0f;
                packed[index].tint[1] = instance.tint.green / 255.0f;
                packed[index].tint[2] = instance.tint.blue / 255.0f;
                packed[index].tint[3] = instance.tint.alpha / 255.0f;
            }
            return packed;
        }
    } // namespace

    InstanceBuffer *create_instance_buffer(std::size_t capacity)
    {
        detail::Renderer *renderer = detail::active_renderer();
        std::uint32_t handle = 0;
        if (capacity == 0 || !renderer || !renderer->create_instance_buffer(capacity, handle))
        {
            detail::set_error("Instance buffers are not supported by the active graphics backend");
            return nullptr;
        }
        InstanceBuffer *buffer = new InstanceBuffer;
        buffer->handle = handle;
        buffer->capacity = capacity;
        return buffer;
    }

    bool update_instance_buffer(InstanceBuffer *buffer, const std::vector<InstanceData3D> &instances)
    {
        detail::Renderer *renderer = detail::active_renderer();
        if (!buffer || !renderer)
        {
            detail::set_error("update_instance_buffer requires an instance buffer");
            return false;
        }
        if (instances.size() > buffer->capacity)
        {
            detail::set_error("Instance buffer is too small for the instances given");
            return false;
        }
        const std::vector<detail::InstanceGpu> packed = pack_instances(instances);
        if (!renderer->update_instance_buffer(buffer->handle, packed.data(), packed.size()))
        {
            detail::set_error("Unable to update the instance buffer");
            return false;
        }
        buffer->count = packed.size();
        return true;
    }

    void destroy_instance_buffer(InstanceBuffer *buffer)
    {
        if (!buffer)
            return;
        for (TransientInstanceBuffer &transient : transient_buffers)
            if (transient.buffer == buffer)
                transient.buffer = nullptr;
        if (detail::Renderer *renderer = detail::active_renderer())
            renderer->destroy_instance_buffer(buffer->handle);
        delete buffer;
    }

    bool draw_mesh_instanced(const Mesh *mesh, const InstanceBuffer *buffer, const Material &material)
    {
        if (!buffer)
        {
            detail::set_error("draw_mesh_instanced requires an instance buffer");
            return false;
        }
        if (buffer->count == 0)
            return true;
        return submit_mesh_draw(mesh, glm::mat4(1.0f), material, buffer->handle, buffer->count);
    }

    void RenderQueue3D::draw_mesh(const Mesh *mesh, const glm::mat4 &model, const Material &material)
    {
        entries_.push_back({mesh, model, material, nullptr});
    }

    void RenderQueue3D::draw_mesh_instanced(const Mesh *mesh, const InstanceBuffer *buffer, const Material &material)
    {
        entries_.push_back({mesh, glm::mat4(1.0f), material, buffer});
    }

    bool submit_render_queue(const RenderQueue3D &queue)
    {
        bool ok = true;
        for (const RenderQueue3D::Entry &entry : queue.entries_)
            ok = (entry.instances ? sl::draw_mesh_instanced(entry.mesh, entry.instances, entry.material)
                                  : sl::draw_mesh(entry.mesh, entry.model, entry.material)) &&
                 ok;
        return ok;
    }

    bool draw_mesh_instanced(const Mesh *mesh, const std::vector<InstanceData3D> &instances, const Material &material)
    {
        if (instances.empty())
            return true;
        TransientInstanceBuffer *slot = nullptr;
        for (TransientInstanceBuffer &candidate : transient_buffers)
            if (candidate.buffer && !candidate.in_use && candidate.buffer->capacity >= instances.size())
            {
                slot = &candidate;
                break;
            }
        if (!slot)
        {
            std::size_t capacity = min_transient_capacity;
            while (capacity < instances.size())
                capacity *= 2;
            InstanceBuffer *created = create_instance_buffer(capacity);
            if (!created)
                return false;
            transient_buffers.push_back({created, false});
            slot = &transient_buffers.back();
        }
        slot->in_use = true;
        if (!update_instance_buffer(slot->buffer, instances))
            return false;
        return draw_mesh_instanced(mesh, slot->buffer, material);
    }

    namespace detail
    {
        void recycle_transient_instance_buffers()
        {
            for (TransientInstanceBuffer &transient : transient_buffers)
                transient.in_use = false;
        }

        void release_transient_instance_buffers()
        {
            std::vector<TransientInstanceBuffer> buffers;
            buffers.swap(transient_buffers);
            for (TransientInstanceBuffer &transient : buffers)
                destroy_instance_buffer(transient.buffer);
        }
    } // namespace detail

} // namespace sl
