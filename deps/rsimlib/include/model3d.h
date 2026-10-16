#pragma once

#include "surface3d.h"

#include <string>
#include <vector>

namespace sl
{

    /** One drawable piece of a Model: a mesh, its material and a transform relative to the model. */
    struct ModelPart
    {
        std::string name;
        /** Owned by the Model; may be shared by several parts. */
        Mesh *mesh = nullptr;
        /** The texture, if any, is owned by the Model. */
        Material material;
        glm::mat4 transform{1.0f};
    };

    /**
     * A loaded multi-part model. The model owns its meshes and textures; release it
     * with destroy_model(). Parts may be edited (for example to retint a material).
     */
    struct Model
    {
        std::vector<ModelPart> parts;
        std::vector<Mesh *> meshes;
        std::vector<Bitmap *> textures;
    };

    /**
     * Load a Wavefront OBJ with its .mtl materials (Kd colour and map_Kd texture; one part per
     * usemtl group). Texture paths are resolved relative to the OBJ file. Missing textures
     * are skipped. Returns nullptr on failure (see last_error()).
     */
    Model *load_obj_model(const std::string &path);

    /**
     * Load a glTF 2.0 file (.gltf with external or base64 buffers, or binary .glb). Supports
     * node hierarchies, triangle primitives, POSITION/NORMAL/TEXCOORD_0/COLOR_0, base colour
     * factor and texture, and doubleSided. Skins, animation, morph targets and sparse accessors
     * are ignored. Returns nullptr on failure.
     */
    Model *load_gltf_model(const std::string &path);

    /** Load by file extension: .obj, .gltf or .glb. */
    Model *load_model(const std::string &path);

    void destroy_model(Model *model);

    /** Draw every part into the active surface; transform is applied on top of each part's own transform. */
    bool draw_model(const Model *model, const glm::mat4 &transform);

    /**
     * Load a Wavefront OBJ file as a single mesh (v/vt/vn/f, polygons triangulated, negative
     * indices supported). Normals are generated when absent. Materials are ignored.
     */
    Mesh *load_obj_mesh(const std::string &path, Colour colour = {255, 255, 255, 255});
    /** As load_obj_mesh() but parses OBJ text already in memory. */
    Mesh *load_obj_mesh_from_memory(const char *data, std::size_t size, Colour colour = {255, 255, 255, 255});

} // namespace sl
