#include "model3d.h"

#include "error.h"

#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <map>
#include <sstream>
#include <unordered_map>

#include <glm/gtc/matrix_transform.hpp>

namespace sl
{
    namespace
    {
        struct AcMaterial
        {
            std::string name;
            Colour diffuse{255, 255, 255, 255};
        };

        struct AcSurface
        {
            unsigned int flags = 0;
            int material = 0;
            std::vector<std::pair<int, glm::vec2>> refs;
        };

        struct AcObject
        {
            std::string type;
            std::string name;
            std::string texture;
            glm::vec2 texture_repeat{1.0f};
            glm::vec2 texture_offset{0.0f};
            glm::mat4 transform{1.0f};
            std::vector<glm::vec3> vertices;
            std::vector<AcSurface> surfaces;
            std::vector<AcObject> children;
        };

        std::string quoted_value(const std::string &line)
        {
            const std::size_t first = line.find('"');
            if (first == std::string::npos)
                return {};
            const std::size_t last = line.find('"', first + 1);
            return last == std::string::npos ? std::string{} : line.substr(first + 1, last - first - 1);
        }

        bool parse_vec2(const std::string &line, glm::vec2 &value)
        {
            std::istringstream stream(line);
            std::string key;
            return static_cast<bool>(stream >> key >> value.x >> value.y) &&
                   std::isfinite(value.x) && std::isfinite(value.y);
        }

        bool parse_vec3(const std::string &line, glm::vec3 &value)
        {
            std::istringstream stream(line);
            std::string key;
            return static_cast<bool>(stream >> key >> value.x >> value.y >> value.z) &&
                   std::isfinite(value.x) && std::isfinite(value.y) && std::isfinite(value.z);
        }

        bool parse_object(const std::vector<std::string> &lines, std::size_t &index, AcObject &object,
                          std::string &error)
        {
            if (index >= lines.size() || lines[index].rfind("OBJECT ", 0) != 0)
            {
                error = "Expected AC3D OBJECT";
                return false;
            }
            object.type = lines[index++].substr(7);
            while (index < lines.size())
            {
                const std::string line = lines[index++];
                if (line.rfind("name ", 0) == 0)
                    object.name = quoted_value(line);
                else if (line.rfind("texture ", 0) == 0)
                    object.texture = quoted_value(line);
                else if (line.rfind("texrep ", 0) == 0)
                {
                    if (!parse_vec2(line, object.texture_repeat))
                    {
                        error = "Invalid AC3D texrep";
                        return false;
                    }
                }
                else if (line.rfind("texoff ", 0) == 0)
                {
                    if (!parse_vec2(line, object.texture_offset))
                    {
                        error = "Invalid AC3D texoff";
                        return false;
                    }
                }
                else if (line.rfind("loc ", 0) == 0)
                {
                    glm::vec3 location;
                    if (!parse_vec3(line, location))
                    {
                        error = "Invalid AC3D loc";
                        return false;
                    }
                    object.transform = glm::translate(glm::mat4(1.0f), location) * object.transform;
                }
                else if (line.rfind("rot ", 0) == 0)
                {
                    std::istringstream stream(line.substr(4));
                    glm::mat3 rotation(1.0f);
                    for (int row = 0; row < 3; ++row)
                        for (int column = 0; column < 3; ++column)
                            if (!(stream >> rotation[column][row]) || !std::isfinite(rotation[column][row]))
                            {
                                error = "Invalid AC3D rotation matrix";
                                return false;
                            }
                    object.transform = object.transform * glm::mat4(rotation);
                }
                else if (line.rfind("numvert ", 0) == 0)
                {
                    std::istringstream count_stream(line.substr(8));
                    std::size_t count = 0;
                    if (!(count_stream >> count) || count > 10'000'000 || count > lines.size() - index)
                    {
                        error = "Invalid AC3D vertex count";
                        return false;
                    }
                    object.vertices.reserve(count);
                    for (std::size_t i = 0; i < count; ++i)
                    {
                        glm::vec3 vertex;
                        std::istringstream vertex_stream(lines[index++]);
                        if (!(vertex_stream >> vertex.x >> vertex.y >> vertex.z) ||
                            !std::isfinite(vertex.x) || !std::isfinite(vertex.y) || !std::isfinite(vertex.z))
                        {
                            error = "Invalid AC3D vertex";
                            return false;
                        }
                        object.vertices.push_back(vertex);
                    }
                }
                else if (line.rfind("numsurf ", 0) == 0)
                {
                    std::istringstream count_stream(line.substr(8));
                    std::size_t count = 0;
                    if (!(count_stream >> count) || count > 10'000'000)
                    {
                        error = "Invalid AC3D surface count";
                        return false;
                    }
                    object.surfaces.reserve(object.surfaces.size() + count);
                    for (std::size_t i = 0; i < count; ++i)
                    {
                        if (index >= lines.size() || lines[index].rfind("SURF ", 0) != 0)
                        {
                            error = "Expected AC3D SURF";
                            return false;
                        }
                        AcSurface surface;
                        try
                        {
                            surface.flags = static_cast<unsigned int>(std::stoul(lines[index++].substr(5), nullptr, 0));
                        }
                        catch (const std::exception &)
                        {
                            error = "Invalid AC3D surface flags";
                            return false;
                        }
                        if (index >= lines.size() || lines[index].rfind("mat ", 0) != 0)
                        {
                            error = "Expected AC3D surface material";
                            return false;
                        }
                        {
                            std::istringstream stream(lines[index++].substr(4));
                            if (!(stream >> surface.material) || surface.material < 0)
                            {
                                error = "Invalid AC3D surface material";
                                return false;
                            }
                        }
                        if (index >= lines.size() || lines[index].rfind("refs ", 0) != 0)
                        {
                            error = "Expected AC3D surface refs";
                            return false;
                        }
                        std::size_t ref_count = 0;
                        {
                            std::istringstream stream(lines[index++].substr(5));
                            if (!(stream >> ref_count) || ref_count > object.vertices.size() ||
                                ref_count > lines.size() - index)
                            {
                                error = "Invalid AC3D surface reference count";
                                return false;
                            }
                        }
                        surface.refs.reserve(ref_count);
                        for (std::size_t ref = 0; ref < ref_count; ++ref)
                        {
                            std::istringstream stream(lines[index++]);
                            int vertex = -1;
                            glm::vec2 uv;
                            if (!(stream >> vertex >> uv.x >> uv.y) || vertex < 0 ||
                                static_cast<std::size_t>(vertex) >= object.vertices.size() ||
                                !std::isfinite(uv.x) || !std::isfinite(uv.y))
                            {
                                error = "Invalid AC3D surface reference";
                                return false;
                            }
                            surface.refs.push_back({vertex, uv});
                        }
                        object.surfaces.push_back(std::move(surface));
                    }
                }
                else if (line.rfind("kids ", 0) == 0)
                {
                    std::istringstream count_stream(line.substr(5));
                    std::size_t count = 0;
                    if (!(count_stream >> count) || count > lines.size() - index)
                    {
                        error = "Invalid AC3D child count";
                        return false;
                    }
                    object.children.reserve(count);
                    for (std::size_t i = 0; i < count; ++i)
                    {
                        object.children.emplace_back();
                        if (!parse_object(lines, index, object.children.back(), error))
                            return false;
                    }
                    return true;
                }
                else if (line.rfind("data ", 0) == 0)
                {
                    std::istringstream stream(line.substr(5));
                    std::size_t bytes = 0;
                    if (!(stream >> bytes))
                    {
                        error = "Invalid AC3D data length";
                        return false;
                    }
                    while (bytes > 0 && index < lines.size())
                    {
                        const std::size_t consumed = lines[index++].size() + 1;
                        bytes = consumed >= bytes ? 0 : bytes - consumed;
                    }
                    if (bytes != 0)
                    {
                        error = "Truncated AC3D object data";
                        return false;
                    }
                }
            }
            error = "AC3D object is missing its kids count";
            return false;
        }

        bool parse_material(const std::string &line, AcMaterial &material)
        {
            std::istringstream stream(line);
            std::string keyword, key;
            float diffuse[3] = {1.0f, 1.0f, 1.0f};
            float transparency = 0.0f;
            if (!(stream >> keyword >> std::quoted(material.name)))
                return false;
            while (stream >> key)
            {
                float values[3];
                if (key == "rgb")
                {
                    if (!(stream >> diffuse[0] >> diffuse[1] >> diffuse[2]))
                        return false;
                }
                else if (key == "trans")
                {
                    if (!(stream >> transparency))
                        return false;
                }
                else if (key == "shi")
                {
                    float shininess;
                    if (!(stream >> shininess))
                        return false;
                }
                else
                {
                    if (!(stream >> values[0] >> values[1] >> values[2]))
                        return false;
                }
            }
            auto channel = [](float value) {
                return static_cast<std::uint8_t>(std::lround(std::clamp(value, 0.0f, 1.0f) * 255.0f));
            };
            if (!std::isfinite(diffuse[0]) || !std::isfinite(diffuse[1]) || !std::isfinite(diffuse[2]) ||
                !std::isfinite(transparency))
                return false;
            material.diffuse = {channel(diffuse[0]), channel(diffuse[1]), channel(diffuse[2]),
                                channel(1.0f - std::clamp(transparency, 0.0f, 1.0f))};
            return true;
        }

        struct SurfaceBucket
        {
            std::vector<Vertex3D> vertices;
            std::vector<std::uint32_t> indices;
            std::vector<glm::vec3> normal_sums;
            std::unordered_map<int, std::vector<std::uint32_t>> smooth_vertices;
            bool two_sided = false;
        };

        bool append_object(Model &model, const AcObject &object, const glm::mat4 &parent,
                           const std::vector<AcMaterial> &materials,
                           const std::filesystem::path &directory,
                           std::map<std::string, Bitmap *> &texture_cache)
        {
            const glm::mat4 transform = parent * object.transform;
            std::map<int, SurfaceBucket> buckets;
            for (const AcSurface &surface : object.surfaces)
            {
                if (surface.refs.size() < 3)
                    continue;
                if (surface.material >= static_cast<int>(materials.size()) && !materials.empty())
                {
                    detail::set_error("AC3D surface references a missing material");
                    return false;
                }
                SurfaceBucket &bucket = buckets[surface.material];
                bucket.two_sided = bucket.two_sided || (surface.flags & 0x10u) != 0;
                const bool smooth = (surface.flags & 0x20u) != 0;
                std::vector<std::uint32_t> face;
                face.reserve(surface.refs.size());
                for (const auto &ref : surface.refs)
                {
                    Vertex3D vertex;
                    const glm::vec3 &position = object.vertices[static_cast<std::size_t>(ref.first)];
                    vertex.position[0] = position.x;
                    vertex.position[1] = position.y;
                    vertex.position[2] = position.z;
                    const glm::vec2 uv = ref.second * object.texture_repeat + object.texture_offset;
                    vertex.uv[0] = uv.x;
                    vertex.uv[1] = 1.0f - uv.y;
                    const std::uint32_t next = static_cast<std::uint32_t>(bucket.vertices.size());
                    bucket.vertices.push_back(vertex);
                    bucket.normal_sums.emplace_back(0.0f);
                    face.push_back(next);
                    if (smooth)
                        bucket.smooth_vertices[ref.first].push_back(next);
                }
                glm::vec3 flat_normal(0.0f);
                for (std::size_t i = 1; i + 1 < face.size(); ++i)
                {
                    const glm::vec3 p0(object.vertices[static_cast<std::size_t>(surface.refs[0].first)]);
                    const glm::vec3 p1(object.vertices[static_cast<std::size_t>(surface.refs[i].first)]);
                    const glm::vec3 p2(object.vertices[static_cast<std::size_t>(surface.refs[i + 1].first)]);
                    const glm::vec3 area_normal = glm::cross(p1 - p0, p2 - p0);
                    if (glm::dot(area_normal, area_normal) <= 1.0e-20f)
                        continue;
                    bucket.indices.insert(bucket.indices.end(), {face[0], face[i], face[i + 1]});
                    if (glm::dot(flat_normal, flat_normal) <= 1.0e-20f)
                        flat_normal = glm::normalize(area_normal);
                    if (smooth)
                        for (std::size_t corner : {std::size_t(0), i, i + 1})
                            bucket.normal_sums[face[corner]] += area_normal;
                }
                if (!smooth && glm::dot(flat_normal, flat_normal) > 1.0e-20f)
                    for (std::uint32_t vertex : face)
                        bucket.normal_sums[vertex] = flat_normal;
            }

            for (auto &entry : buckets)
            {
                SurfaceBucket &bucket = entry.second;
                if (bucket.indices.empty())
                    continue;
                for (auto &normal_entry : bucket.smooth_vertices)
                {
                    glm::vec3 sum(0.0f);
                    for (std::uint32_t index : normal_entry.second)
                        sum += bucket.normal_sums[index];
                    const glm::vec3 normal = glm::dot(sum, sum) > 1.0e-20f ? glm::normalize(sum) : glm::vec3(0, 1, 0);
                    for (std::uint32_t index : normal_entry.second)
                        bucket.normal_sums[index] = normal;
                }
                for (std::size_t i = 0; i < bucket.vertices.size(); ++i)
                {
                    const glm::vec3 normal = bucket.normal_sums[i];
                    for (int axis = 0; axis < 3; ++axis)
                        bucket.vertices[i].normal[axis] = normal[axis];
                }
                Mesh *mesh = create_mesh(bucket.vertices, bucket.indices);
                if (!mesh)
                {
                    detail::set_error("Unable to create AC3D mesh: " + object.name);
                    return false;
                }
                model.meshes.push_back(mesh);
                ModelPart part;
                part.name = object.name;
                part.mesh = mesh;
                part.transform = transform;
                for (const Vertex3D &vertex : bucket.vertices)
                {
                    const glm::vec3 world = glm::vec3(transform * glm::vec4(vertex.position[0], vertex.position[1],
                                                                            vertex.position[2], 1.0f));
                    part.bounds_min = part.has_bounds ? glm::min(part.bounds_min, world) : world;
                    part.bounds_max = part.has_bounds ? glm::max(part.bounds_max, world) : world;
                    part.has_bounds = true;
                }
                part.material.cull = bucket.two_sided ? CullMode::none : CullMode::back;
                if (!materials.empty() && entry.first >= 0 && entry.first < static_cast<int>(materials.size()))
                    part.material.tint = materials[static_cast<std::size_t>(entry.first)].diffuse;
                if (!object.texture.empty())
                {
                    std::string texture_name = object.texture;
                    std::replace(texture_name.begin(), texture_name.end(), '\\', '/');
                    const std::string key = (directory / texture_name).lexically_normal().string();
                    auto found = texture_cache.find(key);
                    if (found == texture_cache.end())
                    {
                        Bitmap *bitmap = load_bitmap(key);
                        if (bitmap)
                        {
                            bitmap->mipmaps = true;
                            bitmap->anisotropy = 16.0f;
                            model.textures.push_back(bitmap);
                        }
                        found = texture_cache.emplace(key, bitmap).first;
                    }
                    part.material.texture = found->second;
                }
                model.parts.push_back(std::move(part));
            }
            for (const AcObject &child : object.children)
                if (!append_object(model, child, transform, materials, directory, texture_cache))
                    return false;
            return true;
        }
    }

    Model *load_ac3d_model(const std::string &path)
    {
        std::ifstream file(path);
        if (!file)
        {
            detail::set_error("Unable to open AC3D model: " + path);
            return nullptr;
        }
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.back() == '\r')
                line.pop_back();
            lines.push_back(std::move(line));
        }
        std::size_t index = 0;
        while (index < lines.size() && lines[index].empty())
            ++index;
        if (index >= lines.size() || (lines[index] != "AC3Db" && lines[index] != "AC3Dc"))
        {
            detail::set_error("Invalid AC3D header: " + path);
            return nullptr;
        }
        ++index;
        std::vector<AcMaterial> materials;
        while (index < lines.size() && lines[index].rfind("MATERIAL ", 0) == 0)
        {
            AcMaterial material;
            if (!parse_material(lines[index++], material))
            {
                detail::set_error("Invalid AC3D material declaration");
                return nullptr;
            }
            materials.push_back(std::move(material));
        }
        if (index >= lines.size())
        {
            detail::set_error("AC3D file contains no root object");
            return nullptr;
        }
        AcObject root;
        std::string error;
        if (!parse_object(lines, index, root, error))
        {
            detail::set_error(error + ": " + path);
            return nullptr;
        }
        Model *model = new Model;
        std::map<std::string, Bitmap *> texture_cache;
        if (!append_object(*model, root, glm::mat4(1.0f), materials,
                           std::filesystem::path(path).parent_path(), texture_cache))
        {
            destroy_model(model);
            return nullptr;
        }
        if (model->parts.empty())
        {
            detail::set_error("AC3D file contains no drawable polygon surfaces: " + path);
            destroy_model(model);
            return nullptr;
        }
        return model;
    }
} // namespace sl
