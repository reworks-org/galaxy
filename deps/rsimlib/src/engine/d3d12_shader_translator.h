/** @file
 * @brief Translates engine GLSL into HLSL for the D3D12 backend (glslang -> SPIR-V -> SPIRV-Cross).
 */
#pragma once

#ifdef _WIN32

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace sl::detail
{
    enum class TranslateStage
    {
        vertex,
        fragment,
        compute
    };

    struct TranslatedUniform
    {
        std::uint32_t offset = 0;
        std::uint32_t size = 0;
    };

    /** Sampler slot i uses t/s register i in register space 0. */
    /** Image (image2D) bindings map to UAV registers u16..u23. */
    constexpr std::uint32_t translated_image_base_register = 16;
    constexpr std::uint32_t translated_max_images = 8;
    constexpr std::uint32_t translated_vertex_constants_register = 12;
    constexpr std::uint32_t translated_pixel_constants_register = 13;

    struct TranslatedShader
    {
        std::string hlsl;
        /** Loose GLSL uniforms, packed into one std140 constant buffer. */
        std::unordered_map<std::string, TranslatedUniform> uniforms;
        std::uint32_t uniform_bytes = 0;
        /** Sampler names in slot order; slot i is register t/s(16 + i). */
        std::vector<std::string> samplers;
        /** Vertex stage output locations by name, used to link fragment inputs. */
        std::unordered_map<std::string, int> varyings;
        /** image2D binding indices this stage declares. */
        std::vector<std::uint32_t> images;
        /** Number of storage-buffer registers (u0..u(n-1)) the shader declares. */
        std::uint32_t storage_registers = 0;
    };

    /** True when the build includes the glslang/SPIRV-Cross translation libraries. */
    bool d3d12_shader_translation_available();

    /** Translate GLSL for one stage. vertex_varyings links fragment inputs to vertex outputs by name. */
    bool translate_glsl_to_hlsl(const std::string &glsl, TranslateStage stage,
                                const std::unordered_map<std::string, int> *vertex_varyings,
                                std::vector<std::string> *shared_samplers,
                                TranslatedShader &out, std::string &error);
}

#endif // _WIN32
