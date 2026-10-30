/** @file
 * @brief Translates engine GLSL into HLSL for the D3D12 backend (glslang -> SPIR-V -> SPIRV-Cross).
 */
#ifdef _WIN32

#include "d3d12_shader_translator.h"

#include <algorithm>
#include <cstdlib>
#include <regex>
#include <set>
#include <sstream>

#ifdef SIMLIB_D3D12_SHADER_TRANSLATION
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
// Source-tree layout (glslang/SPIRV-Cross fetched by CMake): __has_include must not pick up a different installed version.
#if !defined(SIMLIB_TRANSLATION_FETCHED) && __has_include(<glslang/SPIRV/GlslangToSpv.h>)
#include <glslang/SPIRV/GlslangToSpv.h>
#else
#include <SPIRV/GlslangToSpv.h>
#endif
#if !defined(SIMLIB_TRANSLATION_FETCHED) && __has_include(<spirv_cross/spirv_hlsl.hpp>)
#include <spirv_cross/spirv_hlsl.hpp>
#else
#include <spirv_hlsl.hpp>
#endif
#include <mutex>
#endif

namespace sl::detail
{
    namespace
    {
        struct TypeLayout
        {
            std::uint32_t size = 0;
            std::uint32_t alignment = 0;
        };

        bool scalar_or_vector_layout(const std::string &type, TypeLayout &layout)
        {
            static const std::unordered_map<std::string, TypeLayout> layouts = {
                {"float", {4, 4}}, {"int", {4, 4}}, {"uint", {4, 4}}, {"bool", {4, 4}},
                {"vec2", {8, 8}}, {"ivec2", {8, 8}}, {"uvec2", {8, 8}}, {"bvec2", {8, 8}},
                {"vec3", {12, 16}}, {"ivec3", {12, 16}}, {"uvec3", {12, 16}}, {"bvec3", {12, 16}},
                {"vec4", {16, 16}}, {"ivec4", {16, 16}}, {"uvec4", {16, 16}}, {"bvec4", {16, 16}},
                {"mat2", {32, 16}}, {"mat3", {48, 16}}, {"mat4", {64, 16}}};
            const auto found = layouts.find(type);
            if (found == layouts.end())
                return false;
            layout = found->second;
            return true;
        }

        std::uint32_t align_up(std::uint32_t value, std::uint32_t alignment)
        {
            return (value + alignment - 1) / alignment * alignment;
        }

        std::string strip_comment(const std::string &line)
        {
            const std::size_t comment = line.find("//");
            return comment == std::string::npos ? line : line.substr(0, comment);
        }

        struct UniformDecl
        {
            std::string type;
            std::string name;
            std::uint32_t count = 0;
        };

        bool preprocess_glsl(const std::string &glsl, TranslateStage stage,
                             const std::unordered_map<std::string, int> *vertex_varyings,
                             std::vector<std::string> *shared_samplers,
                             TranslatedShader &out, std::string &processed, std::string &error)
        {
            static const std::regex version_pattern(R"(^\s*#\s*version\b.*)");
            static const std::regex sampler_pattern(
                R"(^\s*uniform\s+(?:(?:highp|mediump|lowp)\s+)?(sampler\w+)\s+(\w+)\s*;.*)");
            static const std::regex image_pattern(
                R"(^\s*layout\s*\(([^)]*)\)\s*uniform\s+((?:(?:readonly|writeonly|coherent|volatile|restrict|highp|mediump|lowp)\s+)*)(image\w+)\s+(\w+)\s*;.*)");
            static const std::regex uniform_pattern(
                R"(^\s*uniform\s+(?:(?:highp|mediump|lowp)\s+)?(\w+)\s+(\w+)\s*(?:\[\s*(\d+)\s*\])?\s*;.*)");
            static const std::regex varying_pattern(
                R"(^\s*((?:flat|smooth|noperspective)\s+)?(in|out)\s+((?:(?:highp|mediump|lowp)\s+)?\w+)\s+(\w+)\s*;.*)");
            static const std::regex located_pattern(
                R"(^\s*layout\s*\(\s*location\s*=\s*(\d+)\s*\)\s*(?:(?:flat|smooth|noperspective)\s+)?(in|out)\s+(?:(?:highp|mediump|lowp)\s+)?\w+\s+(\w+)\s*;.*)");

            std::vector<std::string> lines;
            {
                std::istringstream stream(glsl);
                std::string line;
                while (std::getline(stream, line))
                {
                    if (!line.empty() && line.back() == '\r')
                        line.pop_back();
                    lines.push_back(line);
                }
            }

            std::vector<std::string> output;
            std::vector<UniformDecl> uniforms;
            std::size_t block_line = std::string::npos;
            std::set<int> used_outputs;
            int next_output = 0;
            int next_fragment_output = 0;
            int next_unmatched_input = 16;
            int depth = 0;

            // Explicit vertex output locations must be known before auto-assigning the others.
            if (stage == TranslateStage::vertex)
            {
                int scan_depth = 0;
                for (const std::string &line : lines)
                {
                    const std::string code = strip_comment(line);
                    std::smatch match;
                    if (scan_depth == 0 && std::regex_match(code, match, located_pattern) && match[2] == "out")
                    {
                        used_outputs.insert(std::atoi(match[1].str().c_str()));
                        out.varyings[match[3].str()] = std::atoi(match[1].str().c_str());
                    }
                    scan_depth += static_cast<int>(std::count(code.begin(), code.end(), '{')) -
                                  static_cast<int>(std::count(code.begin(), code.end(), '}'));
                }
            }

            for (const std::string &line : lines)
            {
                const std::string code = strip_comment(line);
                std::string emitted = line;
                std::smatch match;
                if (depth == 0)
                {
                    if (std::regex_match(code, version_pattern))
                    {
                        emitted.clear();
                    }
                    else if (std::regex_match(code, match, sampler_pattern))
                    {
                        // Slots are shared across stages so a sampler keeps one slot per program.
                        std::vector<std::string> &slots = shared_samplers ? *shared_samplers : out.samplers;
                        auto existing = std::find(slots.begin(), slots.end(), match[2].str());
                        if (existing == slots.end())
                            existing = slots.insert(slots.end(), match[2].str());
                        const std::uint32_t slot = static_cast<std::uint32_t>(existing - slots.begin());
                        if (&slots != &out.samplers)
                            out.samplers.push_back(match[2].str());
                        emitted = "layout(set = 0, binding = " + std::to_string(slot) +
                                  ") uniform " + match[1].str() + " " + match[2].str() + ";";
                    }
                    else if (std::regex_match(code, match, image_pattern))
                    {
                        // Images get their own register range so they cannot collide with storage buffers.
                        std::string rest;
                        int binding = -1;
                        std::istringstream items(match[1].str());
                        std::string item;
                        while (std::getline(items, item, ','))
                        {
                            std::smatch binding_match;
                            if (std::regex_match(item, binding_match, std::regex(R"(\s*binding\s*=\s*(\d+)\s*)")))
                                binding = std::atoi(binding_match[1].str().c_str());
                            else
                                rest += item + ",";
                        }
                        if (binding < 0)
                            binding = static_cast<int>(out.images.size());
                        if (binding >= static_cast<int>(translated_max_images))
                        {
                            error = "Image binding " + std::to_string(binding) + " exceeds the D3D12 limit.";
                            return false;
                        }
                        if (std::find(out.images.begin(), out.images.end(), static_cast<std::uint32_t>(binding)) ==
                            out.images.end())
                            out.images.push_back(static_cast<std::uint32_t>(binding));
                        emitted = "layout(" + rest + "set = 0, binding = " +
                                  std::to_string(translated_image_base_register + binding) + ") uniform " +
                                  match[2].str() + match[3].str() + " " + match[4].str() + ";";
                    }
                    else if (std::regex_match(code, match, uniform_pattern))
                    {
                        UniformDecl decl;
                        decl.type = match[1].str();
                        decl.name = match[2].str();
                        decl.count = match[3].matched ? static_cast<std::uint32_t>(std::atoi(match[3].str().c_str())) : 0;
                        uniforms.push_back(decl);
                        if (block_line == std::string::npos)
                            block_line = output.size();
                        emitted.clear();
                    }
                    else if (std::regex_match(code, match, varying_pattern))
                    {
                        const std::string qualifier = match[1].str();
                        const bool is_input = match[2] == "in";
                        const std::string name = match[4].str();
                        int location = 0;
                        if (stage == TranslateStage::vertex && !is_input)
                        {
                            while (used_outputs.count(next_output))
                                ++next_output;
                            location = next_output++;
                            used_outputs.insert(location);
                            out.varyings[name] = location;
                        }
                        else if (stage == TranslateStage::fragment && is_input)
                        {
                            const auto linked = vertex_varyings ? vertex_varyings->find(name)
                                                                : std::unordered_map<std::string, int>::const_iterator{};
                            if (vertex_varyings && linked != vertex_varyings->end())
                                location = linked->second;
                            else
                                location = next_unmatched_input++;
                        }
                        else if (stage == TranslateStage::fragment && !is_input)
                        {
                            location = next_fragment_output++;
                        }
                        else
                        {
                            location = 0;
                        }
                        emitted = "layout(location = " + std::to_string(location) + ") " + qualifier +
                                  match[2].str() + " " + match[3].str() + " " + name + ";";
                    }
                }
                depth += static_cast<int>(std::count(code.begin(), code.end(), '{')) -
                         static_cast<int>(std::count(code.begin(), code.end(), '}'));
                output.push_back(emitted);
            }

            std::uint32_t offset = 0;
            std::string block;
            if (!uniforms.empty())
            {
                const std::uint32_t binding = stage == TranslateStage::vertex
                                                  ? translated_vertex_constants_register
                                                  : translated_pixel_constants_register;
                block = "layout(std140, set = 0, binding = " + std::to_string(binding) + ") uniform SlParams {\n";
                for (const UniformDecl &decl : uniforms)
                {
                    TypeLayout layout;
                    if (!scalar_or_vector_layout(decl.type, layout))
                    {
                        error = "Unsupported uniform type '" + decl.type + "' for '" + decl.name + "'.";
                        return false;
                    }
                    std::uint32_t size = layout.size;
                    std::uint32_t alignment = layout.alignment;
                    if (decl.count > 0)
                    {
                        alignment = 16;
                        size = align_up(layout.size, 16) * decl.count;
                    }
                    offset = align_up(offset, alignment);
                    out.uniforms[decl.name] = {offset, size};
                    offset += size;
                    block += "    " + decl.type + " " + decl.name;
                    if (decl.count > 0)
                        block += "[" + std::to_string(decl.count) + "]";
                    block += ";\n";
                }
                block += "};";
                out.uniform_bytes = align_up(offset, 16);
                output[block_line] = block;
            }

            processed = "#version 450\n";
            for (const std::string &line : output)
            {
                processed += line;
                processed += '\n';
            }
            return true;
        }

        void count_storage_registers(TranslatedShader &out)
        {
            static const std::regex register_pattern(R"(register\(u(\d+))");
            std::uint32_t count = 0;
            for (std::sregex_iterator it(out.hlsl.begin(), out.hlsl.end(), register_pattern), end; it != end; ++it)
            {
                const std::uint32_t index = static_cast<std::uint32_t>(std::atoi((*it)[1].str().c_str()));
                if (index < translated_image_base_register)
                    count = (std::max)(count, index + 1);
            }
            out.storage_registers = count;
        }
    }

#ifdef SIMLIB_D3D12_SHADER_TRANSLATION
    bool d3d12_shader_translation_available() { return true; }

    bool translate_glsl_to_hlsl(const std::string &glsl, TranslateStage stage,
                                const std::unordered_map<std::string, int> *vertex_varyings,
                                std::vector<std::string> *shared_samplers,
                                TranslatedShader &out, std::string &error)
    {
        out = {};
        std::string processed;
        if (!preprocess_glsl(glsl, stage, vertex_varyings, shared_samplers, out, processed, error))
            return false;

        static std::once_flag initialised;
        std::call_once(initialised, [] { glslang::InitializeProcess(); });

        const EShLanguage language = stage == TranslateStage::vertex     ? EShLangVertex
                                     : stage == TranslateStage::fragment ? EShLangFragment
                                                                         : EShLangCompute;
        glslang::TShader shader(language);
        const char *text = processed.c_str();
        shader.setStrings(&text, 1);
        shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 100);
        shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
        shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
        const EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
        if (!shader.parse(GetDefaultResources(), 450, false, messages))
        {
            error = std::string("GLSL compile failed: ") + shader.getInfoLog();
            return false;
        }
        glslang::TProgram program;
        program.addShader(&shader);
        if (!program.link(messages))
        {
            error = std::string("GLSL link failed: ") + program.getInfoLog();
            return false;
        }
        std::vector<unsigned int> spirv;
        glslang::GlslangToSpv(*program.getIntermediate(language), spirv);
        if (spirv.empty())
        {
            error = "GLSL to SPIR-V conversion produced no output.";
            return false;
        }

        try
        {
            spirv_cross::CompilerHLSL compiler(std::move(spirv));
            spirv_cross::CompilerHLSL::Options options = compiler.get_hlsl_options();
            options.shader_model = 51;
            options.point_size_compat = true;
            options.point_coord_compat = true;
            options.force_storage_buffer_as_uav = true;
            compiler.set_hlsl_options(options);
            out.hlsl = compiler.compile();
        }
        catch (const std::exception &exception)
        {
            error = std::string("SPIR-V to HLSL failed: ") + exception.what();
            return false;
        }
        count_storage_registers(out);
        return true;
    }
#else
    bool d3d12_shader_translation_available() { return false; }

    bool translate_glsl_to_hlsl(const std::string &, TranslateStage, const std::unordered_map<std::string, int> *,
                                std::vector<std::string> *, TranslatedShader &, std::string &error)
    {
        error = "This build has no GLSL translation support for D3D12. "
                "Install glslang and spirv-cross (vcpkg install glslang:x64-windows spirv-cross:x64-windows) and reconfigure.";
        return false;
    }
#endif
}

#endif // _WIN32
