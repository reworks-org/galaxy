#include "model3d.h"

#include "draw.h"
#include "error.h"
#include <nlohmann/json.hpp>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <set>
#include <tuple>

namespace sl
{
    namespace
    {
        using json = nlohmann::json;

        constexpr int kArrayBuffer = 34962;
        constexpr int kElementArrayBuffer = 34963;

        struct GlbBuilder
        {
            json doc;
            std::vector<std::uint8_t> bin;

            void align()
            {
                while (bin.size() % 4 != 0)
                    bin.push_back(0);
            }

            int add_view(const void *data, std::size_t bytes, int target)
            {
                align();
                json view = {{"buffer", 0}, {"byteOffset", bin.size()}, {"byteLength", bytes}};
                if (target)
                    view["target"] = target;
                const auto *source = static_cast<const std::uint8_t *>(data);
                bin.insert(bin.end(), source, source + bytes);
                doc["bufferViews"].push_back(view);
                return static_cast<int>(doc["bufferViews"].size()) - 1;
            }

            int add_accessor(const void *data, std::size_t count, int component_type, const char *type, std::size_t stride_bytes,
                             int target, const json &min = nullptr, const json &max = nullptr)
            {
                json accessor = {{"bufferView", add_view(data, count * stride_bytes, target)},
                                 {"componentType", component_type},
                                 {"count", count},
                                 {"type", type}};
                if (!min.is_null())
                    accessor["min"] = min;
                if (!max.is_null())
                    accessor["max"] = max;
                doc["accessors"].push_back(accessor);
                return static_cast<int>(doc["accessors"].size()) - 1;
            }

            int add_floats(const std::vector<float> &values, int components, const char *type)
            {
                return add_accessor(values.data(), values.size() / components, 5126, type, sizeof(float) * components, 0);
            }
        };

        struct MaterialKey
        {
            Colour tint;
            const Bitmap *texture;
            const Bitmap *normal;
            CullMode cull;
            float cutoff;
            bool pbr;
            float metallic, roughness, occlusion;
            const Bitmap *metallic_roughness;
            float emissive[3];
            bool operator<(const MaterialKey &o) const
            {
                const auto key = [](const MaterialKey &k) {
                    return std::make_tuple(k.tint.red, k.tint.green, k.tint.blue, k.tint.alpha, k.texture, k.normal,
                                           static_cast<int>(k.cull), k.cutoff, k.pbr, k.metallic, k.roughness, k.occlusion,
                                           k.metallic_roughness, k.emissive[0], k.emissive[1], k.emissive[2]);
                };
                return key(*this) < key(o);
            }
        };

        bool read_png(Bitmap *bitmap, std::vector<std::uint8_t> &bytes)
        {
            static std::atomic<unsigned> counter{0};
            const std::filesystem::path path =
                std::filesystem::temp_directory_path() / ("simlib-gltf-" + std::to_string(counter++) + ".png");
            const bool saved = save_bitmap(bitmap, path.string());
            if (saved)
            {
                std::ifstream file(path, std::ios::binary);
                bytes.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            }
            std::error_code ignored;
            std::filesystem::remove(path, ignored);
            return saved && !bytes.empty();
        }

        int add_image_texture(GlbBuilder &glb, std::map<const Bitmap *, int> &cache, Bitmap *bitmap, std::string &error)
        {
            auto found = cache.find(bitmap);
            if (found != cache.end())
                return found->second;
            std::vector<std::uint8_t> png;
            if (!read_png(bitmap, png))
            {
                error = "Unable to encode a model texture as PNG";
                return -1;
            }
            const int view = glb.add_view(png.data(), png.size(), 0);
            glb.doc["images"].push_back({{"bufferView", view}, {"mimeType", "image/png"}});
            glb.doc["textures"].push_back({{"source", static_cast<int>(glb.doc["images"].size()) - 1}, {"sampler", 0}});
            const int texture = static_cast<int>(glb.doc["textures"].size()) - 1;
            cache.emplace(bitmap, texture);
            return texture;
        }

        json trs_node(const ModelNode &node)
        {
            json out;
            if (!node.name.empty())
                out["name"] = node.name;
            out["translation"] = {node.rest_translation.x, node.rest_translation.y, node.rest_translation.z};
            out["rotation"] = {node.rest_rotation.x, node.rest_rotation.y, node.rest_rotation.z, node.rest_rotation.w};
            out["scale"] = {node.rest_scale.x, node.rest_scale.y, node.rest_scale.z};
            return out;
        }
    } // namespace

    bool save_model_gltf(const Model *model, const std::string &path)
    {
        if (!model)
        {
            detail::set_error("No model to save");
            return false;
        }
        GlbBuilder glb;
        glb.doc["asset"] = {{"version", "2.0"}, {"generator", "simlib"}};
        glb.doc["scene"] = 0;
        glb.doc["samplers"] = json::array({{{"magFilter", 9729}, {"minFilter", 9987}, {"wrapS", 10497}, {"wrapT", 10497}}});
        json &nodes = glb.doc["nodes"] = json::array();
        std::vector<int> roots;

        // Original scene-graph nodes keep their indices so animation channels still target the right node.
        for (const ModelNode &node : model->nodes)
            nodes.push_back(trs_node(node));
        for (std::size_t i = 0; i < model->nodes.size(); ++i)
        {
            const int parent = model->nodes[i].parent;
            if (parent >= 0 && parent < static_cast<int>(model->nodes.size()))
                nodes[parent]["children"].push_back(static_cast<int>(i));
            else
                roots.push_back(static_cast<int>(i));
        }

        for (const ModelSkin &skin : model->skins)
        {
            json out = {{"joints", skin.joints}};
            if (!skin.name.empty())
                out["name"] = skin.name;
            if (!skin.inverse_bind.empty())
                out["inverseBindMatrices"] = glb.add_accessor(skin.inverse_bind.data(), skin.inverse_bind.size(), 5126, "MAT4",
                                                              sizeof(glm::mat4), 0);
            glb.doc["skins"].push_back(out);
        }

        std::map<MaterialKey, int> material_cache;
        std::map<const Bitmap *, int> texture_cache;
        std::set<int> mesh_nodes;
        std::string error;
        for (const ModelPart &part : model->parts)
        {
            const Mesh *mesh = part.mesh;
            if (!mesh || mesh->vertices.empty() || mesh->indices.empty())
            {
                detail::set_error("Part \"" + part.name +
                                  "\" has no CPU mesh data; call set_mesh_cpu_data_retained(true) before loading the model");
                return false;
            }
            const MaterialKey key{part.material.tint, part.material.texture, part.material.normal_map, part.material.cull,
                                  part.material.alpha_cutoff, part.material.pbr, part.material.metallic, part.material.roughness,
                                  part.material.occlusion_strength, part.material.metallic_roughness,
                                  {part.material.emissive[0], part.material.emissive[1], part.material.emissive[2]}};
            std::vector<float> positions, normals, uvs, colours;
            glm::vec3 low(1e30f), high(-1e30f);
            bool white = true;
            for (const Vertex3D &v : mesh->vertices)
            {
                const glm::vec3 p(v.position[0], v.position[1], v.position[2]);
                low = glm::min(low, p);
                high = glm::max(high, p);
                positions.insert(positions.end(), v.position, v.position + 3);
                normals.insert(normals.end(), v.normal, v.normal + 3);
                uvs.insert(uvs.end(), v.uv, v.uv + 2);
                colours.insert(colours.end(), v.colour, v.colour + 4);
                for (int c = 0; c < 4; ++c)
                    white = white && v.colour[c] == 1.0f;
            }
            json primitive;
            primitive["attributes"]["POSITION"] =
                glb.add_accessor(positions.data(), mesh->vertices.size(), 5126, "VEC3", 12, kArrayBuffer,
                                 json::array({low.x, low.y, low.z}), json::array({high.x, high.y, high.z}));
            primitive["attributes"]["NORMAL"] = glb.add_floats(normals, 3, "VEC3");
            primitive["attributes"]["TEXCOORD_0"] = glb.add_floats(uvs, 2, "VEC2");
            if (!white)
                primitive["attributes"]["COLOR_0"] = glb.add_floats(colours, 4, "VEC4");
            const ModelSkinBinding *skin = part.skin_binding >= 0 && part.skin_binding < static_cast<int>(model->skin_bindings.size())
                                               ? &model->skin_bindings[part.skin_binding]
                                               : nullptr;
            if (skin && skin->joints.size() == mesh->vertices.size())
            {
                std::vector<std::uint16_t> joints;
                std::vector<float> weights;
                for (std::size_t i = 0; i < skin->joints.size(); ++i)
                    for (int k = 0; k < 4; ++k)
                    {
                        joints.push_back(static_cast<std::uint16_t>(skin->joints[i][k]));
                        weights.push_back(skin->weights[i][k]);
                    }
                primitive["attributes"]["JOINTS_0"] = glb.add_accessor(joints.data(), skin->joints.size(), 5123, "VEC4", 8, 0);
                primitive["attributes"]["WEIGHTS_0"] = glb.add_floats(weights, 4, "VEC4");
            }
            else
                skin = nullptr;
            primitive["indices"] = glb.add_accessor(mesh->indices.data(), mesh->indices.size(), 5125, "SCALAR", 4, kElementArrayBuffer);

            auto material = material_cache.find(key);
            if (material == material_cache.end())
            {
                json m;
                m["pbrMetallicRoughness"]["baseColorFactor"] = {part.material.tint.red / 255.0f, part.material.tint.green / 255.0f,
                                                                part.material.tint.blue / 255.0f, part.material.tint.alpha / 255.0f};
                m["pbrMetallicRoughness"]["metallicFactor"] = part.material.pbr ? part.material.metallic : 0.0f;
                m["pbrMetallicRoughness"]["roughnessFactor"] = part.material.pbr ? part.material.roughness : 1.0f;
                if (part.material.pbr && part.material.metallic_roughness)
                {
                    const int t = add_image_texture(glb, texture_cache, part.material.metallic_roughness, error);
                    if (t < 0)
                    {
                        detail::set_error(error);
                        return false;
                    }
                    m["pbrMetallicRoughness"]["metallicRoughnessTexture"] = {{"index", t}};
                    if (part.material.occlusion_strength > 0.0f)
                        m["occlusionTexture"] = {{"index", t}, {"strength", part.material.occlusion_strength}};
                }
                if (part.material.pbr && (part.material.emissive[0] > 0.0f || part.material.emissive[1] > 0.0f ||
                                          part.material.emissive[2] > 0.0f))
                {
                    const float peak = std::max({part.material.emissive[0], part.material.emissive[1], part.material.emissive[2]});
                    const float scale = peak > 1.0f ? peak : 1.0f;
                    m["emissiveFactor"] = {part.material.emissive[0] / scale, part.material.emissive[1] / scale,
                                           part.material.emissive[2] / scale};
                    if (scale > 1.0f)
                    {
                        m["extensions"]["KHR_materials_emissive_strength"] = {{"emissiveStrength", scale}};
                        glb.doc["extensionsUsed"] = json::array({"KHR_materials_emissive_strength"});
                    }
                }
                if (part.material.texture)
                {
                    const int t = add_image_texture(glb, texture_cache, part.material.texture, error);
                    if (t < 0)
                    {
                        detail::set_error(error);
                        return false;
                    }
                    m["pbrMetallicRoughness"]["baseColorTexture"] = {{"index", t}};
                }
                if (part.material.normal_map)
                {
                    const int t = add_image_texture(glb, texture_cache, part.material.normal_map, error);
                    if (t < 0)
                    {
                        detail::set_error(error);
                        return false;
                    }
                    m["normalTexture"] = {{"index", t}};
                }
                if (part.material.cull == CullMode::none)
                    m["doubleSided"] = true;
                if (part.material.alpha_cutoff > 0.0f)
                {
                    m["alphaMode"] = "MASK";
                    m["alphaCutoff"] = part.material.alpha_cutoff;
                }
                glb.doc["materials"].push_back(m);
                material = material_cache.emplace(key, static_cast<int>(glb.doc["materials"].size()) - 1).first;
            }
            primitive["material"] = material->second;
            glb.doc["meshes"].push_back({{"name", part.name}, {"primitives", json::array({primitive})}});
            const int mesh_index = static_cast<int>(glb.doc["meshes"].size()) - 1;

            const bool attached = part.node >= 0 && part.node < static_cast<int>(model->nodes.size());
            if (attached && mesh_nodes.insert(part.node).second)
            {
                // The first part of a scene-graph node carries its mesh directly, so a round trip keeps the node count.
                nodes[part.node]["mesh"] = mesh_index;
                continue;
            }
            json node = {{"name", part.name}, {"mesh", mesh_index}};
            if (skin)
                node["skin"] = skin->skin;
            if (!attached && !skin)
            {
                const float *m = &part.transform[0][0];
                node["matrix"] = json::array();
                for (int i = 0; i < 16; ++i)
                    node["matrix"].push_back(m[i]);
            }
            nodes.push_back(node);
            const int index = static_cast<int>(nodes.size()) - 1;
            if (attached)
                nodes[part.node]["children"].push_back(index);
            else
                roots.push_back(index);
        }
        glb.doc["scenes"] = json::array({{{"nodes", roots}}});

        for (const ModelClip &clip : model->clips)
        {
            json animation = {{"name", clip.name}, {"channels", json::array()}, {"samplers", json::array()}};
            for (const ModelChannel &channel : clip.channels)
            {
                const bool rotation = channel.path == ModelChannelPath::rotation;
                const int components = rotation ? 4 : 3;
                const int input = glb.add_accessor(channel.times.data(), channel.times.size(), 5126, "SCALAR", 4, 0,
                                                   json::array({channel.times.front()}), json::array({channel.times.back()}));
                const int output = glb.add_accessor(channel.values.data(), channel.values.size() / components, 5126,
                                                    rotation ? "VEC4" : "VEC3", sizeof(float) * components, 0);
                const char *interpolation = channel.interpolation == ModelInterpolation::step           ? "STEP"
                                            : channel.interpolation == ModelInterpolation::cubic_spline ? "CUBICSPLINE"
                                                                                                        : "LINEAR";
                animation["samplers"].push_back({{"input", input}, {"output", output}, {"interpolation", interpolation}});
                const char *target = rotation                                           ? "rotation"
                                     : channel.path == ModelChannelPath::translation ? "translation"
                                                                                      : "scale";
                animation["channels"].push_back({{"sampler", static_cast<int>(animation["samplers"].size()) - 1},
                                                 {"target", {{"node", channel.node}, {"path", target}}}});
            }
            glb.doc["animations"].push_back(animation);
        }

        glb.align();
        glb.doc["buffers"] = json::array({{{"byteLength", glb.bin.size()}}});
        std::string text = glb.doc.dump();
        while (text.size() % 4 != 0)
            text.push_back(' ');

        const auto put32 = [](std::ofstream &out, std::uint32_t value) {
            const char bytes[4] = {static_cast<char>(value & 0xff), static_cast<char>((value >> 8) & 0xff),
                                   static_cast<char>((value >> 16) & 0xff), static_cast<char>((value >> 24) & 0xff)};
            out.write(bytes, 4);
        };
        std::ofstream out(path, std::ios::binary);
        if (!out)
        {
            detail::set_error("Unable to open for writing: " + path);
            return false;
        }
        put32(out, 0x46546C67); // "glTF"
        put32(out, 2);
        put32(out, static_cast<std::uint32_t>(12 + 8 + text.size() + 8 + glb.bin.size()));
        put32(out, static_cast<std::uint32_t>(text.size()));
        put32(out, 0x4E4F534A); // "JSON"
        out.write(text.data(), static_cast<std::streamsize>(text.size()));
        put32(out, static_cast<std::uint32_t>(glb.bin.size()));
        put32(out, 0x004E4942); // "BIN\0"
        out.write(reinterpret_cast<const char *>(glb.bin.data()), static_cast<std::streamsize>(glb.bin.size()));
        out.flush();
        if (!out)
        {
            detail::set_error("Failed writing glTF file: " + path);
            return false;
        }
        return true;
    }
} // namespace sl
