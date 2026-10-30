#include "model3d.h"

#include "error.h"
#include <nlohmann/json.hpp>

#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <map>
#include <sstream>
#include <unordered_map>

namespace sl
{
    namespace detail
    {
		glm::mat4 model_animation_transform(const Model& model, const ModelPart& part)
		{
			return {};
		}

		bool model_part_visible(const Model& model, const ModelPart& part)
		{
			return false;
		}

		float model_animation_alpha(const Model& model, const ModelPart& part)
		{
			return 1.0f;
		}
    }

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
            {
                // Model textures are minified at distance and viewed at grazing angles.
                bitmap->mipmaps = true;
                bitmap->anisotropy = 16.0f;
                model.textures.push_back(bitmap);
            }
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
                        if (std::count(token.begin(), token.end(), '/') > 2)
                        {
                            detail::set_error("Invalid OBJ face corner");
                            return false;
                        }
                        int slot = 0;
                        std::size_t start = 0;
                        while (slot < 3 && start <= token.size())
                        {
                            const std::size_t end = token.find('/', start);
                            const std::string part = token.substr(start, end == std::string::npos ? end : end - start);
                            if (!part.empty())
                            {
                                char *parsed_end = nullptr;
                                errno = 0;
                                const long parsed = std::strtol(part.c_str(), &parsed_end, 10);
                                if (errno == ERANGE || parsed_end == part.c_str() || *parsed_end != '\0')
                                {
                                    detail::set_error("Invalid OBJ face index");
                                    return false;
                                }
                                ids[slot] = parsed;
                            }
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
                            if (ti < 0 || ti >= static_cast<long>(uvs.size()))
                            {
                                detail::set_error("OBJ face references a missing texture coordinate");
                                return false;
                            }
                            uv = {uvs[ti].x, 1.0f - uvs[ti].y};
                        }
                        glm::vec3 normal{0.0f, 1.0f, 0.0f};
                        bool normal_given = false;
                        if (ids[2] != 0)
                        {
                            const long ni = resolve(ids[2], normals.size());
                            if (ni < 0 || ni >= static_cast<long>(normals.size()))
                            {
                                detail::set_error("OBJ face references a missing normal");
                                return false;
                            }
                            normal = normals[ni];
                            normal_given = true;
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
            std::string normal_texture;
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
                else if (keyword == "map_Kd" || keyword == "norm" || keyword == "map_Bump" ||
                         keyword == "map_bump" || keyword == "bump")
                {
                    // Options (e.g. -s 1 1 1) precede the file name, which is the final token.
                    std::string token, last;
                    while (words >> token)
                        last = token;
                    (keyword == "map_Kd" ? current->texture : current->normal_texture) = last;
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
            if (type == "MAT4")
                return 16;
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
            const std::size_t element_size = static_cast<std::size_t>(components) * size;
            if (count > out.max_size() / static_cast<std::size_t>(components))
            {
                detail::set_error("glTF accessor is too large");
                return false;
            }
            if (accessor.contains("sparse"))
            {
                detail::set_error("Sparse glTF accessors are not supported");
                return false;
            }
            if (!accessor.contains("bufferView"))
            {
                detail::set_error("glTF accessor has no bufferView");
                return false;
            }
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
            const std::size_t view_offset = view.value("byteOffset", 0u);
            const std::size_t view_length = view.value("byteLength", 0u);
            const std::size_t accessor_offset = accessor.value("byteOffset", 0u);
            const std::size_t stride = view.value("byteStride", element_size);
            if (view_offset > buffer.size() || view_length > buffer.size() - view_offset ||
                accessor_offset > view_length || view_offset % size != 0 || accessor_offset % size != 0 ||
                stride < element_size || (view.contains("byteStride") &&
                                           (stride < 4 || stride > 252 || stride % 4 != 0)))
            {
                detail::set_error("Invalid glTF bufferView or accessor layout");
                return false;
            }
            const std::size_t available = view_length - accessor_offset;
            if (count > 0 && (available < element_size || (count - 1) > (available - element_size) / stride))
            {
                detail::set_error("glTF accessor reads past the end of its bufferView");
                return false;
            }
            out.assign(count * static_cast<std::size_t>(components), 0.0);
            const std::size_t base = view_offset + accessor_offset;
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
                            offset <= gltf.buffers[b].size() && length <= gltf.buffers[b].size() - offset)
                            bitmap = load_bitmap_from_memory(gltf.buffers[b].data() + offset, length);
                    }
                }
            }
            add_texture(model, bitmap);
            cache[image_index] = bitmap;
            return bitmap;
        }

        struct SkinAttributes
        {
            std::vector<glm::uvec4> joints;
            std::vector<glm::vec4> weights;
        };

        bool read_skin_attributes(const Gltf &gltf, const json &attributes, std::size_t count, SkinAttributes &out)
        {
            std::vector<double> joints, weights;
            int jc = 0, wc = 0;
            if (!read_accessor(gltf, attributes["JOINTS_0"].get<int>(), joints, jc) || jc != 4 ||
                joints.size() != count * 4 ||
                !read_accessor(gltf, attributes["WEIGHTS_0"].get<int>(), weights, wc) || wc != 4 ||
                weights.size() != count * 4)
            {
                detail::set_error("Invalid glTF JOINTS_0 or WEIGHTS_0 accessor");
                return false;
            }
            out.joints.resize(count);
            out.weights.resize(count);
            for (std::size_t i = 0; i < count; ++i)
            {
                float total = 0.0f;
                for (int k = 0; k < 4; ++k)
                {
                    const double j = joints[i * 4 + k];
                    out.joints[i][k] = j >= 0.0 ? static_cast<unsigned>(j) : 0u;
                    out.weights[i][k] = std::max(0.0f, static_cast<float>(weights[i * 4 + k]));
                    total += out.weights[i][k];
                }
                if (total > 0.0f)
                    out.weights[i] /= total;
                else
                    out.weights[i] = glm::vec4(0.0f);
            }
            return true;
        }

        Mesh *build_gltf_primitive(Model &model, const Gltf &gltf, const json &primitive, SkinAttributes *skin = nullptr)
        {
            if (primitive.value("mode", 4) != 4 || !primitive.contains("attributes") ||
                !primitive["attributes"].contains("POSITION"))
                return nullptr;
            const json &attributes = primitive["attributes"];
            std::vector<double> positions, normals, uvs, colours, tangents;
            int pc = 0, nc = 0, tc = 0, cc = 0, gc = 0;
            if (!read_accessor(gltf, attributes["POSITION"].get<int>(), positions, pc) || pc != 3)
                return nullptr;
            const std::size_t count = positions.size() / 3;
            bool has_normals = false, has_uvs = false, has_colours = false, has_tangents = false;
            if (attributes.contains("NORMAL"))
            {
                if (!read_accessor(gltf, attributes["NORMAL"].get<int>(), normals, nc) ||
                    nc != 3 || normals.size() != count * 3)
                {
                    detail::set_error("Invalid glTF NORMAL accessor");
                    return nullptr;
                }
                has_normals = true;
            }
            if (attributes.contains("TEXCOORD_0"))
            {
                if (!read_accessor(gltf, attributes["TEXCOORD_0"].get<int>(), uvs, tc) ||
                    tc != 2 || uvs.size() != count * 2)
                {
                    detail::set_error("Invalid glTF TEXCOORD_0 accessor");
                    return nullptr;
                }
                has_uvs = true;
            }
            if (attributes.contains("COLOR_0"))
            {
                if (!read_accessor(gltf, attributes["COLOR_0"].get<int>(), colours, cc) ||
                    (cc != 3 && cc != 4) || colours.size() != count * cc)
                {
                    detail::set_error("Invalid glTF COLOR_0 accessor");
                    return nullptr;
                }
                has_colours = true;
            }
            if (attributes.contains("TANGENT"))
            {
                if (!read_accessor(gltf, attributes["TANGENT"].get<int>(), tangents, gc) ||
                    gc != 4 || tangents.size() != count * 4)
                {
                    detail::set_error("Invalid glTF TANGENT accessor");
                    return nullptr;
                }
                has_tangents = true;
            }
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
                if (has_tangents)
                    for (int k = 0; k < 4; ++k)
                        v.tangent[k] = static_cast<float>(tangents[i * 4 + k]);
                if (has_colours)
                    for (int k = 0; k < cc; ++k)
                        v.colour[k] = static_cast<float>(colours[i * cc + k]);
            }
            std::vector<std::uint32_t> indices;
            if (primitive.contains("indices"))
            {
                std::vector<double> raw;
                int ic = 0;
                const int accessor_index = primitive["indices"].get<int>();
                const json &accessors = gltf.doc.value("accessors", json::array());
                if (accessor_index < 0 || accessor_index >= static_cast<int>(accessors.size()))
                {
                    detail::set_error("glTF index accessor is out of range");
                    return nullptr;
                }
                const json &index_accessor = accessors[accessor_index];
                const int index_type = index_accessor.value("componentType", 0);
                if (index_accessor.value("type", "") != "SCALAR" ||
                    (index_type != 5121 && index_type != 5123 && index_type != 5125) ||
                    index_accessor.value("normalized", false))
                {
                    detail::set_error("Invalid glTF index accessor type");
                    return nullptr;
                }
                if (!read_accessor(gltf, accessor_index, raw, ic) || ic != 1)
                    return nullptr;
                for (const double value : raw)
                {
                    if (value < 0 || value >= static_cast<double>(count) || std::floor(value) != value)
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
            if (indices.size() % 3 != 0)
            {
                detail::set_error("glTF triangle primitive has an incomplete triangle");
                return nullptr;
            }
            if (indices.empty())
                return nullptr;
            if (skin && !read_skin_attributes(gltf, attributes, count, *skin))
                return nullptr;
            generate_missing_normals(vertices, indices, normal_flags);
            if (!skin)
                return add_mesh(model, vertices, indices);
            // Skinning needs the bind-pose vertices (with tangents) on the CPU.
            const bool retained = mesh_cpu_data_retained();
            set_mesh_cpu_data_retained(true);
            Mesh *mesh = add_mesh(model, vertices, indices);
            set_mesh_cpu_data_retained(retained);
            return mesh;
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
                        const json &primitive_attributes = primitives[p].value("attributes", json::object());
                        const int skin_index = node.value("skin", -1);
                        const bool skinned = skin_index >= 0 && skin_index < static_cast<int>(model.skins.size()) &&
                                             primitive_attributes.contains("JOINTS_0") &&
                                             primitive_attributes.contains("WEIGHTS_0");
                        SkinAttributes skin_attributes;
                        Mesh *part_mesh = nullptr;
                        if (skinned)
                            part_mesh = build_gltf_primitive(model, gltf, primitives[p], &skin_attributes);
                        else
                        {
                            const auto key = std::make_pair(mesh_index, p);
                            auto cached = mesh_cache.find(key);
                            if (cached == mesh_cache.end())
                                cached = mesh_cache.emplace(key, build_gltf_primitive(model, gltf, primitives[p])).first;
                            part_mesh = cached->second;
                        }
                        if (!part_mesh)
                            continue;
                        ModelPart part;
                        part.name = node.value("name", meshes[mesh_index].value("name", ""));
                        part.mesh = part_mesh;
                        part.transform = skinned ? glm::mat4(1.0f) : world;
                        part.node = skinned ? -1 : node_index;
                        if (skinned)
                        {
                            ModelSkinBinding binding;
                            binding.part = static_cast<int>(model.parts.size());
                            binding.skin = skin_index;
                            binding.joints = std::move(skin_attributes.joints);
                            binding.weights = std::move(skin_attributes.weights);
                            part.skin_binding = static_cast<int>(model.skin_bindings.size());
                            model.skin_bindings.push_back(std::move(binding));
                        }
                        const json &accessors = gltf.doc.value("accessors", json::array());
                        const int position = primitives[p].value("attributes", json::object()).value("POSITION", -1);
                        if (!skinned && position >= 0 && position < static_cast<int>(accessors.size()) &&
                            accessors[position].contains("min") && accessors[position].contains("max") &&
                            accessors[position]["min"].size() == 3 && accessors[position]["max"].size() == 3)
                        {
                            glm::vec3 low, high;
                            for (int c = 0; c < 3; ++c)
                            {
                                low[c] = accessors[position]["min"][c].get<float>();
                                high[c] = accessors[position]["max"][c].get<float>();
                            }
                            for (int corner = 0; corner < 8; ++corner)
                            {
                                const glm::vec3 local((corner & 1) ? high.x : low.x, (corner & 2) ? high.y : low.y,
                                                      (corner & 4) ? high.z : low.z);
                                const glm::vec3 point = glm::vec3(world * glm::vec4(local, 1.0f));
                                part.bounds_min = part.has_bounds ? glm::min(part.bounds_min, point) : point;
                                part.bounds_max = part.has_bounds ? glm::max(part.bounds_max, point) : point;
                                part.has_bounds = true;
                            }
                        }
                        const json &materials = gltf.doc.value("materials", json::array());
                        const int m = primitives[p].value("material", -1);
                        if (m >= 0 && m < static_cast<int>(materials.size()))
                        {
                            const json &material = materials[m];
                            part.material.pbr = true;
                            if (material.value("doubleSided", false))
                                part.material.cull = CullMode::none;
                            if (material.value("alphaMode", "OPAQUE") == "MASK")
                                part.material.alpha_cutoff = material.value("alphaCutoff", 0.5f);
                            if (material.contains("pbrMetallicRoughness"))
                            {
                                const json &pbr = material["pbrMetallicRoughness"];
                                if (pbr.contains("baseColorFactor") && pbr["baseColorFactor"].size() == 4)
                                {
                                    const json &f = pbr["baseColorFactor"];
                                    part.material.tint = {to_byte(f[0].get<double>()), to_byte(f[1].get<double>()),
                                                          to_byte(f[2].get<double>()), to_byte(f[3].get<double>())};
                                }
                                part.material.metallic = pbr.value("metallicFactor", 1.0f);
                                part.material.roughness = pbr.value("roughnessFactor", 1.0f);
                                if (pbr.contains("metallicRoughnessTexture"))
                                {
                                    const json &textures = gltf.doc.value("textures", json::array());
                                    const int t = pbr["metallicRoughnessTexture"].value("index", -1);
                                    if (t >= 0 && t < static_cast<int>(textures.size()) && textures[t].contains("source"))
                                    {
                                        part.material.metallic_roughness = load_gltf_image(
                                            model, gltf, textures[t]["source"].get<int>(), image_cache);
                                        // Occlusion rides in R of this texture; a separate occlusion image is not combined.
                                        const int o = material.value("occlusionTexture", json::object()).value("index", -1);
                                        if (o < 0 || o >= static_cast<int>(textures.size()) ||
                                            !textures[o].contains("source") ||
                                            textures[o]["source"] != textures[t]["source"])
                                            part.material.occlusion_strength = 0.0f;
                                        else
                                            part.material.occlusion_strength =
                                                material["occlusionTexture"].value("strength", 1.0f);
                                    }
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
                            if (material.contains("emissiveFactor") && material["emissiveFactor"].size() == 3 &&
                                !material.contains("emissiveTexture"))
                            {
                                float strength = 1.0f;
                                if (material.contains("extensions") &&
                                    material["extensions"].contains("KHR_materials_emissive_strength"))
                                    strength = material["extensions"]["KHR_materials_emissive_strength"].value(
                                        "emissiveStrength", 1.0f);
                                for (int c = 0; c < 3; ++c)
                                    part.material.emissive[c] = material["emissiveFactor"][c].get<float>() * strength;
                            }
                            if (material.contains("normalTexture"))
                            {
                                const json &textures = gltf.doc.value("textures", json::array());
                                const int t = material["normalTexture"].value("index", -1);
                                if (t >= 0 && t < static_cast<int>(textures.size()) && textures[t].contains("source"))
                                    part.material.normal_map = load_gltf_image(model, gltf, textures[t]["source"].get<int>(),
                                                                               image_cache);
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

        glm::mat4 node_local(const ModelNode &node)
        {
            if (node.use_matrix && !node.animated)
                return node.matrix;
            return glm::translate(glm::mat4(1.0f), node.translation) * glm::mat4_cast(node.rotation) *
                   glm::scale(glm::mat4(1.0f), node.scale);
        }

        glm::mat4 node_world(const Model &model, int index)
        {
            glm::mat4 world(1.0f);
            const int limit = static_cast<int>(model.nodes.size());
            for (int guard = 0; index >= 0 && index < limit && guard <= limit; ++guard)
            {
                world = node_local(model.nodes[index]) * world;
                index = model.nodes[index].parent;
            }
            return world;
        }

        void skin_model_parts(Model &model)
        {
            std::vector<glm::mat4> matrices;
            for (ModelSkinBinding &binding : model.skin_bindings)
            {
                if (binding.part < 0 || binding.part >= static_cast<int>(model.parts.size()) ||
                    binding.skin < 0 || binding.skin >= static_cast<int>(model.skins.size()))
                    continue;
                Mesh *mesh = model.parts[binding.part].mesh;
                const ModelSkin &skin = model.skins[binding.skin];
                if (!mesh || mesh->vertices.size() != binding.joints.size())
                    continue;
                matrices.resize(skin.joints.size());
                for (std::size_t j = 0; j < matrices.size(); ++j)
                    matrices[j] = node_world(model, skin.joints[j]) * skin.inverse_bind[j];
                binding.skinned = mesh->vertices;
                for (std::size_t i = 0; i < binding.skinned.size(); ++i)
                {
                    glm::mat4 blended(0.0f);
                    for (int k = 0; k < 4; ++k)
                        if (binding.weights[i][k] > 0.0f && binding.joints[i][k] < matrices.size())
                            blended += binding.weights[i][k] * matrices[binding.joints[i][k]];
                    if (blended == glm::mat4(0.0f))
                        continue;
                    Vertex3D &v = binding.skinned[i];
                    const glm::vec3 position = glm::vec3(blended * glm::vec4(v.position[0], v.position[1], v.position[2], 1.0f));
                    const glm::mat3 linear(blended);
                    glm::vec3 normal = linear * glm::vec3(v.normal[0], v.normal[1], v.normal[2]);
                    glm::vec3 tangent = linear * glm::vec3(v.tangent[0], v.tangent[1], v.tangent[2]);
                    if (glm::dot(normal, normal) > 0.0f)
                        normal = glm::normalize(normal);
                    if (glm::dot(tangent, tangent) > 0.0f)
                        tangent = glm::normalize(tangent);
                    for (int c = 0; c < 3; ++c)
                    {
                        v.position[c] = position[c];
                        v.normal[c] = normal[c];
                        v.tangent[c] = tangent[c];
                    }
                }
                update_mesh_vertices(mesh, binding.skinned);
            }
        }

        void refresh_model_parts(Model &model)
        {
            for (ModelPart &part : model.parts)
                if (part.node >= 0 && part.node < static_cast<int>(model.nodes.size()))
                    part.transform = node_world(model, part.node);
            skin_model_parts(model);
        }

        void load_gltf_skins(Model &model, const Gltf &gltf)
        {
            const json &skins = gltf.doc.value("skins", json::array());
            for (const json &source : skins)
            {
                ModelSkin skin;
                skin.name = source.value("name", "");
                for (const json &joint : source.value("joints", json::array()))
                {
                    const int index = joint.get<int>();
                    if (index >= 0 && index < static_cast<int>(model.nodes.size()))
                        skin.joints.push_back(index);
                    else
                        skin.joints.push_back(0);
                }
                skin.inverse_bind.assign(skin.joints.size(), glm::mat4(1.0f));
                std::vector<double> values;
                int components = 0;
                if (source.contains("inverseBindMatrices") &&
                    read_accessor(gltf, source["inverseBindMatrices"].get<int>(), values, components) &&
                    components == 16 && values.size() >= skin.joints.size() * 16)
                    for (std::size_t j = 0; j < skin.joints.size(); ++j)
                        for (int i = 0; i < 16; ++i)
                            skin.inverse_bind[j][i / 4][i % 4] = static_cast<float>(values[j * 16 + i]);
                model.skins.push_back(std::move(skin));
            }
        }

        // Poses skinned parts once and takes their bounds from that pose.
        void finish_gltf_skins(Model &model)
        {
            if (model.skin_bindings.empty())
                return;
            skin_model_parts(model);
            for (const ModelSkinBinding &binding : model.skin_bindings)
            {
                ModelPart &part = model.parts[binding.part];
                for (const Vertex3D &v : binding.skinned)
                {
                    const glm::vec3 point(v.position[0], v.position[1], v.position[2]);
                    part.bounds_min = part.has_bounds ? glm::min(part.bounds_min, point) : point;
                    part.bounds_max = part.has_bounds ? glm::max(part.bounds_max, point) : point;
                    part.has_bounds = true;
                }
            }
        }

        void build_gltf_nodes(Model &model, const Gltf &gltf)
        {
            const json &nodes = gltf.doc.value("nodes", json::array());
            model.nodes.resize(nodes.size());
            for (std::size_t i = 0; i < nodes.size(); ++i)
            {
                const json &source = nodes[i];
                ModelNode &node = model.nodes[i];
                node.name = source.value("name", "");
                if (source.contains("matrix") && source["matrix"].is_array() && source["matrix"].size() == 16)
                {
                    node.use_matrix = true;
                    node.matrix = node_matrix(source);
                    const glm::mat4 &m = node.matrix;
                    node.translation = glm::vec3(m[3]);
                    glm::vec3 columns[3] = {glm::vec3(m[0]), glm::vec3(m[1]), glm::vec3(m[2])};
                    glm::vec3 scale(glm::length(columns[0]), glm::length(columns[1]), glm::length(columns[2]));
                    if (glm::determinant(glm::mat3(m)) < 0.0f)
                        scale.x = -scale.x;
                    for (int c = 0; c < 3; ++c)
                        if (scale[c] != 0.0f)
                            columns[c] /= scale[c];
                    node.scale = scale;
                    node.rotation = glm::normalize(glm::quat_cast(glm::mat3(columns[0], columns[1], columns[2])));
                }
                else
                {
                    if (source.contains("translation") && source["translation"].size() == 3)
                        node.translation = {source["translation"][0].get<float>(), source["translation"][1].get<float>(),
                                            source["translation"][2].get<float>()};
                    if (source.contains("scale") && source["scale"].size() == 3)
                        node.scale = {source["scale"][0].get<float>(), source["scale"][1].get<float>(),
                                      source["scale"][2].get<float>()};
                    if (source.contains("rotation") && source["rotation"].size() == 4)
                        node.rotation = glm::normalize(glm::quat(
                            source["rotation"][3].get<float>(), source["rotation"][0].get<float>(),
                            source["rotation"][1].get<float>(), source["rotation"][2].get<float>()));
                }
                node.rest_translation = node.translation;
                node.rest_rotation = node.rotation;
                node.rest_scale = node.scale;
            }
            for (std::size_t i = 0; i < nodes.size(); ++i)
                for (const json &child : nodes[i].value("children", json::array()))
                {
                    const int c = child.get<int>();
                    if (c >= 0 && c < static_cast<int>(nodes.size()) && c != static_cast<int>(i) &&
                        model.nodes[c].parent < 0)
                        model.nodes[c].parent = static_cast<int>(i);
                }
        }

        void load_gltf_clips(Model &model, const Gltf &gltf)
        {
            const json &animations = gltf.doc.value("animations", json::array());
            for (std::size_t a = 0; a < animations.size(); ++a)
            {
                const json &source = animations[a];
                ModelClip clip;
                clip.name = source.value("name", "animation" + std::to_string(a));
                const json &samplers = source.value("samplers", json::array());
                for (const json &channel : source.value("channels", json::array()))
                {
                    const json target = channel.value("target", json::object());
                    const int node = target.value("node", -1);
                    const std::string path = target.value("path", "");
                    const int sampler_index = channel.value("sampler", -1);
                    if (node < 0 || node >= static_cast<int>(model.nodes.size()))
                        continue;
                    if (path != "translation" && path != "rotation" && path != "scale")
                    {
                        model.unsupported_animations.push_back(clip.name + ": " + path);
                        continue;
                    }
                    if (sampler_index < 0 || sampler_index >= static_cast<int>(samplers.size()))
                        continue;
                    const json &sampler = samplers[sampler_index];
                    const std::string mode = sampler.value("interpolation", "LINEAR");
                    ModelChannel out;
                    out.node = node;
                    out.path = path == "translation" ? ModelChannelPath::translation
                               : path == "rotation"  ? ModelChannelPath::rotation
                                                     : ModelChannelPath::scale;
                    out.interpolation = mode == "STEP"          ? ModelInterpolation::step
                                        : mode == "CUBICSPLINE" ? ModelInterpolation::cubic_spline
                                                                : ModelInterpolation::linear;
                    std::vector<double> times, values;
                    int time_components = 0, value_components = 0;
                    const int expected = out.path == ModelChannelPath::rotation ? 4 : 3;
                    const std::size_t per_key =
                        static_cast<std::size_t>(expected) * (out.interpolation == ModelInterpolation::cubic_spline ? 3 : 1);
                    bool valid = read_accessor(gltf, sampler.value("input", -1), times, time_components) &&
                                 time_components == 1 &&
                                 read_accessor(gltf, sampler.value("output", -1), values, value_components) &&
                                 value_components == expected && !times.empty() && values.size() == times.size() * per_key;
                    for (std::size_t i = 0; valid && i < times.size(); ++i)
                        valid = std::isfinite(times[i]) &&
                                std::abs(times[i]) <= static_cast<double>(std::numeric_limits<float>::max()) &&
                                (i == 0 || times[i] >= times[i - 1]);
                    for (std::size_t i = 0; valid && i < values.size(); ++i)
                        valid = std::isfinite(values[i]) &&
                                std::abs(values[i]) <= static_cast<double>(std::numeric_limits<float>::max());
                    if (!valid)
                    {
                        model.unsupported_animations.push_back(clip.name + ": invalid " + path + " channel");
                        continue;
                    }
                    out.times.reserve(times.size());
                    for (double time : times)
                        out.times.push_back(static_cast<float>(time));
                    out.values.reserve(values.size());
                    for (double value : values)
                        out.values.push_back(static_cast<float>(value));
                    clip.duration = std::max(clip.duration, out.times.back());
                    clip.channels.push_back(std::move(out));
                }
                if (!clip.channels.empty())
                    model.clips.push_back(std::move(clip));
            }
        }

        // Writes the channel's value at `time` into out (3 floats, or 4 for rotation as x, y, z, w).
        void sample_channel(const ModelChannel &channel, float time, float *out)
        {
            const int components = channel.path == ModelChannelPath::rotation ? 4 : 3;
            const bool cubic = channel.interpolation == ModelInterpolation::cubic_spline;
            const int stride = components * (cubic ? 3 : 1);
            const int count = static_cast<int>(channel.times.size());
            // part: 0 = in-tangent, 1 = value, 2 = out-tangent (cubic only)
            const auto key = [&](int k, int part) { return &channel.values[static_cast<std::size_t>(k) * stride + (cubic ? part * components : 0)]; };
            const auto copy = [&](const float *from) { std::copy(from, from + components, out); };
            if (count == 1 || time <= channel.times.front())
                return copy(key(0, 1));
            if (time >= channel.times.back())
                return copy(key(count - 1, 1));
            const int i = static_cast<int>(std::upper_bound(channel.times.begin(), channel.times.end(), time) -
                                           channel.times.begin()) - 1;
            const float span = channel.times[i + 1] - channel.times[i];
            if (channel.interpolation == ModelInterpolation::step || span <= 0.0f)
                return copy(key(i, 1));
            const float s = (time - channel.times[i]) / span;
            const float *v0 = key(i, 1);
            const float *v1 = key(i + 1, 1);
            if (cubic)
            {
                const float *b0 = key(i, 2);     // out-tangent of the first key
                const float *a1 = key(i + 1, 0); // in-tangent of the second key
                const float s2 = s * s, s3 = s2 * s;
                for (int c = 0; c < components; ++c)
                    out[c] = (2 * s3 - 3 * s2 + 1) * v0[c] + span * (s3 - 2 * s2 + s) * b0[c] +
                             (-2 * s3 + 3 * s2) * v1[c] + span * (s3 - s2) * a1[c];
            }
            else if (channel.path == ModelChannelPath::rotation)
            {
                const glm::quat q = glm::slerp(glm::quat(v0[3], v0[0], v0[1], v0[2]), glm::quat(v1[3], v1[0], v1[1], v1[2]), s);
                out[0] = q.x;
                out[1] = q.y;
                out[2] = q.z;
                out[3] = q.w;
            }
            else
            {
                for (int c = 0; c < components; ++c)
                    out[c] = v0[c] + (v1[c] - v0[c]) * s;
            }
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
                if (!entry->second.normal_texture.empty())
                {
                    auto cached = texture_cache.find(entry->second.normal_texture);
                    if (cached == texture_cache.end())
                    {
                        std::string relative = entry->second.normal_texture;
                        std::replace(relative.begin(), relative.end(), '\\', '/');
                        cached = texture_cache.emplace(entry->second.normal_texture,
                                                       add_texture(*model, load_bitmap((directory / relative).string())))
                                     .first;
                    }
                    part.material.normal_map = cached->second;
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
            build_gltf_nodes(*model, gltf);
            load_gltf_skins(*model, gltf);
            for (const int root : roots)
                visit_gltf_node(*model, gltf, root, glm::mat4(1.0f), 0, mesh_cache, image_cache);
            load_gltf_clips(*model, gltf);
            finish_gltf_skins(*model);
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
        if (extension == ".ac")
            return load_ac3d_model(path);
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

    int find_model_clip(const Model *model, const std::string &name)
    {
        if (model)
            for (std::size_t i = 0; i < model->clips.size(); ++i)
                if (model->clips[i].name == name)
                    return static_cast<int>(i);
        return -1;
    }

    float model_clip_duration(const Model *model, int clip)
    {
        return model && clip >= 0 && clip < static_cast<int>(model->clips.size()) ? model->clips[clip].duration : 0.0f;
    }

    void reset_model_pose(Model *model)
    {
        if (!model)
            return;
        for (ModelNode &node : model->nodes)
        {
            node.translation = node.rest_translation;
            node.rotation = node.rest_rotation;
            node.scale = node.rest_scale;
            node.animated = false;
        }
        refresh_model_parts(*model);
    }

    bool sample_model_clip(Model *model, int clip, float time, bool loop, bool reset_pose)
    {
        if (!model || clip < 0 || clip >= static_cast<int>(model->clips.size()))
        {
            detail::set_error("Unknown model clip");
            return false;
        }
        const ModelClip &source = model->clips[clip];
        if (!std::isfinite(time))
            time = 0.0f;
        if (loop && source.duration > 0.0f)
        {
            time = std::fmod(time, source.duration);
            if (time < 0.0f)
                time += source.duration;
        }
        else
        {
            time = std::clamp(time, 0.0f, source.duration);
        }
        if (reset_pose)
            for (ModelNode &node : model->nodes)
            {
                node.translation = node.rest_translation;
                node.rotation = node.rest_rotation;
                node.scale = node.rest_scale;
                node.animated = false;
            }
        for (const ModelChannel &channel : source.channels)
        {
            ModelNode &node = model->nodes[channel.node];
            float value[4] = {0, 0, 0, 1};
            sample_channel(channel, time, value);
            if (!node.animated)
            {
                // A matrix node animates from its decomposed pose, so channels that do not touch every property keep the rest value.
                node.animated = true;
            }
            switch (channel.path)
            {
            case ModelChannelPath::translation:
                node.translation = {value[0], value[1], value[2]};
                break;
            case ModelChannelPath::scale:
                node.scale = {value[0], value[1], value[2]};
                break;
            case ModelChannelPath::rotation:
            {
                const glm::quat q(value[3], value[0], value[1], value[2]);
                node.rotation = glm::length(q) > 0.0f ? glm::normalize(q) : glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
                break;
            }
            }
        }
        refresh_model_parts(*model);
        return true;
    }

    bool sample_model_clip(Model *model, const std::string &name, float time, bool loop, bool reset_pose)
    {
        const int clip = find_model_clip(model, name);
        if (clip < 0)
        {
            detail::set_error("Unknown model clip: " + name);
            return false;
        }
        return sample_model_clip(model, clip, time, loop, reset_pose);
    }

    bool play_model_clip(Model *model, const std::string &name, bool loop, float speed)
    {
        const int clip = find_model_clip(model, name);
        if (clip < 0)
        {
            detail::set_error("Unknown model clip: " + name);
            return false;
        }
        model->active_clip = clip;
        model->clip_loop = loop;
        model->clip_speed = std::isfinite(speed) ? speed : 1.0f;
        model->clip_time = model->clip_speed < 0.0f ? model->clips[clip].duration : 0.0f;
        return sample_model_clip(model, clip, model->clip_time, loop);
    }

    void stop_model_clip(Model *model)
    {
        if (!model)
            return;
        model->active_clip = -1;
        reset_model_pose(model);
    }

    void update_model_clip(Model *model, float dt)
    {
        if (!model || model->active_clip < 0 || model->active_clip >= static_cast<int>(model->clips.size()))
            return;
        const float duration = model->clips[model->active_clip].duration;
        model->clip_time += dt * model->clip_speed;
        bool finished = false;
        if (model->clip_loop && duration > 0.0f)
        {
            model->clip_time = std::fmod(model->clip_time, duration);
            if (model->clip_time < 0.0f)
                model->clip_time += duration;
        }
        else if (model->clip_time >= duration && model->clip_speed > 0.0f)
        {
            model->clip_time = duration;
            finished = true;
        }
        else if (model->clip_time <= 0.0f && model->clip_speed < 0.0f)
        {
            model->clip_time = 0.0f;
            finished = true;
        }
        sample_model_clip(model, model->active_clip, model->clip_time, model->clip_loop);
        if (finished)
            model->active_clip = -1;
    }

    bool model_clip_playing(const Model *model)
    {
        return model && model->active_clip >= 0;
    }

    bool model_part_is_visible(const Model *model, const ModelPart &part)
    {
        return model && detail::model_part_visible(*model, part);
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
            if (detail::model_part_visible(*model, part))
            {
                const float alpha = std::clamp(detail::model_animation_alpha(*model, part), 0.0f, 1.0f);
                // The mesh shader does not blend, so mostly-transparent parts (hotspots, glass, propeller blur)
                // would draw as opaque slabs. Skip them instead.
                if (alpha < 0.5f)
                    continue;
                Material material = part.material;
                material.tint.alpha = static_cast<std::uint8_t>(std::lround(alpha * 255.0f));
                ok = draw_mesh(part.mesh, transform * detail::model_animation_transform(*model, part) *
                                                part.transform,
                               material) &&
                     ok;
            }
        return ok;
    }

} // namespace sl
