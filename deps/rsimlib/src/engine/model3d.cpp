#include "model3d.h"

#include "error.h"
#include "json.hpp"

#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <unordered_map>

namespace sl
{
    namespace
    {
        using json = nlohmann::json;

        Vertex3D make_vertex(const glm::vec3 &position, const glm::vec3 &normal, Colour colour,
                             float u, float v)
        {
            Vertex3D vertex;
            for (int i = 0; i < 3; ++i)
            {
                vertex.position[i] = position[i];
                vertex.normal[i] = normal[i];
            }
            vertex.colour[0] = colour.red / 255.0f;
            vertex.colour[1] = colour.green / 255.0f;
            vertex.colour[2] = colour.blue / 255.0f;
            vertex.colour[3] = colour.alpha / 255.0f;
            vertex.uv[0] = u;
            vertex.uv[1] = v;
            return vertex;
        }

        glm::vec3 position_of(const Vertex3D &vertex)
        {
            return {vertex.position[0], vertex.position[1], vertex.position[2]};
        }

        std::uint8_t to_byte(double value)
        {
            return static_cast<std::uint8_t>(std::lround(std::clamp(value, 0.0, 1.0) * 255.0));
        }

        bool read_file(const std::filesystem::path &path, std::vector<std::uint8_t> &out)
        {
            std::ifstream file(path, std::ios::binary);
            if (!file)
                return false;
            out.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            return true;
        }

        // Fill in normals for vertices that lack one using area-weighted face normals.
        void generate_missing_normals(std::vector<Vertex3D> &vertices, const std::vector<std::uint32_t> &indices,
                                      const std::vector<bool> &has_normal)
        {
            if (std::find(has_normal.begin(), has_normal.end(), false) == has_normal.end())
                return;
            std::vector<glm::vec3> accumulated(vertices.size(), glm::vec3(0.0f));
            for (std::size_t i = 0; i + 2 < indices.size(); i += 3)
            {
                const glm::vec3 a = position_of(vertices[indices[i]]);
                const glm::vec3 b = position_of(vertices[indices[i + 1]]);
                const glm::vec3 c = position_of(vertices[indices[i + 2]]);
                const glm::vec3 face_normal = glm::cross(b - a, c - a);
                for (int k = 0; k < 3; ++k)
                    accumulated[indices[i + k]] += face_normal;
            }
            for (std::size_t i = 0; i < vertices.size(); ++i)
            {
                if (has_normal[i] || glm::dot(accumulated[i], accumulated[i]) == 0.0f)
                    continue;
                const glm::vec3 n = glm::normalize(accumulated[i]);
                for (int k = 0; k < 3; ++k)
                    vertices[i].normal[k] = n[k];
            }
        }

        // Upload geometry and register the mesh with the model so it is released with it.
        Mesh *add_mesh(Model &model, const std::vector<Vertex3D> &vertices,
                       const std::vector<std::uint32_t> &indices)
        {
            Mesh *mesh = create_mesh(vertices, indices);
            if (mesh)
                model.meshes.push_back(mesh);
            return mesh;
        }

        Bitmap *add_texture(Model &model, Bitmap *bitmap)
        {
            if (bitmap)
                model.textures.push_back(bitmap);
            return bitmap;
        }

        // ---- Wavefront OBJ / MTL ----

        struct ObjGroup
        {
            std::string material;
            std::vector<Vertex3D> vertices;
            std::vector<std::uint32_t> indices;
            std::vector<bool> has_normal;
            std::unordered_map<std::string, std::uint32_t> corners;
        };

        struct ObjData
        {
            std::vector<ObjGroup> groups;
            std::string material_library;
        };

        bool parse_obj(const char *data, std::size_t size, Colour colour, ObjData &result)
        {
            if (!data || size == 0)
            {
                detail::set_error("OBJ data is empty");
                return false;
            }
            std::vector<glm::vec3> positions, normals;
            std::vector<glm::vec2> uvs;
            std::size_t current_index = 0;
            auto select_group = [&](const std::string &material)
            {
                for (std::size_t i = 0; i < result.groups.size(); ++i)
                {
                    if (result.groups[i].material == material)
                    {
                        current_index = i;
                        return;
                    }
                }
                result.groups.emplace_back();
                result.groups.back().material = material;
                current_index = result.groups.size() - 1;
            };
            select_group("");

            auto resolve = [](long index, std::size_t count) -> long
            {
                return index < 0 ? static_cast<long>(count) + index : index - 1;
            };

            std::istringstream stream(std::string(data, size));
            std::string line;
            while (std::getline(stream, line))
            {
                std::istringstream words(line);
                std::string keyword;
                if (!(words >> keyword))
                    continue;
                if (keyword == "v")
                {
                    glm::vec3 p;
                    if (words >> p.x >> p.y >> p.z)
                        positions.push_back(p);
                }
                else if (keyword == "vn")
                {
                    glm::vec3 n;
                    if (words >> n.x >> n.y >> n.z)
                        normals.push_back(n);
                }
                else if (keyword == "vt")
                {
                    glm::vec2 t;
                    if (words >> t.x >> t.y)
                        uvs.push_back(t);
                }
                else if (keyword == "mtllib")
                {
                    std::getline(words >> std::ws, result.material_library);
                }
                else if (keyword == "usemtl")
                {
                    std::string name;
                    words >> name;
                    select_group(name);
                }
                else if (keyword == "f")
                {
                    ObjGroup *current = &result.groups[current_index];
                    std::vector<std::uint32_t> face;
                    std::string token;
                    while (words >> token)
                    {
                        const auto found = current->corners.find(token);
                        if (found != current->corners.end())
                        {
                            face.push_back(found->second);
                            continue;
                        }
                        long ids[3] = {0, 0, 0};
                        int slot = 0;
                        std::size_t start = 0;
                        while (slot < 3 && start <= token.size())
                        {
                            const std::size_t end = token.find('/', start);
                            const std::string part = token.substr(start, end == std::string::npos ? end : end - start);
                            if (!part.empty())
                                ids[slot] = std::strtol(part.c_str(), nullptr, 10);
                            ++slot;
                            if (end == std::string::npos)
                                break;
                            start = end + 1;
                        }
                        const long pi = resolve(ids[0], positions.size());
                        if (ids[0] == 0 || pi < 0 || pi >= static_cast<long>(positions.size()))
                        {
                            detail::set_error("OBJ face references a missing position");
                            return false;
                        }
                        glm::vec2 uv{0.0f};
                        if (ids[1] != 0)
                        {
                            const long ti = resolve(ids[1], uvs.size());
                            if (ti >= 0 && ti < static_cast<long>(uvs.size()))
                                uv = {uvs[ti].x, 1.0f - uvs[ti].y};
                        }
                        glm::vec3 normal{0.0f, 1.0f, 0.0f};
                        bool normal_given = false;
                        if (ids[2] != 0)
                        {
                            const long ni = resolve(ids[2], normals.size());
                            if (ni >= 0 && ni < static_cast<long>(normals.size()))
                            {
                                normal = normals[ni];
                                normal_given = true;
                            }
                        }
                        const std::uint32_t index = static_cast<std::uint32_t>(current->vertices.size());
                        current->vertices.push_back(make_vertex(positions[pi], normal, colour, uv.x, uv.y));
                        current->has_normal.push_back(normal_given);
                        current->corners.emplace(token, index);
                        face.push_back(index);
                    }
                    for (std::size_t corner = 2; corner < face.size(); ++corner)
                    {
                        current->indices.push_back(face[0]);
                        current->indices.push_back(face[corner - 1]);
                        current->indices.push_back(face[corner]);
                    }
                }
            }
            std::vector<ObjGroup> used;
            for (ObjGroup &group : result.groups)
            {
                if (group.indices.empty())
                    continue;
                generate_missing_normals(group.vertices, group.indices, group.has_normal);
                used.push_back(std::move(group));
            }
            result.groups = std::move(used);
            if (result.groups.empty())
            {
                detail::set_error("OBJ data contains no faces");
                return false;
            }
            return true;
        }

        struct MtlEntry
        {
            Colour diffuse{255, 255, 255, 255};
            std::string texture;
        };

        std::map<std::string, MtlEntry> parse_mtl(const std::filesystem::path &path)
        {
            std::map<std::string, MtlEntry> result;
            std::ifstream file(path);
            if (!file)
                return result;
            MtlEntry *current = nullptr;
            std::string line;
            while (std::getline(file, line))
            {
                std::istringstream words(line);
                std::string keyword;
                if (!(words >> keyword))
                    continue;
                if (keyword == "newmtl")
                {
                    std::string name;
                    words >> name;
                    current = &result[name];
                }
                else if (!current)
                {
                    continue;
                }
                else if (keyword == "Kd")
                {
                    double r = 1, g = 1, b = 1;
                    if (words >> r >> g >> b)
                        current->diffuse = {to_byte(r), to_byte(g), to_byte(b), 255};
                }
                else if (keyword == "map_Kd")
                {
                    // Options (e.g. -s 1 1 1) precede the file name, which is the final token.
                    std::string token, last;
                    while (words >> token)
                        last = token;
                    current->texture = last;
                }
            }
            return result;
        }

        // ---- glTF ----

        std::vector<std::uint8_t> decode_base64(const std::string &text)
        {
            std::vector<std::uint8_t> out;
            std::uint32_t accumulator = 0;
            int bits = 0;
            for (const char c : text)
            {
                int value;
                if (c >= 'A' && c <= 'Z')
                    value = c - 'A';
                else if (c >= 'a' && c <= 'z')
                    value = c - 'a' + 26;
                else if (c >= '0' && c <= '9')
                    value = c - '0' + 52;
                else if (c == '+' || c == '-')
                    value = 62;
                else if (c == '/' || c == '_')
                    value = 63;
                else
                    continue;
                accumulator = (accumulator << 6) | static_cast<std::uint32_t>(value);
                bits += 6;
                if (bits >= 8)
                {
                    bits -= 8;
                    out.push_back(static_cast<std::uint8_t>((accumulator >> bits) & 0xFF));
                }
            }
            return out;
        }

        std::string percent_decode(const std::string &text)
        {
            std::string out;
            for (std::size_t i = 0; i < text.size(); ++i)
            {
                if (text[i] == '%' && i + 2 < text.size() && std::isxdigit(static_cast<unsigned char>(text[i + 1])) &&
                    std::isxdigit(static_cast<unsigned char>(text[i + 2])))
                {
                    out.push_back(static_cast<char>(std::strtol(text.substr(i + 1, 2).c_str(), nullptr, 16)));
                    i += 2;
                }
                else
                {
                    out.push_back(text[i]);
                }
            }
            return out;
        }

        struct Gltf
        {
            json doc;
            std::filesystem::path directory;
            std::vector<std::vector<std::uint8_t>> buffers;
        };

        bool load_uri(const Gltf &gltf, const std::string &uri, std::vector<std::uint8_t> &out)
        {
            if (uri.rfind("data:", 0) == 0)
            {
                const std::size_t comma = uri.find(',');
                if (comma == std::string::npos)
                    return false;
                out = decode_base64(uri.substr(comma + 1));
                return true;
            }
            return read_file(gltf.directory / percent_decode(uri), out);
        }

        int component_size(int type)
        {
            switch (type)
            {
            case 5120:
            case 5121:
                return 1;
            case 5122:
            case 5123:
                return 2;
            case 5125:
            case 5126:
                return 4;
            }
            return 0;
        }

        int type_components(const std::string &type)
        {
            if (type == "SCALAR")
                return 1;
            if (type == "VEC2")
                return 2;
            if (type == "VEC3")
                return 3;
            if (type == "VEC4")
                return 4;
            return 0;
        }

        double read_component(const std::uint8_t *source, int type, bool normalized)
        {
            switch (type)
            {
            case 5120:
            {
                std::int8_t v;
                std::memcpy(&v, source, 1);
                return normalized ? std::max(v / 127.0, -1.0) : v;
            }
            case 5121:
                return normalized ? *source / 255.0 : *source;
            case 5122:
            {
                std::int16_t v;
                std::memcpy(&v, source, 2);
                return normalized ? std::max(v / 32767.0, -1.0) : v;
            }
            case 5123:
            {
                std::uint16_t v;
                std::memcpy(&v, source, 2);
                return normalized ? v / 65535.0 : v;
            }
            case 5125:
            {
                std::uint32_t v;
                std::memcpy(&v, source, 4);
                return v;
            }
            default:
            {
                float v;
                std::memcpy(&v, source, 4);
                return v;
            }
            }
        }

        // Reads an accessor into a flat array; returns false (with an error set) if it is malformed.
        bool read_accessor(const Gltf &gltf, int index, std::vector<double> &out, int &components)
        {
            const json &accessors = gltf.doc.value("accessors", json::array());
            if (index < 0 || index >= static_cast<int>(accessors.size()))
            {
                detail::set_error("glTF accessor index is out of range");
                return false;
            }
            const json &accessor = accessors[index];
            components = type_components(accessor.value("type", ""));
            const int type = accessor.value("componentType", 0);
            const int size = component_size(type);
            const std::size_t count = accessor.value("count", 0u);
            const bool normalized = accessor.value("normalized", false);
            if (components == 0 || size == 0)
            {
                detail::set_error("Unsupported glTF accessor type");
                return false;
            }
            out.assign(count * components, 0.0);
            if (!accessor.contains("bufferView"))
                return true;
            const json &views = gltf.doc.value("bufferViews", json::array());
            const int view_index = accessor["bufferView"].get<int>();
            if (view_index < 0 || view_index >= static_cast<int>(views.size()))
            {
                detail::set_error("glTF bufferView index is out of range");
                return false;
            }
            const json &view = views[view_index];
            const int buffer_index = view.value("buffer", -1);
            if (buffer_index < 0 || buffer_index >= static_cast<int>(gltf.buffers.size()))
            {
                detail::set_error("glTF buffer index is out of range");
                return false;
            }
            const std::vector<std::uint8_t> &buffer = gltf.buffers[buffer_index];
            const std::size_t stride = std::max<std::size_t>(view.value("byteStride", 0u),
                                                             static_cast<std::size_t>(components) * size);
            const std::size_t base = static_cast<std::size_t>(view.value("byteOffset", 0u)) +
                                     static_cast<std::size_t>(accessor.value("byteOffset", 0u));
            if (count > 0 && base + (count - 1) * stride + static_cast<std::size_t>(components) * size > buffer.size())
            {
                detail::set_error("glTF accessor reads past the end of its buffer");
                return false;
            }
            for (std::size_t i = 0; i < count; ++i)
                for (size_t c = 0; c < components; ++c)
                    out[i * components + c] = read_component(&buffer[base + i * stride + c * size], type, normalized);
            return true;
        }

        glm::mat4 node_matrix(const json &node)
        {
            if (node.contains("matrix") && node["matrix"].is_array() && node["matrix"].size() == 16)
            {
                glm::mat4 m(1.0f);
                for (int i = 0; i < 16; ++i)
                    m[i / 4][i % 4] = node["matrix"][i].get<float>();
                return m;
            }
            glm::vec3 t(0.0f), s(1.0f);
            glm::quat q(1.0f, 0.0f, 0.0f, 0.0f);
            if (node.contains("translation") && node["translation"].size() == 3)
                t = {node["translation"][0].get<float>(), node["translation"][1].get<float>(), node["translation"][2].get<float>()};
            if (node.contains("scale") && node["scale"].size() == 3)
                s = {node["scale"][0].get<float>(), node["scale"][1].get<float>(), node["scale"][2].get<float>()};
            if (node.contains("rotation") && node["rotation"].size() == 4)
                q = glm::quat(node["rotation"][3].get<float>(), node["rotation"][0].get<float>(),
                              node["rotation"][1].get<float>(), node["rotation"][2].get<float>());
            return glm::translate(glm::mat4(1.0f), t) * glm::mat4_cast(q) * glm::scale(glm::mat4(1.0f), s);
        }

        Bitmap *load_gltf_image(Model &model, Gltf &gltf, int image_index, std::map<int, Bitmap *> &cache)
        {
            const auto cached = cache.find(image_index);
            if (cached != cache.end())
                return cached->second;
            Bitmap *bitmap = nullptr;
            const json &images = gltf.doc.value("images", json::array());
            if (image_index >= 0 && image_index < static_cast<int>(images.size()))
            {
                const json &image = images[image_index];
                std::vector<std::uint8_t> bytes;
                if (image.contains("uri"))
                {
                    if (load_uri(gltf, image["uri"].get<std::string>(), bytes))
                        bitmap = load_bitmap_from_memory(bytes.data(), bytes.size());
                }
                else if (image.contains("bufferView"))
                {
                    const json &views = gltf.doc.value("bufferViews", json::array());
                    const int v = image["bufferView"].get<int>();
                    if (v >= 0 && v < static_cast<int>(views.size()))
                    {
                        const int b = views[v].value("buffer", -1);
                        const std::size_t offset = views[v].value("byteOffset", 0u);
                        const std::size_t length = views[v].value("byteLength", 0u);
                        if (b >= 0 && b < static_cast<int>(gltf.buffers.size()) &&
                            offset + length <= gltf.buffers[b].size())
                            bitmap = load_bitmap_from_memory(gltf.buffers[b].data() + offset, length);
                    }
                }
            }
            add_texture(model, bitmap);
            cache[image_index] = bitmap;
            return bitmap;
        }

        Mesh *build_gltf_primitive(Model &model, const Gltf &gltf, const json &primitive)
        {
            if (primitive.value("mode", 4) != 4 || !primitive.contains("attributes") ||
                !primitive["attributes"].contains("POSITION"))
                return nullptr;
            const json &attributes = primitive["attributes"];
            std::vector<double> positions, normals, uvs, colours;
            int pc = 0, nc = 0, tc = 0, cc = 0;
            if (!read_accessor(gltf, attributes["POSITION"].get<int>(), positions, pc) || pc != 3)
                return nullptr;
            const std::size_t count = positions.size() / 3;
            const bool has_normals = attributes.contains("NORMAL") &&
                                     read_accessor(gltf, attributes["NORMAL"].get<int>(), normals, nc) &&
                                     nc == 3 && normals.size() == count * 3;
            const bool has_uvs = attributes.contains("TEXCOORD_0") &&
                                 read_accessor(gltf, attributes["TEXCOORD_0"].get<int>(), uvs, tc) &&
                                 tc == 2 && uvs.size() == count * 2;
            const bool has_colours = attributes.contains("COLOR_0") &&
                                     read_accessor(gltf, attributes["COLOR_0"].get<int>(), colours, cc) &&
                                     (cc == 3 || cc == 4) && colours.size() == count * cc;
            std::vector<Vertex3D> vertices(count);
            std::vector<bool> normal_flags(count, has_normals);
            for (std::size_t i = 0; i < count; ++i)
            {
                Vertex3D &v = vertices[i];
                for (int k = 0; k < 3; ++k)
                {
                    v.position[k] = static_cast<float>(positions[i * 3 + k]);
                    if (has_normals)
                        v.normal[k] = static_cast<float>(normals[i * 3 + k]);
                }
                if (has_uvs)
                {
                    v.uv[0] = static_cast<float>(uvs[i * 2]);
                    v.uv[1] = static_cast<float>(uvs[i * 2 + 1]);
                }
                if (has_colours)
                    for (int k = 0; k < cc; ++k)
                        v.colour[k] = static_cast<float>(colours[i * cc + k]);
            }
            std::vector<std::uint32_t> indices;
            if (primitive.contains("indices"))
            {
                std::vector<double> raw;
                int ic = 0;
                if (!read_accessor(gltf, primitive["indices"].get<int>(), raw, ic) || ic != 1)
                    return nullptr;
                for (const double value : raw)
                {
                    if (value < 0 || value >= static_cast<double>(count))
                    {
                        detail::set_error("glTF index is out of range");
                        return nullptr;
                    }
                    indices.push_back(static_cast<std::uint32_t>(value));
                }
            }
            else
            {
                for (std::size_t i = 0; i < count; ++i)
                    indices.push_back(static_cast<std::uint32_t>(i));
            }
            indices.resize(indices.size() - indices.size() % 3);
            if (indices.empty())
                return nullptr;
            generate_missing_normals(vertices, indices, normal_flags);
            return add_mesh(model, vertices, indices);
        }

        void visit_gltf_node(Model &model, Gltf &gltf, int node_index, const glm::mat4 &parent, int depth,
                             std::map<std::pair<int, int>, Mesh *> &mesh_cache,
                             std::map<int, Bitmap *> &image_cache)
        {
            const json &nodes = gltf.doc.value("nodes", json::array());
            if (depth > 64 || node_index < 0 || node_index >= static_cast<int>(nodes.size()))
                return;
            const json &node = nodes[node_index];
            const glm::mat4 world = parent * node_matrix(node);
            if (node.contains("mesh"))
            {
                const int mesh_index = node["mesh"].get<int>();
                const json &meshes = gltf.doc.value("meshes", json::array());
                if (mesh_index >= 0 && mesh_index < static_cast<int>(meshes.size()))
                {
                    const json &primitives = meshes[mesh_index].value("primitives", json::array());
                    for (int p = 0; p < static_cast<int>(primitives.size()); ++p)
                    {
                        const auto key = std::make_pair(mesh_index, p);
                        auto cached = mesh_cache.find(key);
                        if (cached == mesh_cache.end())
                            cached = mesh_cache.emplace(key, build_gltf_primitive(model, gltf, primitives[p])).first;
                        if (!cached->second)
                            continue;
                        ModelPart part;
                        part.name = node.value("name", meshes[mesh_index].value("name", ""));
                        part.mesh = cached->second;
                        part.transform = world;
                        const json &materials = gltf.doc.value("materials", json::array());
                        const int m = primitives[p].value("material", -1);
                        if (m >= 0 && m < static_cast<int>(materials.size()))
                        {
                            const json &material = materials[m];
                            if (material.value("doubleSided", false))
                                part.material.cull = CullMode::none;
                            if (material.contains("pbrMetallicRoughness"))
                            {
                                const json &pbr = material["pbrMetallicRoughness"];
                                if (pbr.contains("baseColorFactor") && pbr["baseColorFactor"].size() == 4)
                                {
                                    const json &f = pbr["baseColorFactor"];
                                    part.material.tint = {to_byte(f[0].get<double>()), to_byte(f[1].get<double>()),
                                                          to_byte(f[2].get<double>()), to_byte(f[3].get<double>())};
                                }
                                if (pbr.contains("baseColorTexture"))
                                {
                                    const json &textures = gltf.doc.value("textures", json::array());
                                    const int t = pbr["baseColorTexture"].value("index", -1);
                                    if (t >= 0 && t < static_cast<int>(textures.size()) && textures[t].contains("source"))
                                        part.material.texture = load_gltf_image(model, gltf, textures[t]["source"].get<int>(),
                                                                                image_cache);
                                }
                            }
                        }
                        model.parts.push_back(std::move(part));
                    }
                }
            }
            if (node.contains("children"))
                for (const json &child : node["children"])
                    visit_gltf_node(model, gltf, child.get<int>(), world, depth + 1, mesh_cache, image_cache);
        }
    } // namespace

    // ---- OBJ API ----

    Mesh *load_obj_mesh_from_memory(const char *data, std::size_t size, Colour colour)
    {
        ObjData obj;
        if (!parse_obj(data, size, colour, obj))
            return nullptr;
        std::vector<Vertex3D> vertices;
        std::vector<std::uint32_t> indices;
        for (const ObjGroup &group : obj.groups)
        {
            const std::uint32_t base = static_cast<std::uint32_t>(vertices.size());
            vertices.insert(vertices.end(), group.vertices.begin(), group.vertices.end());
            for (const std::uint32_t index : group.indices)
                indices.push_back(base + index);
        }
        return create_mesh(vertices, indices);
    }

    Mesh *load_obj_mesh(const std::string &path, Colour colour)
    {
        std::vector<std::uint8_t> bytes;
        if (!read_file(path, bytes))
        {
            detail::set_error("Unable to open OBJ file: " + path);
            return nullptr;
        }
        return load_obj_mesh_from_memory(reinterpret_cast<const char *>(bytes.data()), bytes.size(), colour);
    }

    Model *load_obj_model(const std::string &path)
    {
        std::vector<std::uint8_t> bytes;
        if (!read_file(path, bytes))
        {
            detail::set_error("Unable to open OBJ file: " + path);
            return nullptr;
        }
        ObjData obj;
        if (!parse_obj(reinterpret_cast<const char *>(bytes.data()), bytes.size(), Colour{255, 255, 255, 255}, obj))
            return nullptr;
        const std::filesystem::path directory = std::filesystem::path(path).parent_path();
        std::map<std::string, MtlEntry> materials;
        if (!obj.material_library.empty())
            materials = parse_mtl(directory / obj.material_library);

        Model *model = new Model;
        std::map<std::string, Bitmap *> texture_cache;
        for (const ObjGroup &group : obj.groups)
        {
            Mesh *mesh = add_mesh(*model, group.vertices, group.indices);
            if (!mesh)
            {
                destroy_model(model);
                return nullptr;
            }
            ModelPart part;
            part.name = group.material;
            part.mesh = mesh;
            const auto entry = materials.find(group.material);
            if (entry != materials.end())
            {
                part.material.tint = entry->second.diffuse;
                if (!entry->second.texture.empty())
                {
                    auto cached = texture_cache.find(entry->second.texture);
                    if (cached == texture_cache.end())
                    {
                        std::string relative = entry->second.texture;
                        std::replace(relative.begin(), relative.end(), '\\', '/');
                        cached = texture_cache.emplace(entry->second.texture,
                                                       add_texture(*model, load_bitmap((directory / relative).string())))
                                     .first;
                    }
                    part.material.texture = cached->second;
                }
            }
            model->parts.push_back(std::move(part));
        }
        return model;
    }

    // ---- glTF API ----

    Model *load_gltf_model(const std::string &path)
    {
        std::vector<std::uint8_t> bytes;
        if (!read_file(path, bytes))
        {
            detail::set_error("Unable to open glTF file: " + path);
            return nullptr;
        }
        Gltf gltf;
        gltf.directory = std::filesystem::path(path).parent_path();
        std::vector<std::uint8_t> binary_chunk;
        bool has_binary_chunk = false;
        try
        {
            if (bytes.size() >= 12 && std::memcmp(bytes.data(), "glTF", 4) == 0)
            {
                std::size_t offset = 12;
                bool have_json = false;
                while (offset + 8 <= bytes.size())
                {
                    std::uint32_t length, type;
                    std::memcpy(&length, &bytes[offset], 4);
                    std::memcpy(&type, &bytes[offset + 4], 4);
                    offset += 8;
                    if (length > bytes.size() - offset)
                        break;
                    if (type == 0x4E4F534A && !have_json)
                    {
                        gltf.doc = json::parse(bytes.begin() + offset, bytes.begin() + offset + length);
                        have_json = true;
                    }
                    else if (type == 0x004E4942 && !has_binary_chunk)
                    {
                        binary_chunk.assign(bytes.begin() + offset, bytes.begin() + offset + length);
                        has_binary_chunk = true;
                    }
                    offset += (static_cast<std::size_t>(length) + 3) & ~static_cast<std::size_t>(3);
                }
                if (!have_json)
                {
                    detail::set_error("GLB file has no JSON chunk");
                    return nullptr;
                }
            }
            else
            {
                gltf.doc = json::parse(bytes.begin(), bytes.end());
            }
        }
        catch (const json::exception &error)
        {
            detail::set_error(std::string("Invalid glTF JSON: ") + error.what());
            return nullptr;
        }

        Model *model = new Model;
        try
        {
            if (gltf.doc.value("asset", json::object()).value("version", "") .rfind("2", 0) != 0)
            {
                detail::set_error("Only glTF 2.x is supported");
                delete model;
                return nullptr;
            }
            for (const json &buffer : gltf.doc.value("buffers", json::array()))
            {
                std::vector<std::uint8_t> data;
                if (buffer.contains("uri"))
                {
                    if (!load_uri(gltf, buffer["uri"].get<std::string>(), data))
                    {
                        detail::set_error("Unable to load glTF buffer");
                        delete model;
                        return nullptr;
                    }
                }
                else if (has_binary_chunk)
                {
                    data = binary_chunk;
                }
                gltf.buffers.push_back(std::move(data));
            }

            std::vector<int> roots;
            const json &nodes = gltf.doc.value("nodes", json::array());
            const json &scenes = gltf.doc.value("scenes", json::array());
            const int scene = gltf.doc.value("scene", 0);
            if (scene >= 0 && scene < static_cast<int>(scenes.size()))
            {
                for (const json &n : scenes[scene].value("nodes", json::array()))
                    roots.push_back(n.get<int>());
            }
            else
            {
                std::vector<bool> is_child(nodes.size(), false);
                for (const json &node : nodes)
                    for (const json &child : node.value("children", json::array()))
                        if (child.get<int>() >= 0 && child.get<int>() < static_cast<int>(nodes.size()))
                            is_child[child.get<int>()] = true;
                for (std::size_t i = 0; i < nodes.size(); ++i)
                    if (!is_child[i])
                        roots.push_back(static_cast<int>(i));
            }
            std::map<std::pair<int, int>, Mesh *> mesh_cache;
            std::map<int, Bitmap *> image_cache;
            for (const int root : roots)
                visit_gltf_node(*model, gltf, root, glm::mat4(1.0f), 0, mesh_cache, image_cache);
        }
        catch (const json::exception &error)
        {
            detail::set_error(std::string("Invalid glTF data: ") + error.what());
            destroy_model(model);
            return nullptr;
        }
        if (model->parts.empty())
        {
            detail::set_error("glTF file contains no drawable triangle meshes");
            destroy_model(model);
            return nullptr;
        }
        return model;
    }

    Model *load_model(const std::string &path)
    {
        std::string extension = std::filesystem::path(path).extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (extension == ".obj")
            return load_obj_model(path);
        if (extension == ".gltf" || extension == ".glb")
            return load_gltf_model(path);
        detail::set_error("Unsupported model format: " + path);
        return nullptr;
    }

    void destroy_model(Model *model)
    {
        if (!model)
            return;
        for (Mesh *mesh : model->meshes)
            destroy_mesh(mesh);
        for (Bitmap *texture : model->textures)
            destroy_bitmap(texture);
        delete model;
    }

    bool draw_model(const Model *model, const glm::mat4 &transform)
    {
        if (!model)
        {
            detail::set_error("draw_model requires a model");
            return false;
        }
        bool ok = true;
        for (const ModelPart &part : model->parts)
            ok = draw_mesh(part.mesh, transform * part.transform, part.material) && ok;
        return ok;
    }

} // namespace sl
