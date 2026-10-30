#pragma once

#include "surface3d.h"

#include <glm/gtc/quaternion.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace sl
{

    enum class ModelAnimationType
    {
        rotation,
        translation,
        spin,
        blend
    };

    /** Property-driven transform or alpha blend applied to parts with matching object names. */
    struct ModelAnimation
    {
        ModelAnimationType type = ModelAnimationType::rotation;
        std::string property;
        std::vector<std::string> objects;
        glm::vec3 axis{0.0f, 1.0f, 0.0f};
        glm::vec3 center{0.0f};
        float factor = 1.0f;
        float offset = 0.0f;
        float minimum = 0.0f;
        float maximum = 1.0f;
        std::vector<glm::vec2> interpolation;
    };

    enum class ModelConditionType
    {
        property,
        less_than,
        greater_than,
        equal,
        not_equal,
        less_equal,
        greater_equal,
        logical_and,
        logical_or,
        logical_not
    };

    /** Boolean expression used by model-part visibility rules. */
    struct ModelCondition
    {
        ModelConditionType type = ModelConditionType::property;
        std::string property;
        float value = 0.0f;
        std::vector<ModelCondition> children;
    };

    /** Conditional visibility rule for named model parts. */
    struct ModelVisibility
    {
        std::vector<std::string> objects;
        ModelCondition condition;
    };

    /** One drawable piece of a Model: a mesh, its material and a transform relative to the model. */
    struct ModelPart
    {
        std::string name;
        /** Owned by the Model; may be shared by several parts. */
        Mesh *mesh = nullptr;
        /** The texture, if any, is owned by the Model. */
        Material material;
        glm::mat4 transform{1.0f};
        /** Index into Model::nodes for glTF parts (so clips can move it), or -1. */
        int node = -1;
        /** Index into Model::skin_bindings for skinned parts (whose `transform` is then identity), or -1. */
        int skin_binding = -1;
        bool visible = true;
        /** Axis-aligned bounds of the part's geometry after `transform` (before animation); valid when has_bounds. */
        glm::vec3 bounds_min{0.0f};
        glm::vec3 bounds_max{0.0f};
        bool has_bounds = false;
    };

    /** One node of a glTF scene graph. `translation`, `rotation` and `scale` are the current (animated) local pose. */
    struct ModelNode
    {
        std::string name;
        /** Index of the parent node, or -1 for a root. */
        int parent = -1;
        glm::vec3 translation{0.0f};
        glm::quat rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 scale{1.0f};
        /** The pose the node was loaded with; reset_model_pose() restores it. */
        glm::vec3 rest_translation{0.0f};
        glm::quat rest_rotation{1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 rest_scale{1.0f};
        /** Nodes written as a matrix use it until a clip animates them (then the decomposed pose above is used). */
        bool use_matrix = false;
        glm::mat4 matrix{1.0f};
        /** Set while a clip drives this node. */
        bool animated = false;
    };

    /** A glTF skin: the joint nodes and their inverse bind matrices. */
    struct ModelSkin
    {
        std::string name;
        /** Indices into Model::nodes. */
        std::vector<int> joints;
        /** One per joint (identity when the file has none). */
        std::vector<glm::mat4> inverse_bind;
    };

    /**
     * Per-part skinning data. The model skins on the CPU whenever the pose changes (clip sampling,
     * reset_model_pose()) and uploads the result with update_mesh_vertices(). The skinned mesh is not shared.
     */
    struct ModelSkinBinding
    {
        int part = -1;
        int skin = -1;
        /** Per vertex: indices into ModelSkin::joints and normalised weights. */
        std::vector<glm::uvec4> joints;
        std::vector<glm::vec4> weights;
        /** The latest skinned vertices, in model space. */
        std::vector<Vertex3D> skinned;
    };

    enum class ModelChannelPath
    {
        translation,
        rotation,
        scale
    };

    enum class ModelInterpolation
    {
        linear,
        step,
        cubic_spline
    };

    /** Keyframes for one property of one node. Rotation values are quaternions stored x, y, z, w. */
    struct ModelChannel
    {
        int node = -1;
        ModelChannelPath path = ModelChannelPath::translation;
        ModelInterpolation interpolation = ModelInterpolation::linear;
        std::vector<float> times;
        /** 3 (or 4 for rotation) floats per key; cubic splines store in-tangent, value, out-tangent per key. */
        std::vector<float> values;
    };

    /** A named keyframe animation, for example one exported from Blender's NLA tracks or actions. */
    struct ModelClip
    {
        std::string name;
        /** Time of the last keyframe, in seconds. */
        float duration = 0.0f;
        std::vector<ModelChannel> channels;
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
        std::vector<ModelAnimation> animations;
        std::vector<ModelVisibility> visibility;
        std::unordered_map<std::string, float> animation_properties;
        std::vector<std::string> unsupported_animations;
        /** glTF scene-graph nodes (empty for other formats). */
        std::vector<ModelNode> nodes;
        /** glTF keyframe animations. */
        std::vector<ModelClip> clips;
        /** glTF skins and the per-part skinning data (bounds of skinned parts are taken from the load-time pose). */
        std::vector<ModelSkin> skins;
        std::vector<ModelSkinBinding> skin_bindings;
        /** Playback state driven by play_model_clip() and update_model_clip(). */
        int active_clip = -1;
        float clip_time = 0.0f;
        float clip_speed = 1.0f;
        bool clip_loop = true;
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
     * factor and texture, normal texture, PBR metallic/roughness factors and texture (with occlusion in the same
     * texture) and emissive factor (an emissive texture is ignored, along with the factor, since only a constant glow
     * is supported), doubleSided, alphaMode MASK, and node keyframe animations
     * (translation, rotation and scale with linear, step and cubic-spline interpolation; see
     * play_model_clip()). Skins (JOINTS_0/WEIGHTS_0, CPU skinning, one set of 4 influences) are supported; morph targets and sparse accessors are ignored. This is the
     * recommended format; Blender 4.x exports it directly (File > Export > glTF 2.0, Y-up).
     * Returns nullptr on failure.
     */
    Model *load_gltf_model(const std::string &path);

    /**
     * Load an AC3D model (.ac), preserving object transforms, names, materials, UVs and textures.
     * Polygon surfaces are triangulated; line objects are ignored. Texture paths are relative
     * to the AC3D file.
     */
    Model *load_ac3d_model(const std::string &path);

    /**
     * Load the model path and supported rotate/translate/spin/blend animations from a FlightGear
     * model XML file, including conditional visibility. Other animation types are recorded in
     * Model::unsupported_animations and not evaluated.
     */
    Model *load_flightgear_model(const std::string &path);

    /** Load simlib's JSON animation format into an already loaded model. */
    bool load_model_animations_json(Model *model, const std::string &path);
    /** Save the model's supported transform animations as simlib JSON. */
    bool save_model_animations_json(const Model *model, const std::string &path);

    /** Load by file extension: .obj, .gltf, .glb or .ac. */
    Model *load_model(const std::string &path);

    void destroy_model(Model *model);

    /**
     * Write a model as a binary glTF 2.0 (.glb): parts, node hierarchy, tint/texture/normal-map/double-sided/alpha-mask
     * materials and keyframe clips. Meshes are GPU-only, so call set_mesh_cpu_data_retained(true) before loading the
     * model. FlightGear property animations have no glTF equivalent and are not written (see save_model_animations_json()).
     */
    bool save_model_gltf(const Model *model, const std::string &path);

    /** Index of the clip with this name, or -1. */
    int find_model_clip(const Model *model, const std::string &name);
    /** Duration in seconds of a clip, or 0 for an invalid index. */
    float model_clip_duration(const Model *model, int clip);
    /** Put every node back to the pose it was loaded with. */
    void reset_model_pose(Model *model);
    /**
     * Pose the model as clip `clip` looks at `time` seconds. With loop the time wraps at the clip's duration;
     * otherwise it clamps to the ends. The pose is reset first, so switching clips leaves nothing behind; pass
     * reset_pose = false to layer clips that animate different nodes. Returns false for an invalid clip.
     */
    bool sample_model_clip(Model *model, int clip, float time, bool loop = true, bool reset_pose = true);
    bool sample_model_clip(Model *model, const std::string &name, float time, bool loop = true, bool reset_pose = true);
    /** Start playing a clip from its beginning. update_model_clip() advances it. Returns false if the clip is unknown. */
    bool play_model_clip(Model *model, const std::string &name, bool loop = true, float speed = 1.0f);
    /** Stop playback and return to the rest pose. */
    void stop_model_clip(Model *model);
    /** Advance the playing clip by dt seconds and pose the model. Does nothing when no clip is playing. */
    void update_model_clip(Model *model, float dt);
    /** True while a clip is playing (a non-looping clip stops at its end). */
    bool model_clip_playing(const Model *model);

    /** Set an input property used by the model's property-driven animations. */
    bool set_model_property(Model *model, const std::string &property, float value);

    /** True when the part's visibility flag and any imported select rules currently allow it to draw. */
    bool model_part_is_visible(const Model *model, const ModelPart &part);

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
